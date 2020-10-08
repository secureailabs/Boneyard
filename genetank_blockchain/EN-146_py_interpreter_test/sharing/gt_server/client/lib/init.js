if(typeof web3 === 'undefined')
    //web3 = new Web3(new Web3.providers.HttpProvider('http://blockchain.genetank.ai:8545'));
    web3 = new Web3(new Web3.providers.HttpProvider('http://localhost:8545'));
else
    web3 = new Web3(web3.currentProvider);

/*
const accounts = web3.eth.accounts;

if (web3.eth.defaultAccount == undefined) {
  web3.eth.defaultAccount = accounts[0];
}
*/
