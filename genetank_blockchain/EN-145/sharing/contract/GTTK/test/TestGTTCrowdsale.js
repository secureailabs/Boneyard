var GTTCrowdsale = artifacts.require("GTTCrowdsale");
var GTTToken = artifacts.require("GTTToken");

var MetaCoin = artifacts.require("./MetaCoin.sol");


contract('GTTCrowdsale', function(accounts) {
    it('should deploy GTT token', function(done){
        GTTCrowdsale.deployed().then(async function(instance) {
            const token = await instance.token.call();
            console.log("instance.accounts:" + web3.eth.accounts);
            assert(token, 'Token address could not be deployed');
            done();
       });
    });

    it('one ETH should buy 3 GTT Tokens', function(done){
        GTTCrowdsale.deployed().then(async function(instance) {
            const data = await instance.sendTransaction({ from: accounts[7], value: web3.toWei(1, "ether")});
            const tokenAddress = await instance.token.call();
            const gtt = GTTToken.at(tokenAddress);
            const tokenAmount = await gtt.balanceOf(accounts[7]);
            console.log("token Amount:", tokenAmount.toString());
            assert.equal(tokenAmount.toNumber(), 3000000000000000000, 'The sender didn\'t receive the tokens as per PreICO rate');
            done();
       });
    });

    

    it('should transfer GTT token using smartcontract', function(done){
        GTTCrowdsale.deployed().then(async function(instance) {
           
            const data = await instance.sendTransaction({ from: accounts[0], value: web3.toWei(1, "ether")});
            const tokenAddr = await instance.token.call();
            const gtt = GTTToken.at(tokenAddr);
            const tokenAmount = await gtt.balanceOf(accounts[0]);
            console.log("token Amount:", tokenAmount.toString());
       

            // get the GTT token address
           // const tokenAddress = await instance.token.call();
          //  const gtt = GTTToken.at(tokenAddress);
            // number of tokens that are approved to send from an account
            const tokApp = await gtt.approve(accounts[1], 200000);
         //   console.log("tokApp:" + tokApp);
            const allo = await gtt.allowance.call(accounts[0], accounts[1]);
            console.log("allo:" +  allo);
        //    let balanceAccount2Before = await web3.eth.getBalance(accounts[2]);
        //    console.log("balanceAccount3Before:", balanceAccount2Before);
            const tokTrans = await gtt.transferFrom(accounts[0], accounts[3], 200000, {from: accounts[1]});
        //    console.log("tokTrans:" + tokTrans);
       //     let balanceAccount2After = await web3.eth.getBalance(accounts[2]);
       //     console.log("balanceAccount3After:", balanceAccount2After);

            // call the example code to verify if smartcontract can transfer GTT token
            //const
            //assert(token, 'Token address could not be deployed');
            done();
       });
    });

    it("should put 10000 MetaCoin in the first account", function() {
        return MetaCoin.deployed().then(function(instance) {
          return instance.getBalance.call(accounts[0]);
        }).then(function(balance) {
          assert.equal(balance.valueOf(), 10000, "10000 wasn't in the first account");
        });
    });
});