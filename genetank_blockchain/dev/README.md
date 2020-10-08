# genetank_blockchain


# Skipped features of phase-I

Enclave package auditing

Enclave package validation by the runner

AI model installation: Transfer AI model to a new enclave / Upgrade a new AI model version to an enclave

Enclave attestation report validation by the trainer and query user

Access code validation by the enclave

ModelRepo on Genetank website

# New tasks of phase-II 

1) Donor Dapp (the same Dapp as query user):

Phenotype data management: questionnaire, validation, preprocessing, continuing update, import health reports/medical records

Genotype data management: data import, data preprocessing

P2p messaging: blockchain account address as ID, p2p messaging, closing the feedback loop

Genetic data search request handling, trainer <--> donor interaction

2) Fixed dollar value GTT: buy and sell with PayPal, transfer, pay to smart contract

3) New Genetank AI Smart contract supports data donation

4) AI models for new applications

5) SGX enclave supports model training

6) Automated testing facility


# Directories of this repository

ai: The AI model, including all the files for model development and training

doc: The documents about the development

gtt: The files for Genetank Token development

sharing: The AI model releasing files, including all the files for model sharing on the blockchain

sharing/dapp: The html/javascripts for the Dapp.

sharing/dapp/client: The frontend of the Dapp. It includes the html/javascripts for the model trainer, runner, and end user.

sharing/dapp/imports: The shared codes for both backend and front end of the Dapp.

sharing/dapp/server: The backend of the Dapp.

sharing/sgx: The files for the sgx enclave and application development.

sharing/genehub: The code for the GeneHub server.

sharing/gt_server: The backend and front end of the Genetank server which provides attestation server functions

sharing/tools: The development directory for tools such as encryption, signature verification tools.


# Git command guides

You can follow the below commands for usage of git.


Config your information

$ git config --global user.name “Your name“

$ git config --global user.email youremail@mail.com


Download the repository

$ git clone https://github.com/zzz2010/genetank_blockchain.git

$ git checkout phaseII


Daily update your local codes from the repository

$ git pull origin phaseII


Upload your changes to the repository

$ git add *

$ git commit -m “Brief descriptions for the changes"

$ git push origin phaseII


# Run A Genetank Private Ethereum Blockchain

Please refer to  https://github.com/ethereum/go-ethereum/wiki/Private-network for complete instructions

If the blockchain hasn't been created, use below command to create one:

$ mkdir /home/genetank/mychain

The directory can be any one on the computer running the geth software

$ cd /home/genetank/mychain

Create a genesis.json with below contents in the directory:

{

    "config": {
        "chainId": 9716,
        "homesteadBlock": 0,
        "eip155Block": 0,
        "eip158Block": 0
    },
    "difficulty": "200000000",
    "gasLimit": "2100000",
    "alloc": {
        "B480c3233e1852Ea00Fd0EEc7149b2a6E67c23B2":
          { "balance": "300000000000000000000" },
        "3bbbfaB8e22538667848284Ff78ee800C7aD69d2":
          { "balance": "300000000000000000000" }
    }
}

Create a private blockchain

$ geth --datadir "/home/genetank/mychain/Ethereum" init genesis.json


If there is no bootstrap node (a node for private blockchain discovery), run the below commands.

Configure a bootstrap node

$ bootnode --genkey=boot.key

Run the bootstrap node

$ bootnode --nodekey=boot.key

Run an Ethereum node

$ geth --datadir "/home/genetank/mychain/Ethereum" --networkid 9716 --bootnodes enode://105cb2df56296e27abdbe31eb5ca72a5ace5b3d34e2500bfc4045374b0ebe6571e74b07bbe21c49d7e393cbc8cb9ab09c10d9ee9c951f6b7b7923585320d6c98@[127.0.0.1]:30301 --mine --minerthreads=1 --etherbase=0xB480c3233e1852Ea00Fd0EEc7149b2a6E67c23B2 --identity "GeneTank" --rpc --rpcport "8000" --rpccorsdomain "*" --port "30303" --rpcapi "admin,db,eth,debug,miner,net,shh,txpool,personal,web3" --nat "any" --verbosity "2" console


# Daily Run Test Guides

The following commands can be use on the development server ec2-52-204-170-69.compute-1.amazonaws.com.

Run model sharing server if it is down.

$ cd ${platform_root}/sharing/dapp

$ export set ROOT_URL=http://blockchain.genetank.ai:3200

$ nohup meteor --port 3200 >server.log 2>&1 &

$ disown -h %1

Testing: Use web browser to open: 

http://localhost:3200

or 

http://ip_address:3200

or

http://ec2-52-204-170-69.compute-1.amazonaws.com:3000
