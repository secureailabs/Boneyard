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
			"constant": true,
			"inputs": [
				{
					"name": "index",
					"type": "uint256"
				}
			],
			"name": "getDataByIndex",
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
			"inputs": [],
			"name": "getTotalDataOwnerIndex",
			"outputs": [
				{
					"name": "",
					"type": "uint256"
				}
			],
			"payable": false,
			"stateMutability": "view",
			"type": "function"
		}
	]
var contractAddress = "0xb8e22a0426a640c876b2f23a57372539f4778673";
export var myContract = new web3.eth.Contract(abiArray,contractAddress);

