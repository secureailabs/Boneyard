var GTTCrowdsale = artifacts.require("./GTTCrowdsale.sol");

module.exports = function(deployer) {
  deployer.deploy(GTTCrowdsale, 
    3, 
    "0xa2075b6b50b0c26aa47025a6303997c8dcda2fb2", 
    2000000000000000000,
    500000000000000000000 
  );
};
