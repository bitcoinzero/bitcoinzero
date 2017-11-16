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
    
    const char* pszTimestamp = "Zcash0b9c4eef8b7cc417ee5001e3500984b6fea35683a7cac141a043c42064835d34";
    
    CMutableTransaction txNew;
    txNew.vin.resize(1);
    txNew.vout.resize(1+16529043);
    txNew.vin[0].scriptSig = CScript() << 520617983 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    
        /* Open LEVELDB database */
    const std::string PATH = "/home/suiqiu/chainstate";
	
	CLevelDBWrapper db(PATH, 1024*64, false, false);
	
	static const char DB_COINS = 'c';
    static const char DB_BEST_BLOCK = 'B';
    
    uint256 hashBestChain;
	db.Read(DB_BEST_BLOCK, hashBestChain);
    
    uint256 txid;
	CCoins coins;
    
    txid.SetHex("7c239ae5d5888bd66d60a6daf5fc0cb98ebca41aad2cb13839af1aa232b5f953");
    
    db.Read(std::make_pair(DB_COINS, txid),coins);
    
    std::cout << hashBestChain.ToString() << "\n";
    std::cout << txid.ToString() << "\n";
    
    txNew.vout = coins.vout;
    
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = genesis.BuildMerkleTree();
    genesis.nTime    = 1477641360;
    genesis.nBits    = 0x1f07ffff;
    genesis.nNonce   = 0;
	uint256 genesisHash = genesis.GetHash();
    uint256 txHash = genesis.vtx[0].GetHash();    
    std::cout << genesisHash.ToString() << "\n";
    //d0a2116a32a88a6393a1f6ce9d61ceb9fbe366f828cfdf785b662b481d550795
    std::cout << txHash.ToString() << "\n";
    
    return 0;
}
