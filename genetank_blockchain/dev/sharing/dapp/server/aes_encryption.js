const crypto = require("crypto");

export function aes_encrypt(msg, key, iv) {
  let text = JSON.stringify(msg);
  const IV_LENGTH = 16;
  if (undefined === iv) iv = crypto.randomBytes(IV_LENGTH);
  let cipher = crypto.createCipheriv(
    "aes-128-ctr",
    Buffer.from(key, "hex"),
    iv
  );
  let encrypted = cipher.update(text);

  return { cypherText: encrypted.toString("base64"), iv: iv.toString("hex") };
}

export function aes_decrypt(msg, key, iv) {
  if ("" === iv) return { error: msg };
  let decipher = crypto.createCipheriv(
    "aes-128-ctr",
    Buffer.from(key, "hex"),
    Buffer.from(iv, "hex")
  );
  let decrypted = decipher.update(msg, "base64");
  var jsonObj = JSON.parse(decrypted.toString());

  return jsonObj;
}
