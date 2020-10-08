import { Meteor } from "meteor/meteor";

const fs = require('fs');
const encryption = require('sodium-encryption');

// get user's encryption key
const secret = Buffer.alloc(32, 'temp');
let nonce = Buffer.alloc(24, "temp");

Meteor.methods({

    save_data: function (file, data) {

        data = Buffer.from(data);
        let enc_data = encryption.encrypt(data, nonce, secret);

        fs.writeFile(file, enc_data, (err) => {
            if (err) {
                console.error("Error saving data! " + err);
            } else {
                console.log("Success saving data! " + encryption.decrypt(enc_data, nonce, secret));
            }
        });

    },

    read_data: function (file) {

        let data = fs.readFileSync(file, 'utf8');
        let buffered = Buffer.from(data);

        let decrypted = encryption.decrypt(buffered, nonce, secret);    // THIS STEP DOESN'T WORK! TODO: FIX

        console.log("result: " + decrypted);
        return decrypted;
    }

});