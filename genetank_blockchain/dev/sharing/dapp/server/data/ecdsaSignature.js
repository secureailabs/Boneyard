import { _getPassword, getEthSign } from "../account/EthAccountService.js";

export function calculateECSign(ethereumAddress, message, uid) {
  const result = _getPassword(ethereumAddress, uid);
  if (result.error) return { error: result.error };

  const ethPassword = result.password;

  const ethUtil = require("ethereumjs-util");
  const ethSign = getEthSign(ethereumAddress, ethPassword, message);

  const messageHash = ethUtil.keccak256(
    "\x19Ethereum Signed Message:\n" + message.length + message
  );
  const signVRS = ethUtil.fromRpcSig(ethSign);
  const publicKeyBuffer = ethUtil.ecrecover(
    messageHash,
    signVRS.v,
    signVRS.r,
    signVRS.s
  );
  const publicKey = ethUtil.bufferToHex(publicKeyBuffer);

  const newPublicKey = "0x04" + publicKey.substring(2);
  const msgHash = messageHash.toString("hex");

  var dataJson = {
    ethAddress: ethereumAddress,
    msgHash: msgHash,
    ethSign: ethSign,
    publicKey: newPublicKey
  };

  return dataJson;
}
