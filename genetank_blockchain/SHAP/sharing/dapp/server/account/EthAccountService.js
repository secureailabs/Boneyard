//Etherium Account Service. All the web3 calls go here...
import { Meteor } from "meteor/meteor";
import { getUserCfg } from "./account";
const crypto = Npm.require("crypto");

var Future = Npm.require("fibers/future");

const ethAccountDb = new Mongo.Collection("EthereumAccounts");

const ethUtil = require("ethereumjs-util");

export function importRawKey(private_key, ethPassword) {
  var Future = Npm.require("fibers/future");
  var accFut = new Future();

  web3.eth.personal
    .importRawKey(private_key, ethPassword)
    .then(acc => {
      accFut.return(acc);
    })
    .catch(error => {
      accFut.return(null);
    });

  var newAccount = accFut.wait();
  return newAccount;
}

export function myImportRawKey(private_key, password) {
  const privKeyBuffer = Buffer.from(private_key, "hex");
  const ethAddress = ethUtil
    .privateToAddress(privKeyBuffer)
    .toString("hex")
    .toLowerCase();
  const hash = crypto.createHash("sha256");
  hash.update(password);
  let key = hash.digest();

  const hashIV = crypto.createHash("md5");
  hashIV.update(ethAddress);
  var IV = hashIV.digest();

  let encryptor = crypto.createCipheriv("aes256", key, IV);
  let encrPrivKey = encryptor.update(
    privKeyBuffer.toString("hex"),
    "utf8",
    "hex"
  );
  encrPrivKey += encryptor.final("hex");

  ethAccountDb.insert({ ethAddress: ethAddress, key: encrPrivKey });
}

export function createAccount(password) {
  let Future = Npm.require("fibers/future");
  let accFut = new Future();

  web3.eth.personal
    .newAccount(password)
    .then(result => {
      accFut.return(result);
    })
    .catch(error => {
      accFut.return(null);
    });

  let newAccount = accFut.wait();
  if (null === newAccount) 
    newAccount = myNewAccount(password);
  return newAccount;
}

function myNewAccount(password) {
  const ethECInstance = crypto.createECDH("secp256k1");
  const pubKeyBuffer = ethECInstance.generateKeys();
  const privKeyBuffer = ethECInstance.getPrivateKey();

  const tmpbuf = pubKeyBuffer.slice(1);

  const ethAddress = ethUtil
    .pubToAddress(tmpbuf)
    .toString("hex")
    .toLowerCase();

  const hash = crypto.createHash("sha256");
  hash.update(password);
  let key = hash.digest();

  const hashIV = crypto.createHash("md5");
  hashIV.update(ethAddress);
  var IV = hashIV.digest();

  let encryptor = crypto.createCipheriv("aes256", key, IV);
  let encrPrivKey = encryptor.update(
    privKeyBuffer.toString("hex"),
    "utf8",
    "hex"
  );
  encrPrivKey += encryptor.final("hex");

  ethAccountDb.insert({
    ethAddress: ethAddress,
    key: encrPrivKey
  });
  return ethAddress;
}

function getDappEthSign(ethereumAddress, ethPassword, message) {
  ethereumAddress = ethereumAddress.toLowerCase();
  if (ethereumAddress.substr(0, 2) === "0x")
    ethereumAddress = ethereumAddress.substr(2);
  const ethAccount = ethAccountDb.findOne({
    ethAddress: ethereumAddress
  });

  const hash = crypto.createHash("sha256");
  hash.update(ethPassword);
  let key = hash.digest();

  const hashIV = crypto.createHash("md5");
  hashIV.update(ethereumAddress);
  var IV = hashIV.digest();

  var decipher = crypto.createDecipheriv("aes256", key, IV);
  var decrPrivKey = decipher.update(ethAccount.key, "hex");
  decrPrivKey += decipher.final();

  const messageHash = ethUtil.keccak256(
    "\x19Ethereum Signed Message:\n" + message.length + message
  );

  const ethSign = ethUtil.ecsign(messageHash, Buffer.from(decrPrivKey, "hex"));
  return ( "0x"+
    ethSign.r.toString("hex") +
    ethSign.s.toString("hex") +
    ethSign.v.toString(16)
  );
}

function getWeb3EthSign(ethereumAddress, ethPassword, message) {
  var fut = new Future();
  web3.eth.personal
    .unlockAccount(ethereumAddress, ethPassword, 500)
    .then(response => {
      //console.log(response);
      web3.eth
        .sign(message.toString(), ethereumAddress)
        .then(function(receipt) {
          //console.log(receipt);
          fut.return(receipt);
          web3.eth.personal
            .lockAccount(ethereumAddress)
            .then(response => {
              //console.log(response);
            })
            .catch(error => {
              console.log(error);
            });
        })
        .catch(error => {
          console.log(error);
          fut.return(null);
        });
    })
    .catch(error => {
      console.log(error);
      fut.return(null);
    });

  const ethSign = fut.wait();

  return ethSign;
}

