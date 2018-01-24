// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "main.h"

#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

#include "base58.h"

using namespace std;

#include "chainparamsseeds.h"

/**
 * Main network
 */
/**
 * What makes a good checkpoint block?
 * + Is surrounded by blocks with reasonable timestamps
 *   (no blocks before with a timestamp after, none after with
 *    timestamp before)
 * + Contains no strange transactions
 */

const arith_uint256 maxUint = UintToArith256(uint256S("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"));

class CMainParams : public CChainParams {
public:
    CMainParams() {
        strNetworkID = "main";
        strCurrencyUnits = "BTZ";
        consensus.fCoinbaseMustBeProtected = true;
        consensus.nSubsidyHalvingInterval = 840000;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 4000;
        consensus.powLimit = uint256S("0000000000ffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        consensus.nPowAveragingWindow = 17;
        assert(maxUint/UintToArith256(consensus.powLimit) >= consensus.nPowAveragingWindow);
        consensus.nPowMaxAdjustDown = 32; // 32% adjustment down
        consensus.nPowMaxAdjustUp = 16; // 16% adjustment up
        consensus.nPowTargetSpacing = 2.5 * 60;        
        /**
         * The message start string should be awesome! ⓩ❤
         */
        pchMessageStart[0] = 0x1c;
        pchMessageStart[1] = 0xa7;
        pchMessageStart[2] = 0xfe;
        pchMessageStart[3] = 0xdd;
        vAlertPubKey = ParseHex("04b7ecf0baa90495ceb4e4090f6b2fd37eec1e9c85fac68a487f3ce11589692e4a317479316ee814e066638e1db54e37a10689b70286e6315b1087b6615d179264");
        nDefaultPort = 8083;
        nMaxTipAge = 24 * 60 * 60;
        nPruneAfterHeight = 100000;
		
        consensus.nAuxpowChainId = 0x0abc;
        consensus.nSubAuxpowChainId = 478558;
		/**
         * Build the genesis block. Note that the output of its generation
         * transaction cannot be spent since it did not originally exist in the
         * database (and is in any case of zero value).
         *
            CBlock genesis;
            const char* pszTimestamp = "WSJ 24/Jan/2018 After Bitcoin Futures, Watch Out for Crypto Repos";
            
            CMutableTransaction txNew;
            txNew.vin.resize(1);
            txNew.vout.resize(1+16529043);
            txNew.vin[0].scriptSig = CScript() << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));        
         */

        consensus.hashGenesisBlock.SetHex("32d3e76ed79d617db25ff529e44370d4787b3de3499536fac9c05a1215a1c7a7");
        
        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("seed1.bitcoinzero.io", "seed1.bitcoinzero.io"));
        vSeeds.push_back(CDNSSeedData("seed2.bitcoinzero.io", "seed2.bitcoinzero.io"));
        vSeeds.push_back(CDNSSeedData("seed3.bitcoinzero.io", "seed3.bitcoinzero.io"));
        vSeeds.push_back(CDNSSeedData("seed4.bitcoinzero.io", "seed4.bitcoinzero.io"));
        vSeeds.push_back(CDNSSeedData("seed5.bitcoinzero.io", "seed5.bitcoinzero.io"));
        vSeeds.push_back(CDNSSeedData("seed6.bitcoinzero.io", "seed6.bitcoinzero.io"));
        vSeeds.push_back(CDNSSeedData("seed7.bitcoinzero.io", "seed7.bitcoinzero.io"));
        vSeeds.push_back(CDNSSeedData("seed8.bitcoinzero.io", "seed8.bitcoinzero.io"));
        vSeeds.push_back(CDNSSeedData("seed9.bitcoinzero.io", "seed9.bitcoinzero.io"));
        vSeeds.push_back(CDNSSeedData("seed10.bitcoinzero.io", "seed10.bitcoinzero.io"));
        
        base58Prefixes[PUBKEY_ADDRESS]     = std::vector<unsigned char>(1,0);
        base58Prefixes[SCRIPT_ADDRESS]     = std::vector<unsigned char>(1,5);
        base58Prefixes[SECRET_KEY]         = std::vector<unsigned char>(1,128);
        base58Prefixes[EXT_PUBLIC_KEY]     = boost::assign::list_of(0x04)(0x88)(0xB2)(0x1E).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY]     = boost::assign::list_of(0x04)(0x88)(0xAD)(0xE4).convert_to_container<std::vector<unsigned char> >();
        // guarantees the first character, when base58 encoded, is "z"
        base58Prefixes[ZCPAYMENT_ADDRRESS] = std::vector<unsigned char>(1,99);
        // guarantees the first 4 characters, when base58 encoded, are "ZiVK"
        base58Prefixes[ZCVIEWING_KEY]      = {0xA8,0xAB,0xD3};
        // guarantees the first character, when base58 encoded, is "S"
        base58Prefixes[ZCSPENDING_KEY]     = std::vector<unsigned char>(1,13);
        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = false;

