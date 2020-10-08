import { Meteor } from 'meteor/meteor';
import {HTTP} from "meteor/http";
import {web3url} from "/server/main.js";

var Web3 = require('web3');
if (typeof web3 !== 'undefined') {
    web3 = new Web3(web3.currentProvider);
} else {
    // set the provider you want from Web3.providers
    web3 = new Web3(new Web3.providers.HttpProvider(web3url));
}

var abiArray =
	[
		{
			"constant": false,
			"inputs": [
				{
					"name": "trainer_addr",
					"type": "address"
				}
			],
			"name": "donorClaimToken",
			"outputs": [
				{
					"name": "claimAmount",
					"type": "uint256"
				}
			],
			"payable": false,
			"stateMutability": "nonpayable",
			"type": "function"
		},
		{
			"constant": false,
			"inputs": [
				{
					"name": "creator_address",
					"type": "address"
				},
				{
					"name": "hash_json",
					"type": "uint256"
				}
			],
			"name": "registerAuditor",
			"outputs": [],
			"payable": false,
			"stateMutability": "nonpayable",
			"type": "function"
		},
		{
			"constant": false,
			"inputs": [
				{
					"name": "hash_data_json",
					"type": "uint256"
				}
			],
			"name": "registerDataOwner",
			"outputs": [],
			"payable": false,
			"stateMutability": "nonpayable",
			"type": "function"
		},
		{
			"constant": false,
			"inputs": [
				{
					"name": "trainer_address",
					"type": "address"
				},
				{
					"name": "hash_enclave_json",
					"type": "uint256"
				},
				{
					"name": "enclave_public_key",
					"type": "uint256"
				}
			],
			"name": "registerEnclaveByRunner",
			"outputs": [],
			"payable": false,
			"stateMutability": "nonpayable",
			"type": "function"
		},
		{
			"constant": false,
			"inputs": [
				{
					"name": "runner_address",
					"type": "address"
				},
				{
					"name": "hash_enclave_json",
					"type": "uint256"
				},
				{
					"name": "enclave_public_key",
					"type": "uint256"
				}
			],
			"name": "registerEnclaveByTrainer",
			"outputs": [],
			"payable": false,
			"stateMutability": "nonpayable",
			"type": "function"
		},
		{
			"constant": false,
			"inputs": [
				{
					"name": "hash_enclave_json",
					"type": "uint256"
				}
			],
			"name": "registerEnclavePackage",
			"outputs": [],
			"payable": false,
			"stateMutability": "nonpayable",
			"type": "function"
		},
		{
			"constant": false,
			"inputs": [
				{
					"name": "hash_model_json",
					"type": "uint256"
				},
				{
					"name": "fraction_token_to_allocate_donor",
					"type": "uint256"
				},
				{
					"name": "fraction_token_to_allocate_runner",
					"type": "uint256"
				}
			],
			"name": "registerModel",
			"outputs": [],
			"payable": false,
			"stateMutability": "nonpayable",
			"type": "function"
		},
		{
			"constant": false,
			"inputs": [
				{
					"name": "trainer_address",
					"type": "address"
				},
				{
					"name": "hash_json",
					"type": "uint256"
				}
			],
			"name": "registerModelAuditor",
			"outputs": [],
			"payable": false,
			"stateMutability": "nonpayable",
			"type": "function"
		},
		{
			"constant": false,
			"inputs": [
				{
					"name": "trainer_address",
					"type": "address"
				},
				{
					"name": "runner_address",
					"type": "address"
				},
				{
					"name": "v",
					"type": "uint8"
				},
				{
					"name": "r",
					"type": "bytes32"
				},
				{
					"name": "s",
					"type": "bytes32"
				},
				{
					"name": "donor_address",
					"type": "address"
				},
				{
					"name": "data_donor_json",
					"type": "uint256"
				}
			],
			"name": "registerReceipt",
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
			"inputs": [
				{
					"name": "index",
					"type": "uint256"
				}
			],
			"name": "getAiModelTrainerByIndex",
			"outputs": [
				{
					"name": "hash_model_json",
					"type": "uint256"
				},
				{
					"name": "trainer_addr",
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
					"name": "creator_address",
					"type": "address"
				}
			],
			"name": "getAuditorIndex",
			"outputs": [
				{
					"name": "index",
					"type": "uint256"
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
					"name": "data_owner_address",
					"type": "address"
				},
				{
					"name": "index",
					"type": "uint256"
				}
			],
			"name": "getDataInfoByIndex",
			"outputs": [
				{
					"name": "",
					"type": "uint256"
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
					"name": "trainer_addr",
					"type": "address"
				},
				{
					"name": "index",
					"type": "uint256"
				}
			],
			"name": "getEnclaveByTrainerAndIndex",
			"outputs": [
				{
					"name": "hash_enclave_json",
					"type": "uint256"
				},
				{
					"name": "enclave_public_key",
					"type": "uint256"
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
					"name": "trainer_addr",
					"type": "address"
				},
				{
					"name": "runner_addr",
					"type": "address"
				}
			],
			"name": "getEnclaveByTrainerAndRunner",
			"outputs": [
				{
					"name": "hash_enclave_json",
					"type": "uint256"
				},
				{
					"name": "enclave_public_key",
					"type": "uint256"
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
					"name": "creator_address",
					"type": "address"
				}
			],
			"name": "getEnclavePackage",
			"outputs": [
				{
					"name": "hash_json",
					"type": "uint256"
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
					"name": "trainer_address",
					"type": "address"
				},
				{
					"name": "runner_address",
					"type": "address"
				}
			],
			"name": "getRunner",
			"outputs": [
				{
					"name": "trainingOrNot",
					"type": "bool"
				},
				{
					"name": "hash_json",
					"type": "uint256"
				}
			],
			"payable": false,
			"stateMutability": "view",
			"type": "function"
		},
		{
			"constant": true,
			"inputs": [],
			"name": "getTotalAiModel",
			"outputs": [
				{
					"name": "",
					"type": "uint256"
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
					"name": "data_owner_address",
					"type": "address"
				}
			],
			"name": "getTotalData",
			"outputs": [
				{
					"name": "",
					"type": "uint256"
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
					"name": "trainer_addr",
					"type": "address"
				}
			],
			"name": "getTotalEnclaveByTrainerAddress",
			"outputs": [
				{
					"name": "index",
					"type": "uint256"
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
		}
	]

var contractAddress = "0xed9f4412e9dd1da7d5db55c6d5c3410b09cb305c";
export var myContract = new web3.eth.Contract(abiArray,contractAddress);

