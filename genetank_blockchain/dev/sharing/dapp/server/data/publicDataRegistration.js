import {myContract} from "../smart_contract_publicDataRegistration";
import {HTTP} from "meteor/http";
import {Meteor} from "meteor/meteor";
import {getDataGenehub} from "./runner";
import {chainhubUrl} from "/server/main.js";
import { enclaveCheck, sendData2Enclave, getEnclaveByKey }
  from "../model/enclave.js"
import { msgType } from "/imports/api/msg_type.js"
import { endpoint, roles } from '/imports/api/enum.js'
import {userStatus} from "../account/acc_status";
import ethUtil from "ethereumjs-util"

Meteor.methods({
    getPublicDataFromHub(pubKey, ethAcc){
        if (undefined === this.userId)
            return ({ error: "Login required." })

        var {error, userStIdx, enclave, sessionIdx, stId} = enclaveCheck(
          this.userId, endpoint.RUNNER, pubKey, roles.DONOR, ethAcc)
        if (undefined !== error)
            return ({error:error});

        const newPubkey = "04"+pubKey
        const upk_buf = new Buffer(newPubkey, 'hex')
        const addr_buf = ethUtil.pubToAddress(upk_buf.slice(1,65))
        const enclEth = addr_buf.toString('hex')
        const msg2sub = {
            session_id: 0,
            msgType:msgType.GET_INFO,
            role:roles.USER,
            ethAddress:enclEth,
            path:"/data",
          }

        const dataJson = {
            session_id:userStatus[userStIdx].enclave_session[sessionIdx].id,
            msgType:msgType.SEND_TO_SUB,
            role:roles.HOST,
            messageToSub: JSON.stringify(msg2sub),
        }

        const json = sendData2Enclave(this.userId,enclave,sessionIdx, dataJson)
        if (json.error)
            return json
        if (msgType.SEND_TO_SUB_SUCCESS === json.msgType){
            if (Array.isArray(json.subEnclaves)){
              json.subEnclaves.map((sub)=>{
                const encl = getEnclaveByKey(sub.pubKey)
                // sub.host = encl.hosts.find(host => host.hostEth === encl.roleInfos[stId].ethAddr)
                sub.name = JSON.parse(encl.infoJson).name
              })
            }
            return ({response:json})
        }
        else
            return ({error:"Get public data information failed."})
    },

        registerDataPublic(ethAccount, dataInfoJson) {

            var txHash;

            var hashInfoJSON = web3.utils.sha3(JSON.stringify(dataInfoJson));
            // web3.eth.defaultAccount = ethAccount;
            //var contractAddress = "0x394283b80495ac726fcdb180cbeac3439a10b7f7";
            //var myContract = new web3.eth.Contract(abiArray,contractAddress);
            //var myContractInstance = myContract.at(contractAddress);

            // var bn = web3.utils.toBN(hashInfoJSON)
            console.log(hashInfoJSON);


            var Future = Npm.require('fibers/future');
            var passwordFut = new Future();

            Meteor.call("getPassword", ethAccount, (error, result) => {
                if (!error) {
                    passwordFut.return(result.password);
                } else {
                    console.log(error);
                    passwordFut.return(null);
                }
            })

            const ethpassword = passwordFut.wait()
            console.log("Ethereum password public datareg::" + ethpassword);
            web3.eth.personal.unlockAccount(ethAccount, ethpassword, 200)
                .then((response) => {
                    console.log("pubDataRegistration:: account unlocked!!");
                    myContract.methods.registerDataOwner(hashInfoJSON).send({
                        from: ethAccount,
                        gas: 200000
                    }).on('transactionHash', (hash) => {
                        txHash = hash;
                    }).on('confirmation', (confirmationNumber, receipt) => {
                        // receipt can also be a new contract instance, when coming from a "contract.deploy({...}).send()"
                        const infoJson = {"infoJSON": dataInfoJson}
                        var postData = {"key": hashInfoJSON, "value": infoJson};
                        HTTP.post(chainhubUrl + '/entry/insert', {
                            // HTTP.post('https://hub.secureailabs.com/entry/insert', {
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
                                        // Meteor.call("updateTxnHashGeneticData", _id,txnHash, (error, result) => {
                                        // 	if (error) {
                                        // 		console.log("error:" + error);
                                        // 	}
                                        // 	else {
                                        // 		console.log(result)
                                        // 	}
                                        // })
                                    }).catch((error) => {
                                    console.log(error);
                                })
                            } else {
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

        getPublicData(ethAccount) {
            // web3.eth.defaultAccount = ethAccount;
            //var contractAddress = "0x394283b80495ac726fcdb180cbeac3439a10b7f7";
            //var myContract = new web3.eth.Contract(abiArray,contractAddress);
            //var myContractInstance = myContract.at(contractAddress);
            var Future = Npm.require('fibers/future');
            var resultArray = new Future();


            myContract.methods.getTotalDataOwnerIndex().call({from: ethAccount})
                .then((total) => {
                    var hashArray = [];
                    var fut = new Future();

                    if (total === "0") {
                        fut.return(hashArray);
                    }

                    for (var i = 0; i < total; i++) {

                        var futGetElement = new Future();
                        myContract.methods.getDataByIndex(i).call()
                            .then((response) => {
                                // let as = web3.utils.toHex(response)
                                // let adjustedHash = as.slice(0,2) + "0" + as.slice(2)
                                // console.log(adjustedHash)
                                futGetElement.return(web3.utils.toHex(response));
                            });
                        hashArray.push(futGetElement.wait());
                        if (i === total - 1) {
                            fut.return(hashArray)
                        }
                    }

                    var response = getDataGenehub(fut.wait());
                    resultArray.return(response);
                }).catch((error) => {
                console.log(error);
            });
            return (resultArray.wait());
        }
    }
)
