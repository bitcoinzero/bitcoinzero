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

/* ************************************************************************** */
// This is flawed. I am working on an alternative.
bool
CAuxPow::check(const uint256& hashAuxBlock, const Consensus::Params& params) const
{
    if (nIndex != 0)
        return error("AuxPow is not a generate");

    if (vChainMerkleBranch.size() > 30)
        return error("Aux POW chain merkle branch too long");

    // Check that the chain merkle root is in the coinbase

    const uint256 nRootHash = CBlock::CheckMerkleBranch(hashAuxBlock, vChainMerkleBranch, nChainIndex);
    std::vector<unsigned char> vchRootHash(nRootHash.begin(), nRootHash.end());
    std::reverse(vchRootHash.begin(), vchRootHash.end()); // correct endian
    
    // Check that we are in the parent block merkle tree
    if (CBlock::CheckMerkleBranch(Hash(coinbaseTx.begin(), coinbaseTx.end()), vMerkleBranch, nIndex) != parentBlock.hashMerkleRoot) {
        return error("Aux POW merkle root incorrect");        
    }
        
    std::vector<unsigned char>::const_iterator pcHead =
        std::search(coinbaseTx.begin(), coinbaseTx.end(), UBEGIN(pchMergedMiningHeader), UEND(pchMergedMiningHeader));

    if (pcHead == coinbaseTx.end())
        return error("Aux POW missing MergedMiningHeader in parent coinbase");
    
    std::vector<unsigned char>::const_iterator pc =
        std::search(pcHead + sizeof(pchMergedMiningHeader), coinbaseTx.end(), vchRootHash.begin(), vchRootHash.end());

    if (pc == coinbaseTx.end())
        return error("Aux POW missing chain merkle root in parent coinbase");

    if (pcHead + sizeof(pchMergedMiningHeader) != pc)
        return error("Merged mining header is not just before chain merkle root");    
    
    // Ensure we are at a deterministic point in the merkle leaves by hashing
    // a nonce and our chain ID and comparing to the index.
    pc += vchRootHash.size();
    if (coinbaseTx.end() - pc < 8)
        return error("Aux POW missing chain merkle tree size and nonce in parent coinbase");
    
    int nSize;
    memcpy(&nSize, &pc[0], 4);
    const unsigned merkleHeight = vChainMerkleBranch.size();
    if (nSize != (1 << merkleHeight))
        return error("Aux POW merkle branch size does not match parent coinbase");

    int nNonce;
    memcpy(&nNonce, &pc[4], 4);

    if (nChainIndex != getExpectedIndex(nNonce, params.nAuxpowChainId, merkleHeight))
        return error("Aux POW wrong index");

    return true;
}

int
CAuxPow::getExpectedIndex(int nNonce, int nChainId, unsigned h)
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
