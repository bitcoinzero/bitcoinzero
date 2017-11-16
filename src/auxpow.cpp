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

std::string hash_swap (std::string input) {
    std::string output;
    for (int i = 0; i < 32; i++) {
        output += input[62-i*2];
        output += input[62-i*2+1];
    }
    return  output;
}
/* ************************************************************************** */

bool
CAuxPow::check(const uint256& hashAuxBlock, const Consensus::Params& params) const
{
    if (nIndex != 0)
        return error("AuxPow is not a generate");

    if (vChainMerkleBranch.size() > 30)
        return error("Aux POW chain merkle branch too long");

    // Check that the chain merkle root is in the coinbase
    uint256 rehashAuxBlock;
    rehashAuxBlock.SetHex(hash_swap(hashAuxBlock.ToString()));
    const uint256 nRootHash = CBlock::CheckMerkleBranch(SerializeHash(rehashAuxBlock), vChainMerkleBranch, nChainIndex);
    std::vector<unsigned char> vchRootHash(nRootHash.begin(), nRootHash.end());
    
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
    
    return true;
}
