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
#include <sqlite3.h>

static int callback(void *txNew, int argc, char **argv, char **azColName){
    CMutableTransaction* tx = (CMutableTransaction*)txNew;
    CTxOut tmp;
    tmp.nValue = atol(argv[1]);
    std::vector<unsigned char> rawHex = ParseHex(argv[0]);
	CScript scriptPubKey(&rawHex.front(),&rawHex.back()+1);
    tmp.scriptPubKey = scriptPubKey;
    tx->vout.push_back(tmp);
    // std::cout << argv[0] << "\n" << argv[1] << "\n";
	return 0;
}

int main(int argc, char* argv[]) {

    CBlock genesis;
    genesis.SetNull();
    const char* pszTimestamp = "Zcash0b9c4eef8b7cc417ee5001e3500984b6fea35683a7cac141a043c42064835d34";
    
    CMutableTransaction txNew;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 520617983 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = 0;
    txNew.vout[0].scriptPubKey = CScript() << ParseHex("04678afdb0fe5548271967f1a67130b7105cd6a828e03909a67962e0ea1f61deb649f6bc3f4cef38c4f35504e51ec112de5c384df7ba0b8d578a4c702b6bf11d5f") << OP_CHECKSIG;
       
	sqlite3 *dbsql;
	char *sql;
	char *zErrMsg = 0;
    int rc;
	
	/* Open SQL database */
	rc = sqlite3_open("/home/blockchain/leveldbview/bitcoin-master/src/478558_A_1460.db", &dbsql);
    if( rc ){
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(dbsql));
    }else{
        fprintf(stdout, "Opened database successfully\n");
    }

    sql = "SELECT * from AGGREGATE";
    rc = sqlite3_exec(dbsql, sql, callback, &txNew, &zErrMsg);
    if( rc != SQLITE_OK ){
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
        sqlite3_free(zErrMsg);
    }

    sqlite3_close(dbsql);
    
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock.SetNull();
    genesis.hashMerkleRoot = genesis.BuildMerkleTree();
    genesis.nTime    = 1477641360;
    genesis.nBits    = 0x1f07ffff;
    genesis.nNonce   = 0;
	uint256 genesisHash = genesis.GetHash();
    uint256 txHash = genesis.vtx[0].GetHash();    
    std::cout << genesisHash.ToString() << "\n";
    std::cout << txHash.ToString() << "\n";
    
    /* Open LEVELDB database */
    const std::string PATH = "/btc/478558_Z_backup";
	
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
    
    return 0;
}
