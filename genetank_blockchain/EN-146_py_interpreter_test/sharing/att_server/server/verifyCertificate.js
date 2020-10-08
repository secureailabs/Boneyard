let pem = require("pem");
let fs = require("fs");
let path = require('path');
const crypto = require('crypto');

function processRootCert(pemFile){
    let text = fs.readFileSync(pemFile).toString();
    return text;
}

function processCertChain(chain){
    chain = chain.replace(/%20/g, " ");
    chain = chain.replace(/%0A/g, "\n");
    chain = chain.replace(/%2B/g, "+");
    chain = chain.replace(/%3D/g, "=");

    chainStack = chain.replace(/-----END CERTIFICATE-----\n/g, "-----END CERTIFICATE-----\n,").split(",").slice(0,-1);

    return chainStack;
}

function verifyCertChain(rootCert, certChain){
    let Future = Npm.require('fibers/future');
    let res = new Future();
    pem.verifySigningChain(rootCert,certChain,(error,result) => {
    	if(!error){
    		console.log("chain verification result", result);
        res.return(true);
    	}
    	else{
    		console.log("error", error);
        res.return(false);
    	}
    });
    return res.wait();
}

function verifySignature(certChain, content, signature){
    const verify = crypto.createVerify('RSA-SHA256');
    verify.update(content);
    return verify.verify(certChain, signature, 'base64');
}

export function verifyAttestationCert(certChain, content, signature){
    rootCert = processRootCert(path.join(process.env.PWD, '.meteor/local/build/programs/server/assets/app','ssl/Intel_SGX_Attestation_RootCA.pem'));
    cleanedChain = processCertChain(certChain);
    if(verifyCertChain(rootCert, cleanedChain)){
        if(verifySignature(cleanedChain[0], content, signature)){
            return true;
        }
        return false;
    }
    return false;
}