export function getEthSign(ethereumAddress, ethPassword, message) {
  var ethSign = getWeb3EthSign(ethereumAddress, ethPassword, message);
  if (null === ethSign)
    ethSign = getDappEthSign(ethereumAddress, ethPassword, message);
  return ethSign;
}

export function _getPassword(ethAccount, uid) {
  if (undefined === uid) return { error: "Login required." };
  var cfg = getUserCfg(uid);
  if (undefined === cfg) return { error: "Can't find user configuration." };

  var ethAccountIdx;
  for (ethAccountIdx in cfg.blockchainAccounts) {
    if (cfg.blockchainAccounts[ethAccountIdx].account === ethAccount) {
      var encryPassword = cfg.blockchainAccounts[ethAccountIdx].password;

      const hash = crypto.createHash("sha256");
      hash.update(uid);
      var key = hash.digest();
      const hashIV = crypto.createHash("md5");
      hashIV.update(ethAccount);
      var IV = hashIV.digest();

      var decipher = crypto.createDecipheriv("aes256", key, IV);
      var decrPassword = decipher.update(encryPassword, "hex");
      decrPassword += decipher.final();

      return { password: decrPassword };
    }
  }
  return { error: "Account not found" };
}

Meteor.methods({
  version() {
    var version = web3.version;
    return { version: version };
  },

  userGetEthInfo(username) {
    if (undefined === this.userId) return { error: "Login required." };
    var user = Meteor.users.findOne({ _id: this.userId });
    if (!user) return { error: "Can't get user record." };
    var uId = this.userId;
    if (
      undefined !== username &&
      "" !== username &&
      user.username !== username
    ) {
      if (user.username !== "admin")
        return (error: "Only Admin can get other account's Ethereum account information.");
      user = Meteor.users.findOne({ username: username });
      if (!user) return { error: "User not found." };
      uId = user._id;
    }
    var cfg = getUserCfg(uId);
    if (undefined === cfg) return { error: "Can't find user configuration." };
    return {
      blockchainAccounts: cfg.blockchainAccounts,
      defaultEthAccount: cfg.defaultEthAccount
    };
  },

  getBalance(ethAccount) {
    const getBalanceAsync = Meteor.wrapAsync(web3.eth.getBalance);
    const balance = getBalanceAsync(ethAccount);

    const balanceEther = web3.utils.fromWei(balance);
    return { balance: balanceEther };
  },

  // use this function, when user id is not present
  getPasswordFromUserId(ethAccount, userId) {
    //NOT TO BE EXPOSED TO ANYONE
    if (undefined === userId) return { error: "Login required." };
    return _getPassword(ethAccount, userId);
  },

  getPassword(ethAccount) {
    //NOT TO BE EXPOSED TO ANYONE
    if (undefined === this.userId) return { error: "Login required." };
    return _getPassword(ethAccount, this.userId);
  },

  getHash(data) {
    console.log("data", data);
    var hashData = web3.utils.sha3(data);
    return { hashData: hashData };
  },

  seedEther(acc) {
    let Future = Npm.require("fibers/future");
    let seedFut = new Future();
    web3.eth.personal
      .unlockAccount(
        "0x8ddb6634a199c9c9076fa1fddd2c0509a9c9d3fc",
        "gtai1234",
        200
      )
      .then(response => {
        web3.eth
          .sendTransaction({
            from: "0x8ddb6634a199c9c9076fa1fddd2c0509a9c9d3fc",
            to: acc,
            value: 1000000000000000000
          })
          .then(function(receipt) {
            console.log(receipt);
            seedFut.return(null);
            web3.eth.personal
              .lockAccount("0x8ddb6634a199c9c9076fa1fddd2c0509a9c9d3fc")
              .then(response => {
                console.log(response);
              })
              .catch(error => {
                console.log(error);
              });
          });
      })
      .catch(error => {
        console.log(error);
        seedFut.return(null);
      });
    seedFut.wait();
    return null;
  },

  transferEther(fromAcc, toAcc, amount) {
    var Future = Npm.require("fibers/future");
    var passwordFut = new Future();

    Meteor.call("getPassword", fromAcc, (error, result) => {
      if (!error) {
        passwordFut.return(result.password);
      } else {
        console.log(error);
        passwordFut.return(null);
      }
    });

    const password = passwordFut.wait();
    //console.log(password);

    web3.eth.personal
      .unlockAccount(fromAcc, password, 200)
      .then(response => {
        console.log("unlocking", response);
        web3.eth
          .sendTransaction({
            from: fromAcc,
            to: toAcc,
            value: amount
          })
          .then(function(receipt) {
            console.log(receipt);
            web3.eth.personal
              .lockAccount(fromAcc)
              .then(response => {
                console.log(response);
              })
              .catch(error => {
                console.log("locking", error);
              });
          });
      })
      .catch(error => {
        console.log(error);
      });
  }
});
