//file: contract.js Author:sz Version: 0.1
import { EJSON } from 'meteor/ejson'

export var contract_info=
{
	"name":"GeneTankAI",
	"date":"2018.1.13.9",
	"address":"0xf85b06cc1f3cbaa411d9b5c2b46b953b86b6c471",
	"abi":
[
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
				"name": "is_confirmed_by_user",
				"type": "bool"
			}
		],
		"name": "confirmTransactionByUser",
		"outputs": [
			{
				"name": "",
				"type": "bool"
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
				"name": "is_confirmed_by_user",
				"type": "bool"
			}
		],
		"name": "findIfAllConditionsMet",
		"outputs": [
			{
				"name": "",
				"type": "bool"
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
				"name": "creator_address",
				"type": "address"
			},
			{
				"name": "runner_address",
				"type": "address"
			},
			{
				"name": "mrenclave",
				"type": "uint256"
			},
			{
				"name": "access_code",
				"type": "string"
			}
		],
		"name": "cancelAccessCode",
		"outputs": [
			{
				"name": "",
				"type": "bool"
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
				"name": "creator_addr",
				"type": "address"
			},
			{
				"name": "mrenclave",
				"type": "uint256"
			},
			{
				"name": "runner_address",
				"type": "address"
			}
		],
		"name": "getRunner",
		"outputs": [
			{
				"name": "att_report_hash",
				"type": "uint256"
			},
			{
				"name": "att_sign_hash",
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
				"name": "new_owner",
				"type": "address"
			}
		],
		"name": "transferOwnership",
		"outputs": [],
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
				"name": "runner_address",
				"type": "address"
			},
			{
				"name": "user_confirms",
				"type": "bool"
			},
			{
				"name": "mrenclave",
				"type": "uint256"
			},
			{
				"name": "access_code",
				"type": "string"
			}
		],
		"name": "sendDepositAccessCode",
		"outputs": [
			{
				"name": "",
				"type": "bool"
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
				"name": "runner_address",
				"type": "address"
			},
			{
				"name": "mrenclave",
				"type": "uint256"
			}
		],
		"name": "getAttestationReport",
		"outputs": [
			{
				"name": "",
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
				"name": "creator_addr",
				"type": "address"
			}
		],
		"name": "getEnclaveByCreator",
		"outputs": [
			{
				"name": "name",
				"type": "string"
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
				"name": "creator_addr",
				"type": "address"
			},
			{
				"name": "mrenclave",
				"type": "uint256"
			}
		],
		"name": "getEnclavePackage",
		"outputs": [
			{
				"name": "m_s_ver",
				"type": "string"
			},
			{
				"name": "hash_bin_file",
				"type": "uint256"
			},
			{
				"name": "hash_src_file",
				"type": "uint256"
			},
			{
				"name": "hash_public_key",
				"type": "uint256"
			},
			{
				"name": "hash_signature",
				"type": "uint256"
			}
		],
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
		"constant": false,
		"inputs": [
			{
				"name": "creator_address",
				"type": "address"
			},
			{
				"name": "signed_hash_auditing_package",
				"type": "uint256"
			},
			{
				"name": "mrenclave",
				"type": "uint256"
			},
			{
				"name": "result",
				"type": "uint8"
			},
			{
				"name": "public_key",
				"type": "string"
			},
			{
				"name": "certificate",
				"type": "string"
			},
			{
				"name": "auditor_signature",
				"type": "string"
			}
		],
		"name": "registerEnclaveByAuditor",
		"outputs": [],
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
				"name": "mrenclave",
				"type": "uint256"
			},
			{
				"name": "att_report_hash",
				"type": "uint256"
			},
			{
				"name": "att_sign_hash",
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
				"name": "mrenclave",
				"type": "uint256"
			},
			{
				"name": "m_s_ver",
				"type": "string"
			},
			{
				"name": "hash_bin_file",
				"type": "uint256"
			},
			{
				"name": "hash_src_file",
				"type": "uint256"
			},
			{
				"name": "name",
				"type": "string"
			},
			{
				"name": "hash_signature",
				"type": "uint256"
			},
			{
				"name": "hash_public_key",
				"type": "uint256"
			}
		],
		"name": "registerEnclaveByTrainer",
		"outputs": [],
		"payable": false,
		"stateMutability": "nonpayable",
		"type": "function"
	}
]
}

export var contract = web3.eth.contract(contract_info.abi).at(contract_info.address);

export function setContractInfo(json) {
  contract_info = json;
  contract = web3.eth.contract(contract_info.abi).at(contract_info.address);
}
