var GeneTankAI = artifacts.require("GeneTankAI");

var Web3 = require('web3')
var web3 = new Web3(new Web3.providers.HttpProvider('http://localhost:8545'))


contract('GeneTankAI', function(accounts) {

    it('Test 1 ecrecover result matches address', function(done) {
        var address = accounts[0]
        GeneTankAI.deployed().then(async function(instance) {
            var msg = '0x8CbaC5e4d803bE2A3A5cd3DbE7174504c6DD0c1C'
            //var msg = '0x6CbaC5e4d803bE2A3A5cd3DbE7174504c6DD0c1C'

            var h = web3.sha3(msg)
            var sig = web3.eth.sign(address, h).slice(2)
            var r = `0x${sig.slice(0, 64)}`
            var s = `0x${sig.slice(64, 128)}`
            var v = web3.toDecimal(sig.slice(128, 130)) + 27
            var result = await instance.verifySignature.call(h, v, r, s)
            console.log("result:", result);
            console.log("address:", address);
            assert.equal(result, address)
            done();
        });
    });

    it('Test SGX ecrecover result matches address', function(done) {
        var address = accounts[0]
        GeneTankAI.deployed().then(async function(instance) {
            var h = 0x3DAC3DC96C881174C963D42E14C32D663AAB0363581AB463F026633F296924BB
    
            var r = 0x47442A1E19C369A9C187C8CEF45EBA3A81BA29E2654289FBF20A65D1DDE79821//`0x${sig.slice(0, 64)}`
            
            var s = 0x3819EAFB83AA221E78E06536462AA85DE3D1197252A525A1DDF1F9CD5390BAC6//`0x${sig.slice(64, 128)}`
            
            var v = 27//web3.toDecimal(sig.slice(128, 130)) + 27
            var result = await instance.verifySignature.call(h, v, r, s)
            console.log("result:", result);
            console.log("address:", address);
            assert.equal(result, address)
            done();
        });
    });

    
});
