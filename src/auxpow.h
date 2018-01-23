// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2014-2015 Daniel Kraft
// Distributed under the MIT/X11 software license, see the accompanying
// file license.txt or http://www.opensource.org/licenses/mit-license.php.

#ifndef BITCOIN_AUXPOW_H
#define BITCOIN_AUXPOW_H

#include "consensus/params.h"
#include "primitives/pureheader.h"
#include "primitives/transaction.h"
#include "serialize.h"
#include "uint256.h"

#include <vector>

class CBlock;
class CBlockIndex;

/** Header for merge-mining data in the coinbase.  */
static const unsigned char pchMergedMiningHeader[] = {0xfa, 0xbe, 'm', 'm'};

class CAuxPowSupplement
{
public:
    std::vector<uint256> vSubChainMerkleBranch;
    uint32_t nSubNonce;

public:
    CAuxPowSupplement() {
        nSubNonce = 0;
    }

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void
    SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion)
    {
        READWRITE(vSubChainMerkleBranch);
        READWRITE(nSubNonce);
    }
    
    uint256 getIntermediateHash(uint256 hash, int nChainId) const; 
};

class CAuxPow
{
    
public:
    /** The merkle branch connecting the aux block to our coinbase.  */
    std::vector<uint256> vChainMerkleBranch;
    std::vector<uint256> vMerkleBranch;
    
    /** Parent block header (on which the real PoW is done).  */
    CPureBlockHeader parentBlock;
    std::vector<unsigned char> coinbaseTx;
    
    /** subtree. */
    std::vector<CAuxPowSupplement> vSubTree;

public:
    CAuxPow(){}
    
    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void
    SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion)
    {
        READWRITE(parentBlock);
        READWRITE(coinbaseTx);        
        READWRITE(vMerkleBranch);      
        READWRITE(vChainMerkleBranch);
        READWRITE(vSubTree);
    }

    /**
   * Check the auxpow, given the merge-mined block's hash and our chain ID.
   * Note that this does not verify the actual PoW on the parent block!  It
   * just confirms that all the merkle branches are valid.
   * @param hashAuxBlock Hash of the merge-mined block.
   * @param params Consensus parameters.
   * @return True if the auxpow is valid.
   */
    bool check(const uint256& hashAuxBlock, const Consensus::Params& params) const;
    bool check2(const uint256& hashAuxBlock, const Consensus::Params& params) const;

    /**
   * Get the parent block's hash.  This is used to verify that it
   * satisfies the PoW requirement.
   * @return The parent block hash.
   */
    inline uint256
    getParentBlockPoWHash() const
    {
        return parentBlock.GetPoWHash();
    }
    
    /**
   * Calculate the expected index in the merkle tree.  This is also used
   * for the test-suite.
   * @param nNonce The coinbase's nonce value.
   * @param nChainId The chain ID.
   * @param h The merkle block height.
   * @return The expected index for the aux hash.
   */
    static int getExpectedIndex(uint32_t nNonce, int nChainId, unsigned h);
};

/** Add support for classic auxpow */
class CPureTransaction
{

public:
    // The local variables are made const to prevent unintended modification
    // without updating the cached hash value. However, CTransaction is not
    // actually immutable; deserialization and assignment are implemented,
    // and bypass the constness. This is safe, as they update the entire
    // structure, including the hash.
    const int32_t nVersion;
    const std::vector<CTxIn> vin;
    const std::vector<CTxOut> vout;
    const uint32_t nLockTime;

    /** Construct a CTransaction that qualifies as IsNull() */
    CPureTransaction();

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion) {
        READWRITE(*const_cast<int32_t*>(&this->nVersion));
        nVersion = this->nVersion;
        READWRITE(*const_cast<std::vector<CTxIn>*>(&vin));
        READWRITE(*const_cast<std::vector<CTxOut>*>(&vout));
        READWRITE(*const_cast<uint32_t*>(&nLockTime));
    }
    
    std::string ToHex() const;
};

class CPureMerkleTx : public CPureTransaction
{

public:
    uint256 hashBlock;
    std::vector<uint256> vMerkleBranch;
    int nIndex;

    CPureMerkleTx(){}

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion)
    {
        READWRITE(*(CPureTransaction*)this);
        nVersion = this->nVersion;
        READWRITE(hashBlock);
        READWRITE(vMerkleBranch);
        READWRITE(nIndex);
    }
};

class CClassicAuxPow : public CPureMerkleTx
{
    /* Public for the unit tests.  */
public:
    /** The merkle branch connecting the aux block to our coinbase.  */
    std::vector<uint256> vChainMerkleBranch;

    /** Merkle tree index of the aux block header in the coinbase.  */
    int nChainIndex;

    /** Parent block header (on which the real PoW is done).  */
    CPureBlockHeader parentBlock;

public:
    /* Prevent accidental conversion.  */

    CClassicAuxPow(){}

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void
    SerializationOp(Stream& s, Operation ser_action, int nType, int nVersion)
    {
        READWRITE(*static_cast<CPureMerkleTx*>(this));
        nVersion = this->nVersion;

        READWRITE(vChainMerkleBranch);
        READWRITE(nChainIndex);
        READWRITE(parentBlock);
    }
};

#endif // BITCOIN_AUXPOW_H
