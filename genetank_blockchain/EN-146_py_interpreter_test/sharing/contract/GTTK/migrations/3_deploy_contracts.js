var GeneTankStructs = artifacts.require("./GeneTankStructs.sol");
var GeneTankAI = artifacts.require("./GeneTankAI.sol");
var GTTToken = artifacts.require("./GTTToken.sol");
module.exports = function(deployer) {
  deployer.deploy(GeneTankStructs);
  deployer.deploy(GTTToken);
  
  deployer.link(GeneTankStructs, GeneTankAI);
  deployer.link(GTTToken, GeneTankAI);
  deployer.deploy(GeneTankAI, 5);
};
