pragma solidity ^0.4.18;

import './MintableToken.sol';

/**
 * @title GTTK Token
 * @dev Very simple ERC20 Token that can be minted.
 * To be used to sell GTTK token in exchange of ether
 */
contract GTTToken is MintableToken {
  string public name = "GTT Token";
  string public symbol = "GTTK";
  uint8 public decimals = 18;
}