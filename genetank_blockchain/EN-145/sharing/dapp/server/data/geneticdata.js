import { Meteor } from 'meteor/meteor';
import { Mongo } from 'meteor/mongo';
import { userCfgDb, getUserCfg } from '../account/account.js';
import { userStatus, getUserSt } from '../account/acc_status.js';
import { EJSON } from 'meteor/ejson'
import { Random } from 'meteor/random'
import {HTTP} from "meteor/http";
import { shareData } from './share.js'
import { getDataGenehub } from './runner.js'
import { myContract } from "/server/smartcontract.js";

var fs = require('fs')

Meteor.methods({
  userAddGeneticEhrData(dataInfoJson, id, fn) {
    if (undefined === this.userId)
      return ({ error: "Login required." })
    var cfg = getUserCfg(this.userId)
    if (undefined === cfg)
      return ({ error: "Can't find user configuration." });
    const dataInfoJsonString = EJSON.stringify(dataInfoJson)
    const _id = (undefined === id) ? Random.id() : id
    var infoJsonHash = web3.utils.sha3(dataInfoJsonString);
    // console.log("packageJson",packageJson);
    //console.log("Registration hash",infoJsonHash);
    const new_Data = {
      _id: _id,
      infoJson: dataInfoJsonString,
      infoJsonHash: infoJsonHash,
      filename: fn,
      registerTxId: "",
      shareInfo: [],
    }
    var idx
    var found = false
    for (idx in cfg.data)
      if (cfg.data[idx]._id === _id) {
        found = true;
        break
      }
    if (found) {
      //new_Data.filename = cfg.data[idx].filename
      new_Data.registerTxId = cfg.data[idx].registerTxId
      new_Data.shareInfo = cfg.data[idx].shareInfo
      cfg.data[idx] = new_Data
    } else {
      cfg.data.push(new_Data)
    }

    userCfgDb.update({ _id: this.userId }, { $set: { data: cfg.data } });
    return ({ result: "Add Data success." });
  },

  userDelData(id) {
    if (undefined === this.userId)
      return ({ error: "Login required." })
    //var cfg = userCfgDb.findOne({_id:this.userId})
    var cfg = getUserCfg(this.userId)
    if (undefined === cfg)
      return ({ error: "Can't find user configuration." });
    var data = cfg.data
    var i//=models.length
    var found = false;
    for (i in data)
      if (data[i]._id === id) {
        found = true
        break;
      }
    if (found) {
      data.splice(i, 1)
      userCfgDb.update({ _id: this.userId }, { $set: { data: data } });
      return ({ result: "Delete data " + id + " success." })
    }
    return ({ error: "Delete data " + id + " fail.", result: "Id not found." })
  },

  userShareData(shareInfo, checked){
    if (undefined === this.userId)
      return ({ error: "Login required." })
    var cfg = getUserCfg(this.userId)
    if (undefined === cfg)
      return ({ error: "Can't find user configuration." });
    var found=false, idx
    for (idx in checked)
      if (checked[idx] === true) {   
        const ret = shareData(this.userId, shareInfo.enclavePubKey, idx,
          shareInfo.ethereumAddress)
        if (ret.error)
          return({error:ret.error})
        shareInfo.receipt = ret.receipt
        const shareInfoString = EJSON.stringify(shareInfo)
        cfg.data[idx].shareInfo.push(shareInfoString)
        userCfgDb.update({ _id: this.userId },{$set: {data:cfg.data}})
    } 
    // console.log(cfg.data)

    return ({result:"Share data to enclave success."});

  },

  updateTxnHashGeneticData(_id, txnHash) {
        if (undefined === this.userId)
            return ({error:"Login required."})
        var cfg = getUserCfg(this.userId)
        //var cfg = userCfgDb.findOne({_id:this.userId})
        if (undefined === cfg)
            return ({error:"Can't find user configuration."});

        var found=false, idx
        for (idx in cfg.data)
            if (cfg.data[idx]._id === _id) {
                found = true;
                break
            }

        if (found){
            cfg.data[idx].registerTxId = txnHash;
            userCfgDb.update({_id:this.userId},
                {$set: {data:cfg.data}});
            return ({result:"Txn hash set successfully"});

        } else {
            return ({error:"Package Id does not match"});
        }

  },

    registerGeneticData(_id, ethAccount, ethpassword, dataInfoJson) {


        var hashInfoJSON = web3.utils.sha3(JSON.stringify(dataInfoJson));
        web3.eth.defaultAccount = ethAccount;
        //var contractAddress = "0x394283b80495ac726fcdb180cbeac3439a10b7f7";
        //var myContract = new web3.eth.Contract(abiArray,contractAddress);
        //var myContractInstance = myContract.at(contractAddress);

        // var bn = web3.utils.toBN(hashInfoJSON)
        console.log(hashInfoJSON);

        web3.eth.personal.unlockAccount(ethAccount, ethpassword, 200)
            .then((response) => {
                myContract.methods.registerDataOwner(hashInfoJSON).send({from: ethAccount, gas: 1500000})
                    .then((receipt) => {
                        // receipt can also be a new contract instance, when coming from a "contract.deploy({...}).send()"
                        console.log("Success-Receipt", receipt);
                        txnHash = receipt.transactionHash;
                        var postData = {"key": hashInfoJSON, "value": {"infoJSON": JSON.stringify(dataInfoJson)}};
                        // HTTP.post('http://localhost:4000/entry/insert', {
                        HTTP.post('https://hub.secureailabs.com/entry/insert', {
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
                                        Meteor.call("updateTxnHashGeneticData", _id,txnHash, (error, result) => {
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
                    }).catch((error) => {
                    console.log(error);
                })
            }).catch((error) => {
            console.log(error);
        })

    },

    getGeneticDataByEthAcc(ethAccount) {
        web3.eth.defaultAccount = ethAccount;
        //var contractAddress = "0x394283b80495ac726fcdb180cbeac3439a10b7f7";
        //var myContract = new web3.eth.Contract(abiArray,contractAddress);
        //var myContractInstance = myContract.at(contractAddress);
        var Future = Npm.require('fibers/future');
        var resultArray = new Future();


        myContract.methods.getTotalData(ethAccount).call({from: ethAccount})
            .then((total) => {
                var hashArray = [];
                var fut = new Future();
                console.log(total);

                if(total === "0") {
                    fut.return(hashArray);
                }

                for(var i=0; i<total; i++){

                    var futGetElement = new Future();
                    myContract.methods.getDataInfoByIndex(ethAccount,i).call()
                        .then((response) => {
                            // let as = web3.utils.toHex(response)
                            // let adjustedHash = as.slice(0,2) + "0" + as.slice(2)
                            // console.log(adjustedHash)
                            futGetElement.return(web3.utils.toHex(response));
                        });
                    hashArray.push(futGetElement.wait());
                    if(i===total-1) {
                        fut.return(hashArray)
                    }
                }

                var response = getDataGenehub(fut.wait());
                resultArray.return(response);
            }).catch((error) => {
            console.log(error);
        });
        return (resultArray.wait());
    },

})
