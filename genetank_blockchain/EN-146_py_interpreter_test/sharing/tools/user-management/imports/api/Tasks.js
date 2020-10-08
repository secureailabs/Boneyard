import { Mongo } from "meteor/mongo";

export const user_data = new Mongo.Collection('user_data');

/*
import './buffer.js';
import bip39 from 'bip39';
import bitcoin from 'bitcoinjs-lib';
import cryptico from 'cryptico';
import encryption from 'sodium-encryption';

import crypto from 'crypto';

//export const


Meteor.methods({
    'signup.generateWords'() {
        console.log('Generating words...');
        var mnemonic = bip39.generateMnemonic();   
        return mnemonic;
    },
    'signup.createKeys'(mnemonic, user_name, password) {
       // console.log('mnemonic server:', mnemonic);

        
        const salt_password = crypto.randomBytes(16).toString('hex');
        const key_password = Buffer(crypto.pbkdf2Sync(password, salt_password, 2048, 16, 'sha512').toString('hex'));
       
        const salt_mnemonic = crypto.randomBytes(16).toString('hex');
        const key_mnemonic = Buffer(crypto.pbkdf2Sync(mnemonic, salt_mnemonic, 2048, 16, 'sha512').toString('hex'));

       
        // let mnemonic = 'message foot patrol now film mansion spy wide bean latin unhappy toe'
       //console.log(encr)
       var nonce_mnemonic = encryption.nonce();
       var mnemonic_buffer = Buffer(mnemonic);
      // console.log("key_password:", key_password);
      // console.log("encryption.key():", encryption.key());
       var encrypted_mnemonic = encryption.encrypt(mnemonic_buffer, nonce_mnemonic, key_password)
      // console.log('encrypted mnemonic is:', encrypted_mnemonic)
      // console.log('decrypted mnemonic is:', encryption.decrypt(encrypted_mnemonic, nonce_mnemonic, key_password).toString())


      // var key = Buffer(mnemonic);//'mnemonic';//encryption.key();
       var nonce_password = encryption.nonce();
       var password_buffer = Buffer(password);
       var encrypted_password = encryption.encrypt(password_buffer, nonce_password, key_mnemonic)
      // console.log('encrypted password is:', encrypted_password)
      // console.log('decrypted password is:', encryption.decrypt(encrypted_password, nonce_password, key_mnemonic).toString())


       // insert the following to the mongodb
       //username, encrypted_words, encrypted_password, encrypted_private_key
       // user_data.insert({username: user_name, mnemonic: mnemonic});
       user_data.insert({ username: user_name,
                          saltpass: salt_password, 
                          keypass: key_password, 
                          noncepass: nonce_password, 
                          saltmnem: salt_mnemonic, 
                          keymnem: key_mnemonic, 
                          noncemnem: nonce_mnemonic}, function( error, result) { 
                            if ( error ) console.log ( 'error:', error ); //info about what went wrong
                            if ( result ) console.log ('result:', result ); //the _id of new object if successful
                           }); 
       console.log("db contains:");
       //console.log(user_data.find({saltpass: salt_password}));
       console.log(user_data.find({}).fetch());
       var user = user_data.find({}).count();
       console.log(user);

       console.log("END");

        if( !bip39.validateMnemonic(mnemonic)) {
            return 'Registration Failed';
        }

        let seed = bip39.mnemonicToSeed(mnemonic)
        console.log('seed:', seed);

       
        var Bits = 1024; 
        var MattsRSAkey = cryptico.generateRSAKey(seed, Bits);
        //KyzPuynDQUTSK23YnhNDCaeFfoZ7GBNM6aLQ65KaZ7VUMaLy7K7N
        console.log("MattsRSAkey:", MattsRSAkey);
        var MattsPublicKeyString = cryptico.publicKeyString(MattsRSAkey);  
        console.log("MattsPublicKeyString:", MattsPublicKeyString);

        var bitcoinNetwork = bitcoin.networks.bitcoin;
        var hdMaster = bitcoin.HDNode.fromSeedBuffer(seed, bitcoinNetwork);

        var key1 = hdMaster.derivePath('m/0');
        var key2 = hdMaster.derivePath('m/1');

        console.log(key1.keyPair.toWIF());
        console.log('public key buffer:', key1.keyPair.getPublicKeyBuffer());

        var publicKey = bitcoin.ECPair.fromPublicKeyBuffer(key1.keyPair.getPublicKeyBuffer())
        console.log("publickey:", publicKey);

        console.log(key2.keyPair.toWIF());
        return "testing here";

    },
});
*/