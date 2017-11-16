#include "leveldbwrapper.h"
#include "leveldbwrapper.cpp"
#include "uint256.h"
#include "uint256.cpp"
#include "coins.h"
#include "coins.cpp"
#include "util.h"
#include "util.cpp"
#include "utiltime.h"
#include "utiltime.cpp"
#include "utilstrencodings.cpp"
#include "utilstrencodings.h"
#include "random.h"
#include "random.cpp"
#include "compressor.h"
#include "compressor.cpp"
#include "hash.h"
#include "hash.cpp"
#include "pubkey.h"
#include "pubkey.cpp"
#include "script/script.h"
#include "script/script.cpp"
#include "support/cleanse.h"
#include "support/cleanse.cpp"
#include "crypto/sha256.h"
#include "crypto/sha256.cpp"
#include "crypto/ripemd160.h"
#include "crypto/ripemd160.cpp"
#include "crypto/sha512.h"
#include "crypto/sha512.cpp"
#include "crypto/hmac_sha512.h"
#include "crypto/hmac_sha512.cpp"
#include "primitives/transaction.h"
#include "primitives/transaction.cpp"
#include "primitives/block.h"
#include "primitives/block.cpp"
#include "primitives/pureheader.h"
#include "primitives/pureheader.cpp"
#include "chainparamsbase.h"
#include "chainparamsbase.cpp"

#include "zcash/IncrementalMerkleTree.cpp"
#include "zcash/IncrementalMerkleTree.hpp"
#include "zcash/NoteEncryption.cpp"
#include "zcash/NoteEncryption.hpp"
#include "zcash/Note.cpp"
#include "zcash/Note.hpp"
#include "zcash/util.cpp"
#include "zcash/prf.cpp"

#include "secp256k1/src/secp256k1.c"
#include "secp256k1/src/modules/recovery/main_impl.h"

#include "sodium.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <stdint.h>
#include <string>
#include <stdlib.h> 
#include <stdio.h>

#include <boost/date_time/time_parsing.hpp>

int main(int argc, char* argv[]) {

    CBlock genesis;
    genesis.SetNull();
    const char* pszTimestamp = "Zcash0b9c4eef8b7cc417ee5001e3500984b6fea35683a7cac141a043c42064835d34";
    
    CMutableTransaction txNew;
    txNew.vin.resize(1);
    txNew.vout.resize(2);
    txNew.vin[0].scriptSig = CScript() << 520617983 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = 0;
    txNew.vout[0].scriptPubKey = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
    
    std::vector<unsigned char> rawHex = ParseHex("2103b70627d03bc93394d3ca84554f3d64699893400513f2f60ba0d4d8ceb0886a4fac"); 
	CScript scriptPubKey(&rawHex.front(),&rawHex.back()+1); // To do: +1

    txNew.vout[1].nValue = 90000000000000;
    txNew.vout[1].scriptPubKey = scriptPubKey;
    
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = genesis.BuildMerkleTree();
    genesis.nVersion = 4;
    genesis.nTime    = 1296688602;
    genesis.nBits    = 0x207fffff;
    genesis.nNonce   = 2;
    uint256 genesisHash = genesis.GetHash();
    uint256 txHash = genesis.vtx[0].GetHash(); 
    /* Open LEVELDB database */
    const std::string PATH = "/btc/bitcoinzero/regtest/chainstate";
	
	CLevelDBWrapper db(PATH, 1024*64, false, true);
	
	static const char DB_COINS = 'c';
    static const char DB_BEST_BLOCK = 'B';

    db.Write(DB_BEST_BLOCK,genesisHash);
    
    CCoins coins;
    
    coins.fCoinBase = true;
    coins.vout = txNew.vout;
    coins.nHeight = 0;
    coins.nVersion = 4;
    
    db.Write(std::make_pair(DB_COINS, txHash), coins);
      
    std::cout << "block hash: " << genesisHash.ToString() << "\n";
    std::cout << "tree root: " << genesis.hashMerkleRoot.ToString() << "\n";
    std::cout << "txid: " << txHash.ToString() << "\n";
    return 0;
}
