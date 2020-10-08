//var assert = require('assert');
var assert = require('chai').assert
var expect = require('chai').expect
const crypto = require('crypto');

import { aes_encrypt, aes_decrypt } from '/server/aes_encryption.js'
var key = crypto.randomBytes(16);
var iv = crypto.randomBytes(16);


describe('AES encryption function test', function () {
 it('AES encryption with a provided IV', function () {
        var cipherText = aes_encrypt({num:23323, str:"hello"}, key);
        assert.typeOf(cipherText, 'object');
        expect(cipherText.iv).not.equal(undefined);
        var json = aes_decrypt(cipherText.cypherText, key, cipherText.iv);
        assert.typeOf(json, 'object');
        expect(json.num).equal(23323);
        expect(json.str).equal("hello");
    });
 it('AES encryption with a generated IV', function () {
        var cipherText = aes_encrypt({num:23323, str:"hello"}, key, iv);
        assert.typeOf(cipherText, 'object');
        expect(cipherText.iv).equal(iv.toString('hex'));
        var json = aes_decrypt(cipherText.cypherText, key, cipherText.iv);
        assert.typeOf(json, 'object');
        expect(json.num).equal(23323);
        expect(json.str).equal("hello");
    });
});
