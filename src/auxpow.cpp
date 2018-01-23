// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2011 Vince Durham
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 Daniel Kraft
// Copyright (c) 2014-2015 The Dogecoin Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file license.txt or http://www.opensource.org/licenses/mit-license.php.

#include "auxpow.h"

#include "hash.h"
#include "primitives/transaction.h"

#include "chainparams.h"
#include "consensus/validation.h"
#include "main.h"
#include "script/script.h"
#include "util.h"

#include <algorithm>

uint256 CAuxPowSupplement::getIntermediateHash(uint256 hash, int nSubChainId) const
{
    int nChainIndex = CAuxPow::getExpectedIndex(nSubNonce, nSubChainId, vSubChainMerkleBranch.size());
    uint256 subRoot = CBlock::CheckMerkleBranch(hash, vSubChainMerkleBranch, nChainIndex);
    std::vector<unsigned char> vchsubRoot(subRoot.begin(), subRoot.end());
    std::reverse(vchsubRoot.begin(), vchsubRoot.end());
    
    uint32_t nSubSize = (1 << vSubChainMerkleBranch.size());
    unsigned char* pSize = reinterpret_cast<unsigned char*>(&nSubSize);
    vchsubRoot.insert(vchsubRoot.end(), pSize, pSize + 4);
    uint32_t nSubNonceNotConst = nSubNonce;
    unsigned char* pNonce = reinterpret_cast<unsigned char*>(&nSubNonceNotConst);
    vchsubRoot.insert(vchsubRoot.end(), pNonce, pNonce + 4);
    
    return Hash(vchsubRoot.begin(),vchsubRoot.end());
}

bool
CAuxPow::check(const uint256& hashAuxBlock, const Consensus::Params& params) const
{
    if (vChainMerkleBranch.size() > 20)
        return error("Aux POW chain merkle branch too long");
    
    //check coinbase
    std::vector<unsigned char>::const_iterator pcHead =
        std::search(coinbaseTx.begin(), coinbaseTx.end(), UBEGIN(pchMergedMiningHeader), UEND(pchMergedMiningHeader));

    if (pcHead == coinbaseTx.end())
        return error("Aux POW missing MergedMiningHeader in parent coinbase");
    
    pcHead = pcHead + sizeof(pchMergedMiningHeader);
    
    uint32_t nSize;
    memcpy(&nSize, &pcHead[32], 4);
    const unsigned merkleHeight = vChainMerkleBranch.size();
    if (nSize != (1 << merkleHeight))
        return error("Aux POW merkle branch size does not match parent coinbase");
    
    uint32_t nNonce;
    memcpy(&nNonce, &pcHead[36], 4);
    
    // get chain merkle root
    int nChainIndex;
    nChainIndex = getExpectedIndex(nNonce, params.nAuxpowChainId, merkleHeight);
    
    const uint256 nRootHash = CBlock::CheckMerkleBranch(hashAuxBlock, vChainMerkleBranch, nChainIndex);
    std::vector<unsigned char> vchRootHash(nRootHash.begin(), nRootHash.end());
    std::reverse(vchRootHash.begin(), vchRootHash.end()); // correct endian
    
    // check that chain merkle root is in the coinbase
    std::vector<unsigned char>::const_iterator pc =
    std::search(pcHead, pcHead + 32, vchRootHash.begin(), vchRootHash.end());
    if (pcHead != pc)
        return error("chain merkle root is not just after merged mining header");
    
    // Check the coinbase is in the parent block merkle tree
    if (CBlock::CheckMerkleBranch(Hash(coinbaseTx.begin(), coinbaseTx.end()), vMerkleBranch, 0) != parentBlock.hashMerkleRoot) {
        return error("Aux POW merkle root incorrect");        
    }

    return true;
}

bool
CAuxPow::check2(const uint256& hashAuxBlock, const Consensus::Params& params) const
{
    int nSubtreeLayer = vSubTree.size();
    if (nSubtreeLayer > 4)
        return error("Too many sub tree layer");
    int nTotalHeight = vChainMerkleBranch.size();
    for (int i = 0; i < nSubtreeLayer; i++) {
        nTotalHeight += vSubTree[i].vSubChainMerkleBranch.size();
    }
    if (nTotalHeight > 20)
        return error("Aux POW chain merkle branch too long");
    
    uint256 intermediateHash = hashAuxBlock;
    for (int i = nSubtreeLayer - 1; i >= 0; i--) {
        intermediateHash = vSubTree[i].getIntermediateHash(intermediateHash,params.nSubAuxpowChainId);
    }    
    
    return check(intermediateHash, params);
}

int
CAuxPow::getExpectedIndex(uint32_t nNonce, int nChainId, unsigned h)
{
    // Choose a pseudo-random slot in the chain merkle tree
    // but have it be fixed for a size/nonce/chain combination.
    //
    // This prevents the same work from being used twice for the
    // same chain while reducing the chance that two chains clash
    // for the same slot.

    unsigned rand = nNonce;
    rand = rand * 1103515245 + 12345;
    rand += nChainId;
    rand = rand * 1103515245 + 12345;

    return rand % (1 << h);
}

CPureTransaction::CPureTransaction() : nVersion(0), vin(), vout(), nLockTime(0) { }

std::string CPureTransaction::ToHex() const 
{
    CDataStream ssTx(SER_NETWORK, PROTOCOL_VERSION);
    ssTx << (*this);
    return HexStr(ssTx.begin(), ssTx.end());
}