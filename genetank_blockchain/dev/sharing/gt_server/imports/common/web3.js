// Set up web3 object.

import Web3 from 'web3';
import Accounts from 'web3-eth-accounts';

// From https://github.com/ethereum/wiki/wiki/JavaScript-API#adding-web3
if (typeof web3 !== 'undefined') {
  web3 = new Web3(web3.currentProvider);
  web3.accounts = new Accounts(web3.currentProvider.host);
} else {
  // Set the provider you want from Web3.providers.
  web3 = new Web3(new Web3.providers.HttpProvider('http://localhost:8545'));
  web3.accounts = new Accounts('http://localhost:8545');
}

// Checks whether the given string has the given Keccak-256 SHA3 hash.
verifyStringSha3Hash = function (str, hash) {
  return web3.sha3(str).trim() === hash.trim();
}
