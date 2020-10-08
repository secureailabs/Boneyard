# Preparing knowledge

It is better to go through the solicity language in https://solidity.readthedocs.io/en/develop/index.html.

You may also need to run through the Web3 Javascript Ethereum API https://github.com/ethereum/wiki/wiki/JavaScript-API before starting your work.

# Ethereum smart contract guides

Two ways to access to the testRPC blockchain in our test environment:

1. Use the Dapp to connect to http://ec2-52-204-170-69.compute-1.amazonaws.com:8545. This can be done by the javascripts of the Dapp with the below codes:

``if(typeof web3 === 'undefined')``

``    web3 = new Web3(new Web3.providers.HttpProvider('http://ec2-52-204-170-69.compute-1.amazonaws.com:8545'));``

2. Use metamask Ethereum wallet plug in of the browser on your local computer. Configure the metamask use the private test network with the RPC "'http://ec2-52-204-170-69.compute-1.amazonaws.com:8545". The Dapps use the below codes to talk to metamask and interact with the test network:

``if(typeof web3 === 'undefined')``

``    web3 = new Web3(new Web3.providers.HttpProvider('http:localhost:8545'));``


# Online Solidity compiling and deploying tool

We can use http://ethereum.github.io/browser-solidity/#version=soljson-latest.js for smart contract compiling and deploying (require metamask for deploying). See http://zeltsinger.com/2016/10/18/ethereum-dapp-front-end-display-variables/ for tutorial materials. 

After the smart contract is deployed, we can use the following javascript codes in the Dapp or the brower console to access the smart contract.

``web3.eth.defaultAccount = "0x..."; //Set the default account for smart contract access``

``var abiArray = [...]; //You can copy the abi of the smart contract from the online compiler, and paste over "[...]".``

``var contractAddress = "0x..."; //You can copy the contract address from the online compiler after the contract has been created. You can find the contract address right after "contract_name at 0x...".``

``var contract = web3.eth.contract(abiArray).at(contractAddress); //``

``contract.yourContractFunction.call(); //Call your smart contract function``

If you lose the smart contract address accidentally, you can calculate the address with solidity codes:

``address addr = "0x..."; //"0x..." is the account address which deployed the smart contract``

``unit8 trans_nonce = xx; // xx is the value of the nonce of the transaction that deploy the smart contract. You may find it use the web3.eth.getTransaction() on the brower console.``

``address contractAddress = address(keccak256(0xd6, 0x94, addr, trans_nonce))``

# Install modules for using React

$ cd ${platform_root}/sharing/dapp

$ meteor npm install --save react react-dom react-router-dom

$ meteor add  react-meteor-data


