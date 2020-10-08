pragma solidity ^0.4.18;

import './GTTToken.sol';
import './CappedCrowdsale.sol';
//import 'zeppelin-solidity/contracts/crowdsale/FinalizableCrowdsale.sol';
import './Ownable.sol';

/**
 * @title GTT Crowdsale 
 * @dev This contract can be used to set the rate of a gtt token.
 * Buying a GTT Token in exchange for Ether
 * We do limit the number of tokens and user CappedCrowdsale
 */

contract GTTCrowdsale is Crowdsale, Ownable/*, FinalizableCrowdsale*/ {
  
  
  // Events
  event ForwardFunds(string log);

  // Constructor 
  // -------------
  function GTTCrowdsale(uint256 _rate, address _wallet, ERC20 _token) public
    Crowdsale(_rate, _wallet, _token)
  {
  }
  // -------------


  // Change the current rate
  // ------------------------
  /// @dev Function to set the rate of a GTTK token
  /// @param _rate rate to set
  function setCurrentRate(uint256 _rate) private {
      rate = _rate;
  }

  // Buy Token
  // -----------------
  function () external payable {
     // ForwardFunds("Buying the tokens");
      buyTokens(msg.sender);
  }

  // @dev Overriding forwardFunds to create custom payment
  function forwardFunds() internal {
  //  ForwardFunds("Function forwards funds inside gttcrowdsale called");
    wallet.transfer(msg.value);
  }

}