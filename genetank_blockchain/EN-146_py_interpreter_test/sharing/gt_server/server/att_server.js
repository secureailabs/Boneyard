import { Meteor } from 'meteor/meteor'
import { HTTP } from 'meteor/http'
import { EJSON } from 'meteor/ejson'

mType = {TYPE_RA_SIMPLE:5,
         TYPE_RA_SRESP:6}

var fs = require('fs')
    , path = require('path')
    , certFile = path.join(process.env.PWD, '.meteor/local/build/programs/server/assets/app','ssl/genetankai.crt') 
    , keyFile = path.join(process.env.PWD, '.meteor/local/build/programs/server/assets/app','ssl/genetankai.key')
    //, caFile = path.join('/dev/shm/platform/modelrel/gt_server/.meteor/local/build/programs/server/assets/app','ssl/genetankai.pem')
    //, request = require('request');

var get_report_options = {
    rejectUnauthorized : false,
    headers: {
        'Content-Type': 'application/json'
    },
    agentOptions: {
      cert: fs.readFileSync(certFile),
      key: fs.readFileSync(keyFile),
      //ca: fs.readFileSync(caFile)
    },
    json: true, // Automatically stringifies the body
    resolveWithFullResponse:true //Get the full response instead of just the body
}

//Use the Meteor package simple:rest to provide a Restful API. See details on: https://atmospherejs.com/simple/rest
//It is actually define a method with can be called remotely through HTTP Restful API.
Meteor.method("attestation", 
  function (body) {
    console.log(EJSON.stringify(body));
    var syncHTTP = Meteor.wrapAsync( HTTP.call )
    let rp_result=null;
    if (body.msgType === mType.TYPE_RA_SIMPLE) {
      get_report_options.body={"isvEnclaveQuote":body.quote}
      response = syncHTTP("POST", 
        'https://test-as.sgx.trustedservices.intel.com:443/attestation/sgx/v2/report',
        {npmRequestOptions:get_report_options});
      const report = {
          'Content-Length':response.headers['content-length'],
          'X-IASReport-Signature':response.headers['x-iasreport-signature'],
          'X-IASReport-Signing-Certificate':response.headers['x-iasreport-signing-certificate'],
          attestation_report:response.content
      }
      rp_result = {
          msgType:mType.TYPE_RA_SRESP,
          attestationResult:report
      }
    }
/*
      .catch(function (err) {
        // POST failed...
        console.log('Error:'+err)
        return {error:"Internal error"}
      });
*/
    else {
      this.setHttpStatusCode(401);
      rp_result = {error:"Unkown meesage type"}
    }
    return rp_result
  }, 
  {
    url: "attestation",
    httpMethod: "post"
  }
)
