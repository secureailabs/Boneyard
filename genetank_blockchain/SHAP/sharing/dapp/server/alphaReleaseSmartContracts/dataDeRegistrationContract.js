import {Meteor} from 'meteor/meteor';
import {HTTP} from "meteor/http";
import {web3url} from "/server/main.js";
import {chainhubUrl} from "/server/main.js";

var Web3 = require('web3');
if (typeof web3 !== 'undefined') {
    web3 = new Web3(web3.currentProvider);
} else {
    // set the provider you want from Web3.providers
    web3 = new Web3(new Web3.providers.HttpProvider(web3url));
}

const crypto = Npm.require('crypto');

let abiArray =
    [
        {
            "constant": true,
            "inputs": [],
            "name": "getDeRegisteredDataList",
            "outputs": [
                {
                    "name": "",
                    "type": "string[]"
                }
            ],
            "payable": false,
            "stateMutability": "view",
            "type": "function"
        },
        {
            "constant": true,
            "inputs": [],
            "name": "owner",
            "outputs": [
                {
                    "name": "",
                    "type": "address"
                }
            ],
            "payable": false,
            "stateMutability": "view",
            "type": "function"
        },
        {
            "constant": false,
            "inputs": [
                {
                    "name": "data_dereg_info_sha3",
                    "type": "string"
                }
            ],
            "name": "deRegisterDataByOwner",
            "outputs": [],
            "payable": false,
            "stateMutability": "nonpayable",
            "type": "function"
        },
        {
            "constant": true,
            "inputs": [
                {
                    "name": "",
                    "type": "uint256"
                }
            ],
            "name": "sha3Hashes",
            "outputs": [
                {
                    "name": "",
                    "type": "string"
                }
            ],
            "payable": false,
            "stateMutability": "view",
            "type": "function"
        },
        {
            "inputs": [],
            "payable": false,
            "stateMutability": "nonpayable",
            "type": "constructor"
        }
    ]
let contractAddress = "0x86d54e4eaedcb0df17d733e5294957152c0e8186";
export var dataDeRegistrationContract = new web3.eth.Contract(abiArray, contractAddress);

export function deRegisterDataByOwner(donorEth, dataInfo) {
    var txHash;
    var txReceipt;

    var Future = Npm.require('fibers/future');
    var passwordFut = new Future();

    Meteor.call("getPassword", donorEth, (error, result) => {
        if(!error){
            passwordFut.return(result.password);
        }
        else{
            console.log(error);
            passwordFut.return(null);
        }
    })

    const decrPassword = passwordFut.wait()

    let infoHash = web3.utils.sha3(JSON.stringify(dataInfo))

    web3.eth.personal.unlockAccount(donorEth, decrPassword, 1)
        .then((response) => {
            try {
                dataDeRegistrationContract.methods.deRegisterDataByOwner(infoHash).send({
                    from: donorEth,
                    gas: 200000
                }).on('transactionHash', (hash) => {
                    txHash = hash;
                }).on('confirmation', (confirmationNumber, receipt) => {
                    console.log(confirmationNumber)
                    txReceipt = receipt;
                    web3.eth.getTransaction(txHash).then((transactionInfo) => {
                        let postData = {
                            "key": infoHash,
                            "value": {
                                transactionType: "Data De-Registration/Removal",
                                eventInfo: dataInfo,
                                transactionInfo: transactionInfo,
                                //transactionReceipt: txReceipt
                            }
                        };
                        HTTP.post(chainhubUrl + '/entry/insert', {
                            headers: {
                                'Content-Type': 'application/json'
                            },
                            data: postData
                        }, (error, result) => {
                            if (!error) {
                                console.log("Data Posted");
                                console.log(result);
                                web3.eth.personal.lockAccount(donorEth)
                            }
                        });

                    });
                })
            } catch (e) {
                console.log("Promise Rejected")
            }
        });
}
