# Bitcoin Zero
Bitcoin Zero is forked from Zcash. Here is an detailed introduction of Bitcoin Zero:
https://github.com/bitcoinzero/doc/blob/master/bitcoinzero.pdf
# How To Start
Quite like Zcash, clone it, run ./zcutil/build.sh, then it will be built. If you already have fetched Zcash parameter files, you do NOT need to do it again. Otherwise, run ./zcutil/fetch-params.sh. Basically, https://github.com/zcash/zcash/wiki/1.0-User-Guide is a good guide for Bitcoin Zero too. The only difference is, like stated on that detaild introduction, instead of synchronizing the whole Bitcoin blockchain, a genesis block needs to be downloaded. You can find one at https://github.com/bitcoinzero/genesis. ~/.bitcoinzero is the default data file path. For example, once you put a corresponding blk00000.dat under ~/.bitcoinzero/testnet3/blocks, you can then run the zcashd on testnet with an additional flag -reindex. Like this ./src/zcashd -testnet -reindex. You do NOT need to run with -reindex everytime, but you HAVE TO RUN WITH IT AT THE FIRST TIME. If you missed it at the first time, blk00000.dat will be modified and you will need to delete the folder testnet3 and then try it again with an unchanged blk00000.dat. I am sorry for the trouble and I am working on it. If you have a better idea, please.
# How To Mine
Bitcoinzero uses and only uses _merged mining_ for PoW. 
## classic merged mining
Bitcoinzero fully supports classic merged mining method stated on https://en.bitcoin.it/wiki/Merged_mining_specification. That means, it provided the same RPC, namely, *getauxblock* with the same usage: 
### call *getauxblock* without any parameters 
Return merged mining infomation including the hash of the block waiting for PoW.
### call *getauxblock* with two parameters
Send the hash of the block waiting for PoW and the serialized classic *CAuxPow* object will submit this block with the PoW.
## extra feature
Bitcoinzero also supports fractal aux work merkle tree. A detailed introduction is upcoming, maybe. The basic idea is simple. We can extend a slot to be the root for a subtree. In bitcoinzero, a class called *CAuxPowSupplement* is defined to represent a subtree, actually, a sub-branch links the AuxPOW block to the subtree root. The slot of the AuxPOW block in the subtree is calculated in the classic way, with a subchain id. Then, corresponding merkle_size and merkle_nonce are appended to that root, no magic bytes needed. Next, hash that subtree root with the merkle_size and merkle_nonce to get a *protected subtree root*. Finally, the classic *CAuxPow* will do the rest, namely, link the *protected subtree root* to the coinbase of the parent block. Of course, we can also have subtrees of subtrees.
### call *getauxblock* with one parameter
Send the maximum number of nested subtrees allowed. Return corresponding merged mining infomation.
### call *getauxblock* with three parameters
Send the hash of the block waiting for PoW and a serialized classic *CAuxPow* object and a serialized *vector of CAuxPowSupplement* will submit this block with the PoW utilizing a fractal aux work merkle tree.
### call *getauxblock* with six parameters
This is mainly for testing. The classic *CAuxPow* object includes some unneeded parts. We can break it down and only send wanted parts to form the PoW. This will submit the block with the PoW.
# How To Redeem
Bitcoin Zero takes the snapshot of Bitcoin chainstate at height 478558. As a result, people who have BTC at Bitcoin 478558 have the same amout of BCZ now. This section introduces how can we redeem that part of BCZ.
## step 1 : call RPC *z_getnewaddress*
This will give you a new shielded address where informations like balance are shielded from other people. You are going to redeem those BCZ to this address.
### note:
In Zcash, coinbase can only be sent to a shielded address in order to increase privacy for all participants on a systemic level. For the same reason, in Bitcoin Zero, coinbase and BCZ from the snapshot can only be sent to a shielded address.
## step 2 : call RPC *importprivkey*
Import your private keys related to your BTC at height 478558. It will return corresponding addresses. You can check the balance with RPC *listunspent* afterwards.
### note:
The snapshot and the Bitcoin chainstate at height 478558 use the same pubkeys. So the same private keys will give us access to corresponding BCZ. Details of this RPC can be found here https://en.bitcoin.it/wiki/Original_Bitcoin_client/API_calls_list.
## step 3 : call RPC *z_shieldcoinbase*
*z_shieldcoinbase* takes two parameter. First, the address you get from *importprivkey*. Second, the address you get from *z_getnewaddress*. That's it. This will send BCZ from the snapshot to your new shielded address. After waiting about a minute, you can check the transaction by call RPC *z_getoperationresult*.
### note: 
Details of this RPC can be found here https://github.com/zcash/zcash/blob/master/doc/payment-api.md.
