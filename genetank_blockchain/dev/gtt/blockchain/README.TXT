#pre-conditions
User: ubuntu
Home: /home/ubuntu
pwd: /home/ubuntu/blockchain

#Init wallet
geth --datadir ./data init beta.json

#run wallet
geth --networkid=8878 --datadir=/home/ubuntu/blockchain/data --cache=512 --syncmode=full --ethstats=shifa_geth:gtai1234@34.199.72.224:8083 --bootnodes=enode://56e238797ca68407ed239f35108567f6018a34c2485521e7cfa6fd848aecc4dbebad651833198e8277b54b17c9974e56fb4db7b78b7a3b91d1101040700c2487@34.199.72.224:30319 --port 30335 --rpc --rpcport 8530 -rpcaddr 0.0.0.0 --rpcvhosts='*' --rpcapi admin,db,eth,debug,net,shh,txpool,personal,web3 --allow-insecure-unlock

#run wallet for dApp
geth --networkid=8878 --datadir=/home/ubuntu/.dapp_wallet --cache=512 --syncmode=fast --port 30326 --rpc --rpcport 8500 --rpcaddr "0.0.0.0" --rpccorsdomain "*" --allow-insecure-unlock --rpcapi admin,db,eth,debug,net,txpool,personal,web3
