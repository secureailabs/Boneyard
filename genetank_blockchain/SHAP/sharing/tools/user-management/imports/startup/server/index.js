import { Mongo } from "meteor/mongo";

import { user_data } from '../../api/Tasks';
import '../../api/buffer.js';
import bip39 from 'bip39';
import bitcoin from 'bitcoinjs-lib';
import encryption from 'sodium-encryption';

import crypto from 'crypto';

//@TODO If you have more than one accounts need to iterate over each to show what to 

async function decrypt(secret, salt, nonce, encrypted_data) {
  
    const key = Buffer(crypto.pbkdf2Sync(secret, salt, 2048, 16, 'sha512').toString('hex'))

    console.log("key:", key);

    var decr_mnemonic = encryption.decrypt(encrypted_data, nonce, key).toString()
    console.log("decrypted menmeonc:", decr_mnemonic)

    return decr_mnemonic;
}

Meteor.methods({
  generateWords() {
    console.log('Generating words...');
    var mnemonic = bip39.generateMnemonic();   
    return mnemonic;
  },

  async recoverMnemonic(user_name, secret) {
    console.log("user_name:", user_name)
    var user = user_data.find({username: user_name},{mnemonic:1, noncemnem:1, saltmnem:1}).fetch()
    console.log("user count:", user.length)

    var recovered_mnemonic_list = []

    for (var i = 0; i < user.length; i++) {
      //alert(myStringArray[i]);
      var nonce = new Buffer(user[i].noncemnem, 'hex')
      var salt = user[i].saltmnem
      var encrypted_mnemonic = new Buffer(user[i].mnemonic, 'hex')
  
      console.log("secret:", secret)
      console.log("nonce:", nonce)
      console.log("salt:", salt)
      console.log("encrypted_mnemonic:", encrypted_mnemonic)
  
      var decrypted_mnemonic = await decrypt(secret, salt, nonce, encrypted_mnemonic)
      if( !bip39.validateMnemonic(decrypted_mnemonic)) {
        
        //recovered_mnemonic_list.push(decrypted_mnemonic)
      } else {
        //return decrypted_mnemonic
        recovered_mnemonic_list.push(decrypted_mnemonic)
      }
    }
    console.log("recovered_mnemonic_list:",recovered_mnemonic_list)
    if( 0 == recovered_mnemonic_list.length) {
      recovered_mnemonic_list.push('Failed to recover mnemonic')
    } else {
      return recovered_mnemonic_list
    }

   

    /* var nonce = new Buffer(user[0].noncemnem, 'hex')
    var salt = user[0].saltmnem
    var encrypted_mnemonic = new Buffer(user[0].mnemonic, 'hex')

    console.log("secret:", secret)
    console.log("nonce:", nonce)
    console.log("salt:", salt)
    console.log("encrypted_mnemonic:", encrypted_mnemonic)

    var decrypted_mnemonic = await decrypt(secret, salt, nonce, encrypted_mnemonic)
    if( !bip39.validateMnemonic(decrypted_mnemonic)) {
      return 'Failed to recover mnemonic'
    } else {
      return decrypted_mnemonic
    }*/

  },

  async recoverPassword(user_name, secret) {
    console.log("user_name:", user_name);
    var user = user_data.find({username: user_name},{password: 1, noncepass: 1, saltpass: 1}).fetch()
    var nonce = new Buffer(user[0].noncepass, 'hex')
    var salt = user[0].saltpass
    var encrypted_password = new Buffer(user[0].password, 'hex')

    console.log("secret:", secret)
    console.log("nonce:", nonce)
    console.log("salt:", salt)
    console.log("encrypted_password:", encrypted_password)

    return await decrypt(secret, salt, nonce, encrypted_password)
  },

  createKeys(mnemonic, user_name, password, confirm_password) {
    if(password != confirm_password) {
      return 'Password not the same'
    }

    console.log('mnemonic server:', mnemonic);
    
    console.log("password:", password);
    const salt_for_mnemonic = crypto.randomBytes(16).toString('hex');
    const key_for_mnemonic = Buffer(crypto.pbkdf2Sync(password, salt_for_mnemonic, 2048, 16, 'sha512').toString('hex'));
    console.log("key_for_mnemonic:", key_for_mnemonic);
    console.log("salt_for_mnemonic:", salt_for_mnemonic);
    var nonce_for_mnemonic = encryption.nonce();
    console.log("nonce_for_mnemonic:", nonce_for_mnemonic);
    var buffer_of_mnemonic = Buffer(mnemonic);
    console.log("buffer_of_mnemonic:", buffer_of_mnemonic);
    var encrypted_mnemonic = encryption.encrypt(buffer_of_mnemonic, nonce_for_mnemonic, key_for_mnemonic)
    console.log("encrypted_mnemonic:", encrypted_mnemonic);

    const salt_for_password = crypto.randomBytes(16).toString('hex');
    const key_for_password = Buffer(crypto.pbkdf2Sync(mnemonic, salt_for_password, 2048, 16, 'sha512').toString('hex'));
    var nonce_for_password = encryption.nonce();
    var buffer_of_password = Buffer(password);
    var encrypted_password = encryption.encrypt(buffer_of_password, nonce_for_password, key_for_password)
   

    if( !bip39.validateMnemonic(mnemonic)) {
      return 'Registration Failed';
    }

    let seed = bip39.mnemonicToSeed(mnemonic)
    console.log('seed:', seed);

    var bitcoinNetwork = bitcoin.networks.bitcoin;
    var hdMaster = bitcoin.HDNode.fromSeedBuffer(seed, bitcoinNetwork);

    var private_key = hdMaster.derivePath('m/0');
    var private_address = Buffer(private_key.getAddress());

   // console.log(key1.keyPair.toWIF());
   // console.log('public key buffer:', key1.keyPair.getPublicKeyBuffer());

    var public_key = bitcoin.ECPair.fromPublicKeyBuffer(private_key.keyPair.getPublicKeyBuffer())
    var public_address = public_key.getAddress();

    var encrypted_private_key = encryption.encrypt(private_address, nonce_for_mnemonic, key_for_mnemonic)

    var user_insert_data = {
        username: user_name,
        saltpass: salt_for_password, 
       // keypass: key_password, 
        password: encrypted_password.toString('hex'),
        mnemonic: encrypted_mnemonic.toString('hex'),
        noncepass: nonce_for_password.toString('hex'), 
        saltmnem: salt_for_mnemonic, 
      //  keymnem: key_mnemonic, 
        noncemnem: nonce_for_mnemonic.toString('hex'),
        publickey: public_address,
        privatekey: encrypted_private_key.toString('hex')
    };

   // var array = [user_insert_data];
    user_data.insert(user_insert_data, function( error, result) { 
        if ( error ) console.log ( 'error insert:', error ); //info about what went wrong
        if ( result ) console.log ('result insert:', result ); //the _id of new object if successful
    }); 

 /*   var user = user_data.find({username:'smu@cykra.com'},{mnemonic:1, noncemnem:1, saltmnem:1}).fetch();
    var nonce = user[0].noncemnem;
    var salt = user[0].saltmnem;

    const key_mnemonic2 = Buffer(crypto.pbkdf2Sync(mnemonic, salt, 2048, 16, 'sha512').toString('hex'));

    console.log("2 key_mnemonic:", key_mnemonic2);
    console.log("2 nonce_mnemonic:", nonce);
    console.log("2 salt_mnem:", salt);

    var encrypted_mnemonic = user[0].mnemonic;
*/
   // console.log("db contains:");
    //console.log(user_data.find({saltpass: salt_password}));
  //  console.log(user_data.find({}).fetch());
 //   var user = user_data.find({}).count();
 //   console.log(user);

  //  console.log("END");

    return "testing here";

 }
});

Meteor.publish('tasks', () => {
  return user_data.find();
});

