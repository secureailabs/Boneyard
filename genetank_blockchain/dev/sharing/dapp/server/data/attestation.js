import { Meteor } from 'meteor/meteor';
const crypto = require('crypto');

const verify = crypto.createVerify('RSA-SHA256');


export function verifyAttestation(data, signature, certificate) {
        var pem  =require("pem");

        if(certificate === undefined || data === undefined || signature === undefined)
            return false;

        certificate = certificate.replace(/%20/g, " ");
        certificate = certificate.replace(/%0A/g, "\n");
        certificate = certificate.replace(/%2B/g, "+");
        certificate = certificate.replace(/%3D/g, "=");

        // pem.verifySigningChain(cert6, cert7, (error, result) => {
        //     if(!error) {
        //         console.log(result);
        //     }
        //     else{
        //         console.log(error);
        //     }
        // })

        verify.update(data);
        return verify.verify(certificate, signature, 'base64')
}