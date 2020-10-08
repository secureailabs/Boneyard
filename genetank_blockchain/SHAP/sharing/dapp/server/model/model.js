import { Meteor } from 'meteor/meteor';
import { userCfgDb, getUserCfg } from '../account/account.js';
import { myContract } from "/server/smartcontract.js";

import { Random } from 'meteor/random'

import { EJSON } from 'meteor/ejson'
import {HTTP} from "meteor/http";

const crypto = require('crypto');
const hash = crypto.createHash('sha256');

Meteor.methods({
  userDelModel(id) {
    if (undefined === this.userId)
      return ({error:"Login required."})
    //var cfg = userCfgDb.findOne({_id:this.userId})
    var cfg = getUserCfg(this.userId)
    if (undefined === cfg)
      return ({error:"Can't find user configuration."});
    var models = cfg.models
    var i//=models.length
    var found=false;
    for (i in models)
      if (models[i]._id === id){
        found = true
        break;
      }
    if (found) {
      models.splice(i, 1)
      userCfgDb.update({_id:this.userId},{$set: {models:models}});
      return({result:"Delete model "+id+" success."})
    }
    return({error:"Delete model "+id+" fail.", result:"Model id not found."})
  },

  userAddModModel(modelInfoJson, src, bin, id) {
    if (undefined === this.userId)
      return ({error:"Login required."})
    //var cfg = userCfgDb.findOne({_id:this.userId})
    var cfg = getUserCfg(this.userId)
    if (undefined === cfg)
      return ({error:"Can't find user configuration."});
    const infoJsonString = EJSON.stringify(modelInfoJson)
    var infoJsonHash = web3.utils.sha3(infoJsonString);
    //hash.update(infoJsonString)
    //const infoJsonHash = hash.digest('hex')
    const _id = (undefined === id || "" === id)? Random.id():id
    var i//=models.length
    var found=false;
    for (i in cfg.models)
      if (cfg.models[i]._id === _id){
        found = true
        break;
      }
    if (found) {
      if (undefined !== cfg.models[i].registerTxId)
        return({error:"Registered model can't be modified, remove _id value to create a new one"})
      cfg.models[i].infoJson = infoJsonString
      cfg.models[i].infoJsonHash = infoJsonHash
      cfg.models[i].srcFiles = src
      cfg.models[i].binFiles = bin
    } else
      cfg.models.push({
        _id:_id, //random id
        infoJson: infoJsonString, //stringified infoJson obj
        infoJsonHash: infoJsonHash,
        srcFiles: src,
        binFiles: bin
      })
    userCfgDb.update({_id:this.userId},{$set: {models:cfg.models}});
    if (found)
      return ({result:"Modify AI Model success."});
    return ({result:"Add AI Model success."});
  },

  updateTxnHashModel(_id, txnHash) {
        if (undefined === this.userId)
            return ({error:"Login required."})
        var cfg = getUserCfg(this.userId)
        //var cfg = userCfgDb.findOne({_id:this.userId})
        if (undefined === cfg)
            return ({error:"Can't find user configuration."});

        var found=false, idx
        for (idx in cfg.models)
            if (cfg.models[idx]._id === _id) {
                found = true;
                break
            }

        if (found){
            cfg.models[idx].registerTxId = txnHash;
            userCfgDb.update({_id:this.userId},
                {$set: {models:cfg.models}});
            return ({result:"Txn hash set successfully"});

        } else {
            return ({error:"Package Id does not match"});
        }

    },

  registerModel(_id, ethAccount, infoJson) {

        //var ethAccount = packageJson.ethereumAddress;
        var fractionTokenDonor = 1;

        var txnHash = null;

        //var infoJSON = {"infoJSON": packageJson};
        var hashInfoJSON = web3.utils.sha3(JSON.stringify(infoJson));
        //var contractAddress = "0xda6ccdf90fe9414ab7ee3d4b0ac22f1b10c63904";
        //var myContract = new web3.eth.Contract(abiArray,contractAddress);

        console.log("hash infoJSON",hashInfoJSON);

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

        //console.log(ethAccount, ethpassword, hashInfoJSON, fractionTokenDonor)
        web3.eth.personal.unlockAccount(ethAccount, ethpassword)
            .then((response) => {
                myContract.methods.registerModel(hashInfoJSON, fractionTokenDonor, 10).send({from: ethAccount})
                    .then(function (receipt) {
                        // receipt can also be a new contract instance, when coming from a "contract.deploy({...}).send()"
                        console.log("Success-Receipt", receipt);
                        txnHash = receipt.transactionHash;
                        //console.log("inside",txnHash);
                        var postData = {"key": hashInfoJSON, "value": {"infoJSON": JSON.stringify(infoJson)}};
                        HTTP.post('http://localhost:4100/entry/insert', {
                        //HTTP.post('https://hub.secureailabs.com/entry/insert', {
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
                                        Meteor.call("updateTxnHashModel", _id,txnHash, (error, result) => {
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

  }
})
