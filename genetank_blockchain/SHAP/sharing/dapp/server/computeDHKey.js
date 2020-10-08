const crypto = require("crypto");
const ethUtil = require("ethereumjs-util");

export function createDappPublicKey(priv) {
  const dappECInstance = crypto.createECDH("secp256k1");
  let dappKeyBuffer;
  if (undefined === priv) dappKeyBuffer = dappECInstance.generateKeys();
  else {
    dappECInstance.setPrivateKey(priv, "hex");
    dappKeyBuffer = dappECInstance.getPublicKey();
  }
  return {
    ecInstance: dappECInstance,
    publicKey: dappKeyBuffer.toString("hex")
  };
}

export function computeDHKey(ecInstance, enclavePubKey) {
  enclavePubKey = "04" + enclavePubKey;
  var enclavePubKeyBuffer = Buffer.from(enclavePubKey, "hex");
  const secretKey = ecInstance.computeSecret(enclavePubKeyBuffer);
  var secretKeyHash =
    "0x" + ethUtil.keccak256(secretKey.toString("hex")).toString("hex");
  var dhKey = secretKeyHash.substring(2, 2 * 16 + 2);

  return dhKey;
}
