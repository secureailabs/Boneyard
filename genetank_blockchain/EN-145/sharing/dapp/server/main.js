import { Meteor } from 'meteor/meteor';
import { initAccount } from '/imports/startup/server/p2p/p2paccount.js';
import { initSysAccount } from './account/account.js';

export var web3url = "http://35.153.217.215:3200"
export var chainhubUrl = "http://35.153.217.215:4100";

var Web3 = require('web3');
if (typeof web3 !== 'undefined') {
    web3 = new Web3(web3.currentProvider);
} else {
    // set the provider you want from Web3.providers
    web3 = new Web3(new Web3.providers.HttpProvider(web3url));
}
Meteor.startup(() => {
  initSysAccount();
  initAccount();
});

