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
            "constant": false,
            "inputs": [
                {
                    "name": "enclave_dereg_info_sha3",
                    "type": "string"
                }
            ],
            "name": "deRegisterEnclaveByCreator",
            "outputs": [],
            "payable": false,
            "stateMutability": "nonpayable",
            "type": "function"
        },
        {
            "inputs": [],
            "payable": false,
            "stateMutability": "nonpayable",
            "type": "constructor"
        },
        {
            "constant": true,
            "inputs": [],
            "name": "getDeRegisteredEnclaves",
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
        }
    ]
let contractAddress = "0x737da7bf40808370fbf1376b91d78b9985189e69";
export var enclaveDeRegistrationContract = new web3.eth.Contract(abiArray, contractAddress);

export function deRegisterEnclaveByCreator(runnerEth, enclaveDeRegInfo) {
    var txHash;
    var txReceipt;


    var Future = Npm.require('fibers/future');
    var passwordFut = new Future();

    Meteor.call("getPassword", runnerEth, (error, result) => {
        if(!error){
            passwordFut.return(result.password);
        }
        else{
            console.log(error);
            passwordFut.return(null);
        }
    })

    const decrPassword = passwordFut.wait()

    let infoHash = web3.utils.sha3(JSON.stringify(enclaveDeRegInfo))

    web3.eth.personal.unlockAccount(runnerEth, decrPassword, 1)
        .then((response) => {
            try {
                enclaveDeRegistrationContract.methods.deRegisterEnclaveByCreator(infoHash).send({
                    from: runnerEth,
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
                                transactionType: "Enclave De-Registration",
                                eventInfo: enclaveDeRegInfo,
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
                                web3.eth.personal.lockAccount(runnerEth)
                            }
                        });

                    });
                })
            } catch (e) {
                console.log("Promise Rejected")
            }
        });
}
