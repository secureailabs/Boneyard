import { Meteor } from 'meteor/meteor';
import { Mongo } from 'meteor/mongo';
import { userCfgDb, getUserCfg, homeDir } from '../account/account.js';
import { userStatus, getUserSt } from '../account/acc_status.js';
import { myContract } from "/server/smartcontract.js";

import { Random } from 'meteor/random'

import { EJSON } from 'meteor/ejson'

var fs = require('fs')

import { msgType } from '/imports/api/msg_type.js'
import {HTTP} from "meteor/http";

const crypto = Npm.require('crypto');
const hash = crypto.createHash('sha256');

Meteor.methods({

  userDelPackage(id) {
      if (undefined === this.userId)
          return ({error:"Login required."})
      //var cfg = userCfgDb.findOne({_id:this.userId})
      var cfg = getUserCfg(this.userId)
      if (undefined === cfg)
          return ({error:"Can't find user configuration."});
      var enclavePackages = cfg.enclavePackages
      var i//=models.length
      var found=false;
      for (i in enclavePackages)
          if (enclavePackages[i]._id === id){
              found = true
              break;
          }
      if (found) {
          enclavePackages.splice(i, 1)
          userCfgDb.update({_id:this.userId},{$set: {enclavePackages:enclavePackages}});
          return({result:"Delete model "+id+" success."})
      }
      return({error:"Delete model "+id+" fail.", result:"Package id not found."})
  },

  userAddModPackage(packageJson, id, src, bin) {
    if (undefined === this.userId)
      return ({error:"Login required."})
    var cfg = getUserCfg(this.userId)
    //var cfg = userCfgDb.findOne({_id:this.userId})
    if (undefined === cfg)
      return ({error:"Can't find user configuration."});


    const packageJsonString = EJSON.stringify(packageJson)
    var infoJsonHash = web3.utils.sha3(packageJsonString);
    //console.log("packageJson",packageJson);
    //console.log("Registration hash",infoJsonHash);

    const _id = (undefined === id)? Random.id():id
    const new_package = {
      _id:_id,
      infoJson: packageJsonString,
      infoJsonHash: infoJsonHash,
      srcFileName: src,
      binFileName: bin,
      registerTxId: "",
    }
    var found=false, idx
    for (idx in cfg.enclavePackages)
      if (cfg.enclavePackages[idx]._id === _id) {
        found = true;
        break
      }
    if (found){
      new_package.srcFileName = cfg.enclavePackages[idx].srcFileName
      new_package.binFileName = cfg.enclavePackages[idx].binFileName
      new_package.registerTxId = cfg.enclavePackages[idx].registerTxId
      cfg.enclavePackages[idx] = new_package
    } else {
        //cfg["enclavePackages"] = [];
        cfg.enclavePackages.push(new_package)
    }
    userCfgDb.update({_id:this.userId},
      {$set: {enclavePackages:cfg.enclavePackages}})
    return ({result:"Add enclave package success."});
  },

  updateTxnHash(_id, txnHash) {
      if (undefined === this.userId)
          return ({error:"Login required."})
      var cfg = getUserCfg(this.userId)
      //var cfg = userCfgDb.findOne({_id:this.userId})
      if (undefined === cfg)
          return ({error:"Can't find user configuration."});

      var found=false, idx
      for (idx in cfg.enclavePackages)
          if (cfg.enclavePackages[idx]._id === _id) {
              found = true;
              break
          }

      if (found){
          cfg.enclavePackages[idx].registerTxId = txnHash;
          userCfgDb.update({_id:this.userId},
              {$set: {enclavePackages:cfg.enclavePackages}});
          return ({result:"Txn hash set successfully"});

      } else {
          return ({error:"Package Id does not match"});
      }

  },

  registerPackage(_id, ethAccount, packageJson) {

      //var ethAccount = packageJson.ethereumAddress;
      var mrenclave = packageJson.mrEnclave;

      var txnHash = null;

      //var infoJSON = {"infoJSON": packageJson};
      var hashInfoJSON = web3.utils.sha3(JSON.stringify(packageJson));
      //var contractAddress = "0xfd4ccf9be718949cb90b700820b987b91a54632c"
      //var myContract = new web3.eth.Contract(abiArray,contractAddress);

      //console.log("hash infoJSON",hashInfoJSON);
      var Future = Npm.require('fibers/future');
      var passwordFut = new Future();

      Meteor.call("getPassword", ethAccount, (error, result) => {
          if(!error){
              passwordFut.return(result.password);
          }
          else{
              console.log(error);
              passwordFut.return(null);
          }
      })

      const ethpassword = passwordFut.wait();

      web3.eth.personal.unlockAccount(ethAccount, ethpassword)
          .then((response) => {
              myContract.methods.registerEnclavePackage(//mrenclave,
                 hashInfoJSON).send({from: ethAccount})
                  .then(function (receipt) {
                      // receipt can also be a new contract instance, when coming from a "contract.deploy({...}).send()"
                      console.log("Success-Receipt", receipt);
                      txnHash = receipt.transactionHash;
                      //console.log("inside",txnHash);
                      var postData = {"key": hashInfoJSON, "value": {"infoJSON": packageJson}};
                      // HTTP.post('http://localhost:4000/entry/insert', {
                      HTTP.post('https://genehub.genetank.ai/entry/insert', {
                          headers: {
                              'Content-Type': 'application/json'
                          },
                          // npmRequestOptions: {
                          //     rejectUnauthorized: false // TODO remove when deploy
                          // },
                          data: postData
                      }, (error, result) => {
                          if (!error) {
                              console.log(result);
                              web3.eth.personal.lockAccount(ethAccount)
                                  .then((response) => {
                                      //console.log(response);
                                      //console.log("outside",txnHash);
                                      Meteor.call("updateTxnHash", _id,txnHash, (error, result) => {
                                          if (error) {
                                              console.log("error:" + error);
                                          }
                                          else {
                                              console.log(result)
                                          }
                                      })
                                  }).catch((error) => {
                                  console.log(error);
                              })
                          }
                          else{
                              console.log(error);
                          }
                      });
                  })
          })
      //console.log("outside",txnHash);
      //return txnHash;

  }

})