                // Founders reward script expects a vector of 2-of-3 multisig addresses
        vFoundersRewardAddress = {
            "3D7R2atx1hZ1iban4fjnvS9sPa8wDnUJny", // "t3Vz22vK5z2LcKEdg16Yv4FFneEL1zg9ojd", /* main-index: 0* /a9147d46a730d31f97b1930d3368a967c309bd4d136a87
            "3KTYAaC4cQySgtAoeev4eVD7fvdR5R6ikq", // "t3cL9AucCajm3HXDhb5jBnJK2vapVoXsop3", /* main-index: 1* /a914c2e5af6fa0864ceb5ab93d88813772cebe04c62087
            "3NyKkfSiQFa1z8hWRWMr9cdjxy9xEBPdfY", // "t3fqvkzrrNaMcamkQMwAyHRjfDdM2xQvDTR", /* main-index: 2* /a914e9702e27abc52e441fd979544ecab6b20d045c3287
            "3Aox9E2tE8eiTR7tXxtyhqGf2uqZRmXUCT", // "t3TgZ9ZT2CTSK44AnUPi6qeNaHa2eC7pUyF", /* main-index: 3* /a9146408e3247aa526d95f5033e9fd3840e1b40a8bff87
            "39x9c3zFhadwhELBzZEGnJzURfc11kdrVQ", // "t3SpkcPQPfuRYHsP5vz3Pv86PgKo5m9KVmx", /* main-index: 4* /a9145a9da85a1d09ce65c1ac4d3c161ffb5dcad03ba087
            "3F1To4wHRFuM1BMwpFMNabayBnsQzBmHzH", // "t3Xt4oQMRPagwbpQqkgAViQgtST4VoSWR6S", /* main-index: 5* /a9149216b7f7c14f101778b2c2cbe24a529adf6bd60887
            "3J6akDeo8RXwP9tuM8SMJdacAfhiooME3L", // "t3ayBkZ4w6kKXynwoHZFUSSgXRKtogTXNgb", /* main-index: 6* /a914b3f6fa632d7303d689678700b0502aa1a927296b87
            "3HkhB5VSbhEJWjuhUhjErzwqWPGWeYn47d", // "t3adJBQuaa21u7NxbR8YMzp3km3TbSZ4MGB", /* main-index: 7* /a914b0340671addc94974003b231a450453382e31ada87
            "32ByLDAYU7QNqzomDqTXLoyMuKtLBs95qs", // "t3K4aLYagSSBySdrfAGGeUd5H9z5Qvz88t2", /* main-index: 8* /a9140579e6348f398c5e78611da902ca457885cda23987
            "38gBsGfeiv8j8HX9cxtYHdeCQKVvPQnf7Q", // "t3RYnsc5nhEvKiva3ZPhfRSk7eyh1CrA6Rk", /* main-index: 9* /a9144ca0c3d712bf1ea99f87a2d36482699ef1d392d187
            "3C1TK9Qtb7ZrnjBCBPNLwXjcx3qkHqqPtY", // "t3Ut4KUq2ZSMTPNE67pBU5LqYCi2q36KpXQ", /* main-index: 10* /a914712df07c5176f8438e0cfdde89f38fffb903b63387
            "3GubMqWYvmQrNei7z65jpEUo7UwYdy4FB2", // "t3ZnCNAvgu6CSyHm1vWtrx3aiN98dSAGpnD", /* main-index: 11* /a914a6eaca126b366ff3db16b66a90d313bd8c38df6a87
            "3NJYbqdWUDwAVR8YDXrp2UJZ4gVKdHrptb", // "t3fB9cB3eSYim64BS9xfwAHQUKLgQQroBDG", /* main-index: 12* /a914e21aaec105793af8f35a0e2ae9b0630407f47faa87
            "3L4xeyxEki8vkXEHUDqpdHicSf4YrzQk9S", // "t3cwZfKNNj2vXMAHBQeewm6pXhKFdhk18kD", /* main-index: 13* /a914c998afe1705849c9f8892249be32c03497aefdce87
            "3FkCuQ7XuVjNXDYUYSTCpS8bKBBoAGgDNa", // "t3YcoujXfspWy7rbNUsGKxFEWZqNstGpeG4", /* main-index: 14* /a9149a2c2cbabfe310f09ae4d86d51a78f85573d5b6a87
            "3JUKC1JYdn4znDRZVeZpcssabuRV81KrYu", // "t3bLvCLigc6rbNrUTS5NwkgyVrZcZumTRa4", /* main-index: 15* /a914b81324ed42304e59551e235ebab3a989bdb660bf87
            "3D3gWEhi5VBVWuqa7ukC8PQemduZTmv9gW", // "t3VvHWa7r3oy67YtU4LZKGCWa2J6eGHvShi", /* main-index: 16* /a9147c91e899227cb5c6a608b982e0a2d7db86901d0687
            "3MNYWm6tf81WkZsZoEk7s8QwFLeq6j3gVC", // "t3eF9X6X2dSo7MCvTjfZEzwWrVzquxRLNeY", /* main-index: 17* /a914d7e425c9d068408dda1d3a37caf2bc055f6104f987
            "3MzbNcXdeepY7WnWjYeUQtMvWtLUMwK8Ht", // "t3esCNwwmcyc8i9qQfyTbYhTqmYXZ9AwK3X", /* main-index: 18* /a914deb5825db797ce6a81b41b83abb047f415af241287
            "34C8MnHQFhrRXLHyxyaGc6PjVTpRi4aDAW", // "t3M4jN7hYE2e27yLsuQPPjuVek81WV3VbBj", /* main-index: 19* /a9141b71afad5f3185b75457bdab3fabc297b56694e287
            "3PPuxHmx8sknCYYVnx7jihEFGJCkmTqaMf", // "t3gGWxdC67CYNoBbPjNvrrWLAWxPqZLxrVY", /* main-index: 20* /a914ee16b664f7bca502cad662e5e36bad071423951987
            "33aueUYWY4pB37RK6wh4i6e9G6PuzJxsmK", // "t3LTWeoxeWPbmdkUD3NWBquk4WkazhFBmvU", /* main-index: 21* /a91414c884180589f6d0fdc7c400181afb1bb10c9fe487
            "36CiKBiyiCkeqwPqMxujmb8bsQnAMUEatG", // "t3P5KKX97gXYFSaSjJPiruQEX84yF5z3Tjq", /* main-index: 22* /a914317e483f60b1b9029b232b206aada27c68735d2187
            "3NAr3SnNtv3QAZzBYtGuAsJk14V2JmXX6B", // "t3f3T3nCWsEpzmD35VK62JgQfFig74dV8C9", /* main-index: 23* /a914e0a5ea1340cc6b1d6a82c06c0a9c60b9898b6ae987
            "38yCnaar9Ft9eTxBA8LwntxoU7EMVGMB61", // "t3Rqonuzz7afkF7156ZA4vi4iimRSEn41hj", /* main-index: 24* /a9144fd89a8512e083965f79d92eb1019f850acee5ec87
            "3NRx5Q8k1dSJKjocFDzEU7Cf9YXyXB7Hev", // "t3fJZ5jYsyxDtvNrWBeoMbvJaQCj4JJgbgX", /* main-index: 25* /a914e38130c1e0add9fb67365f61c6978f2c1891a5d387
            "36uzfn7bQnTfnYS23gG9xGUhkAWCyWJdXn", // "t3Pnbg7XjP7FGPBUuz75H65aczphHgkpoJW", /* main-index: 26* /a914394cfd39873191560f79a1ec8d62e001d9f364bc87
            "3DmiPtY4kQYUvUp3KDwtDeEEKHJDyD2vUU", // "t3WeKQDxCijL5X7rwFem1MTL9ZwVJkUFhpF", /* main-index: 27* /a9148484f50714d67b89f5305ef037ea7bc7d1e002ff87
            "3FGeNNbxKnhHZq9AqNm7KXVSYU8M9t9vEp", // "t3Y9FNi26J7UtAUC4moaETLbMo8KS1Be6ME", /* main-index: 28* /a91494f5c8f356a24300ed4d822bee76a7bc19551c9c87
            "3HVpL1TC4eMN26Loi8kp89wUNFv854vd6g", // "t3aNRLLsL2y8xcjPheZZwFy3Pcv7CsTwBec", /* main-index: 29* /a914ad63c1f0c6ad2b5227261a191dc34852f8411a0387
            "3PXcEFWc7BCZZeGEXRbmtNQJ6J3dPf7CXf", // "t3gQDEavk5VzAAHK8TrQu2BWDLxEiF1unBm", /* main-index: 30* /a914ef8b3e86db855eb48bcf0b7585a90b6b9ece75c087
            "38jNkNXsV9UG5tosF833ApvXDJEESZqM3q", // "t3Rbykhx1TUFrgXrmBYrAJe2STxRKFL7G9r", /* main-index: 31* /a9144d3b188f065e03b85d4fefcc7f4ede7bf0c271b787
            "3Hhu4F3VQnnY2NSZHaNgVDsnLzVbUwfFhS", // "t3aaW4aTdP7a8d1VTE1Bod2yhbeggHgMajR", /* main-index: 32* /a914afacb22d73c1f6da72fb06c3a9fdbf5d8ec42d8187
            "3FN7AEgmGQkLegz29a5Lsrg8ifVucmRTrG", // "t3YEiAa6uEjXwFL2v5ztU1fn3yKgzMQqNyo", /* main-index: 33* /a91495fe3a8328f7ea678710aca7496b8908a2c5860987
            "3P9NU9Xk44pAdHJKiVe5FhNhLZPoYCYvR3", // "t3g1yUUwt2PbmDvMDevTCPWUcbDatL2iQGP", /* main-index: 34* /a914eb569b42848d5ebcefd047215a0fd446aa5725b787
            "3LWunKPxaWUoJtxJiBsUWkkcu4HrRF67vW", // "t3dPWnep6YqGPuY1CecgbeZrY9iUwH8Yd4z", /* main-index: 35* /a914ce811a7457b9f4553af120e553bc8ddd8bb33ce987
            "37YxWwoFihvLsR3pUSCyyHeWfFKbC1v8Ms", // "t3QRZXHDPh2hwU46iQs2776kRuuWfwFp4dV", /* main-index: 36* /a914404a954619f431f1f4c6c1e8156caa13c5d3d03c87
            "3Mv69s1pjgmcX15kTNxN8WgCh9vHPm1qos", // "t3enhACRxi1ZD7e8ePomVGKn7wp7N9fFJ3r", /* main-index: 37* /a914dddb7433536ad23ef1495d3398da9c70e65935c187
            "36sjg7gsVTTQQPjYzkegQ47BsKBX8Du2sS", // "t3PkLgT71TnF112nSwBToXsD77yNbx2gJJY", /* main-index: 38* /a91438dfa4066f0032c86024bb5899adfa167c80189c87
            "33YHH8offnn77HajVVFoemhSv31mAheAnm", // "t3LQtHUDoe7ZhhvddRv4vnaoNAhCr2f4oFN", /* main-index: 39* /a9141449535d5cb388b318343ca7a7fb1764a69f0ecd87
            "3NW1cr4U1J8zcFpK6MLhuXp31TYQZ7Ez5m", // "t3fNcdBUbycvbCtsD2n9q3LuxG7jVPvFB8L", /* main-index: 40* /a914e445cfa944b6f2bdacefbda904a81d5fdd26d77f87
            "3LTCj93tG2xGRVjPYhFLnwPKDZhvYcB1P8", // "t3dKojUU2EMjs28nHV84TvkVEUDu1M1FaEx", /* main-index: 41* /a914cdcd95aa6892db5fc849ac15804d2b9dd035a3b787
            "3HSg63JNPh24fz5i4j3Sb2ada66xxymJds", // "t3aKH6NiWN1ofGd8c19rZiqgYpkJ3n679ME", /* main-index: 42* /a914accba16ec3b9fa60ca3143419c81e9fc4217d05a87
            "34MvCujNuPJSjJmVxyp5xns7mhjwkJ4s4E", // "t3MEXDF9Wsi63KwpPuQdD6by32Mw2bNTbEa", /* main-index: 43* /a9141d4bb379ddd4043ae86c8b96e7183784f9eddf4987
            "3DM6PLJc4jGNn8LZuTwS2zJeQAy3A483ho", // "t3WDhPfik343yNmPTqtkZAoQZeqA83K7Y3f", /* main-index: 44* /a9147fdcea2e77715232b15ab63e49fc30076ce80ffc87
            "36aB58BDNqTLWbr99njVkSZ5BLdSDJ4aAv", // "t3PSn5TbMMAEw7Eu36DYctFezRzpX1hzf3M", /* main-index: 45* /a914358d5a7f5e2a687eeba360bb473f3c38890674f387
            "38Aw5bN3NXTBXh43KJac3Xh3BpDnZmztnb", // "t3R3Y5vnBLrEn8L6wFjPjBLnxSUQsKnmFpv", /* main-index: 46* /a91447185bab99466b849d774faa2edfa983bd37741487
            "36kA6hh6uAxfrxeyxbZXcWnPPbwwyBb2KA", // "t3Pcm737EsVkGTbhsu2NekKtJeG92mvYyoN", /* main-index: 47* /a9143770b379a58159d907f6b83868ca65e1a450753a87
//            "t3PZ9PPcLzgL57XRSG5ND4WNBC9UTFb8DXv", /* main-index: 48*/
//            "t3L1WgcyQ95vtpSgjHfgANHyVYvffJZ9iGb", /* main-index: 49*/
//            "t3JtoXqsv3FuS7SznYCd5pZJGU9di15mdd7", /* main-index: 50*/
//            "t3hLJHrHs3ytDgExxr1mD8DYSrk1TowGV25", /* main-index: 51*/
//            "t3fmYHU2DnVaQgPhDs6TMFVmyC3qbWEWgXN", /* main-index: 52*/
//            "t3T4WmAp6nrLkJ24iPpGeCe1fSWTPv47ASG", /* main-index: 53*/
//            "t3fP6GrDM4QVwdjFhmCxGNbe7jXXXSDQ5dv", /* main-index: 54*/
};
        assert(vFoundersRewardAddress.size() <= consensus.GetLastFoundersRewardBlockHeight());    
	}	
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
        strNetworkID = "test";
        strCurrencyUnits = "TAZ";
        consensus.nMajorityEnforceBlockUpgrade = 51;
        consensus.nMajorityRejectBlockOutdated = 75;
        consensus.nMajorityWindow = 400;
        consensus.powLimit = uint256S("0000000000ffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        pchMessageStart[0] = 0xfb;
        pchMessageStart[1] = 0x2a;
        pchMessageStart[2] = 0x3d;
        pchMessageStart[3] = 0x4a;
        vAlertPubKey = ParseHex("044e7a1553392325c871c5ace5d6ad73501c66f4c185d6b0453cf45dec5a1322e705c672ac1a27ef7cdaf588c10effdf50ed5f95f85f2f54a5f6159fca394ed0c6");
        nDefaultPort = 18083;
        nPruneAfterHeight = 1000;

        //! Modify the testnet genesis block so the timestamp is valid for a later start.
        consensus.hashGenesisBlock = uint256S("59058d8828acad88163f9bc444e232fea2c9e1cfcc2b6066c668e12b4dbbe39d");

        vFixedSeeds.clear();
        vSeeds.clear();
        vSeeds.push_back(CDNSSeedData("z.cash", "dnsseed.testnet.z.cash")); // Zcash

        base58Prefixes[PUBKEY_ADDRESS]     = std::vector<unsigned char>(1,111);
        base58Prefixes[SCRIPT_ADDRESS]     = std::vector<unsigned char>(1,196);
        base58Prefixes[SECRET_KEY]         = std::vector<unsigned char>(1,239);
        base58Prefixes[EXT_PUBLIC_KEY]     = {0x04, 0x35, 0x87, 0xCF};
        base58Prefixes[EXT_SECRET_KEY]     = {0x04, 0x35, 0x83, 0x94};

        base58Prefixes[ZCPAYMENT_ADDRRESS] = std::vector<unsigned char>(1,49);
        // guarantees the first 4 characters, when base58 encoded, are "ZiVt"
        base58Prefixes[ZCVIEWING_KEY]      = {0xA8,0xAC,0x0C};
        base58Prefixes[ZCSPENDING_KEY]     = std::vector<unsigned char>(1,6);

        vFixedSeeds = std::vector<SeedSpec6>(pnSeed6_test, pnSeed6_test + ARRAYLEN(pnSeed6_test));

        fMiningRequiresPeers = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;
        
        //these addresses' underlying pubkeyScripts are the same as Zcash testnet founder reward addresses'
        vFoundersRewardAddress = {
            "2NF5QVDMtYBHaUzPLTxirybjwPjm9wVwacv", "2N8qoHsvVvdrDc89dXjpBWZHMbWdaHVc3rg", "2N8xpkGmATvu1GjgpjSjmt5L5V5fnEhW89v", "2N1568RhFcWyXNPTzReYdP7fVJEN4fzNqTu",
            "2MxSxeDc3mbZgMvTrswMt5MMMXQvL1MyjFR", "2N4qF2ggm1aq4MX2GUCmgvFFbDFHc94fpvZ", "2MyZFAWPPES9Vp1of13vzvxogWBqoBWsM9C", "2N1GmQD89nS15u4NzHCYwzXts6X6kgwjSUY", 
            "2N1r3u91qoyKc3JhhUnodS4A1U3s2X3LKX7", "2N768sWCRK7fz7a1BVWzH3UAyU1bqnNCBcf", "2Msf5yXrgo9bXT65hr9wC14gJFQrMPe42MK", "2MykA5PnP1XjU9zxojV3LcPrfkYbEPqeKkT", 
            "2MzCxCVN59Q31qKMdXtB1wRbHFUdgwFNb7a", "2MxkAFyZUB753fWB4oeQDjaL7BYdv2NRJcA", "2N8Us6dQeuxYhgGpLnSBejyWqCghZyzABTq", "2Mw7etF3ZGgMWHrYnFagShCYWSyUmyk9Fzt", 
            "2NGMUx423pENGQB4eTxHBGGeFGLp6QNweiD", "2N5k5eE82s6CGVMXFdf8gk4EgQumRPtajgz", "2NFwC4485jxYf3ScxbBjoAxDbRQCD3cYLAf", "2N1pwpeFP92wV4SEqmNKzRXASnmDD5aG7GS", 
            "2Mx8XNb1LRLA3DWxmaiivfNxCNheLJh2Tmu", "2N2brxXjxPrbn15ZpMm6ZBWesSMdtAfwHcx", "2NDDTUrned8D53XDVp8h78wySi2coz7CnpX", "2NFmRhHnCZyC8Un6yQcxFQQJGm7vAiwGLNB", 
            "2N2fst59VQSB7hwwdf2mhfcwsbgWYimBxeK", "2N1F1H54WYVwg3E8NzCuTnLHQhkZ2PMY5Em", "2N96ssBJK4ZQcFpFMrbGtH3rr1SesT8HDdY", "2N38vaHikaKyw27PNKVYxibBHowWTzKRURx", 
            "2MztcR7uNj9JpVrojSLecdQw65CrCzr5kq3", "2N5h9p9FYgCzqKtN2Z5nT2GMBGP4qgUUisn", "2Mvw4gLWnVKaAvyKgVGqJVtKTa9iUiWdGtg", "2N95H4HgK2YV76PvgJavtPT9NmSFbsMouCz", 
            "2MztcR7uNj9JpVrojSLecdQw65CrCzr5kq3", "2N5H1EDA1u8hDYmYaeB6koMTCUV7jK7kstc", "2N1yQWoyPNoKMCaZEaPnPozyQx17XsAuDwm", "2MzAd9JRm2ZLdqW2iJJcnJnJWXjnxyaB7xd", 
            "2NFRuTMTkUKPmjckpL6K7SXuR3MkCYQWSNH", "2NDstBRe6jxbACA6MLWgEvb3UpKY92xzKuM", "2NEgTuwPT9mm73vW8R1PyDdEB3M4KyNVLJg", "2N9gHKAye7cTiZgJ1AEqtpU8Pwtp2owG1ya", 
            "2N6DiWEPjdQcM1teVoZLrhNygX5rm6X81pn", "2N4qxZcgnnJ32mnZuYY2m78ceLub1uEfkKL", "2NBPLWoH8vfgesqMBpEEhudKXpqZpVTmSrS", "2N8usK7Z7aYSD2hPC7jUSYbWy47xPABCqbj", 
            "2MvWhDuvatWUSJBwM3FUqskYFVMcfwnGVtp", "2N1gZL5YF521Yv4xp3iB87GQ5mvRErtXYiQ", "2MynP82VRbCh7Q3GmA9DTCBjU6ZvaugAHSU", "2NFBSxaR9EFb733pfJ14LP8yfKWSUjNzcLj", 
            };
        assert(vFoundersRewardAddress.size() <= consensus.GetLastFoundersRewardBlockHeight());        
	}
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
        strNetworkID = "regtest";
        strCurrencyUnits = "REG";
        consensus.fCoinbaseMustBeProtected = false;
        consensus.nSubsidyHalvingInterval = 840000;
        consensus.nMajorityEnforceBlockUpgrade = 750;
        consensus.nMajorityRejectBlockOutdated = 950;
        consensus.nMajorityWindow = 1000;
        consensus.powLimit = uint256S("7fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff");
        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xbf;
        pchMessageStart[2] = 0xb5;
        pchMessageStart[3] = 0xda;
        nMaxTipAge = 24 * 60 * 60;
        nDefaultPort = 18344;
        consensus.hashGenesisBlock.SetHex("fcde2cc4f25acaa6f6577a994f7e34a31df525c0bfc554ec294e90adb7249d70");

        vFixedSeeds.clear(); //! Regtest mode doesn't have any fixed seeds.
        vSeeds.clear();  //! Regtest mode doesn't have any DNS seeds.

        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
        vFoundersRewardAddress = { "t2FwcEhFdNXuFMv1tcYwaBJtYVtMj8b1uTg" };
        assert(vFoundersRewardAddress.size() <= consensus.GetLastFoundersRewardBlockHeight());
	}
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams &Params(CBaseChainParams::Network network) {
    switch (network) {
        case CBaseChainParams::MAIN:
            return mainParams;
        case CBaseChainParams::TESTNET:
            return testNetParams;
        case CBaseChainParams::REGTEST:
            return regTestParams;
        default:
            assert(false && "Unimplemented network");
            return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network) {
    SelectBaseParams(network);
    pCurrentParams = &Params(network);

    // Some python qa rpc tests need to enforce the coinbase consensus rule
    if (network == CBaseChainParams::REGTEST && mapArgs.count("-regtestprotectcoinbase")) {
        regTestParams.SetRegTestCoinbaseMustBeProtected();
    }
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}

// Block height must be >0 and <=last founders reward block height
// Index variable i ranges from 0 - (vFoundersRewardAddress.size()-1)
std::string CChainParams::GetFoundersRewardAddressAtHeight(int nHeight) const {
    int maxHeight = consensus.GetLastFoundersRewardBlockHeight();
    assert(nHeight > 0 && nHeight <= maxHeight);

    size_t addressChangeInterval = (maxHeight + vFoundersRewardAddress.size()) / vFoundersRewardAddress.size();
    size_t i = nHeight / addressChangeInterval;
    return vFoundersRewardAddress[i];
}

// Block height must be >0 and <=last founders reward block height
// The founders reward address is expected to be a multisig (P2SH) address
CScript CChainParams::GetFoundersRewardScriptAtHeight(int nHeight) const {
    assert(nHeight > 0 && nHeight <= consensus.GetLastFoundersRewardBlockHeight());

    CBitcoinAddress address(GetFoundersRewardAddressAtHeight(nHeight).c_str());
    assert(address.IsValid());
    assert(address.IsScript());
    CScriptID scriptID = get<CScriptID>(address.Get()); // Get() returns a boost variant
    CScript script = CScript() << OP_HASH160 << ToByteVector(scriptID) << OP_EQUAL;
    return script;
}

std::string CChainParams::GetFoundersRewardAddressAtIndex(int i) const {
    assert(i >= 0 && i < vFoundersRewardAddress.size());
    return vFoundersRewardAddress[i];
}