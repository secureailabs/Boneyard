import { Meteor } from 'meteor/meteor'
import { EJSON } from 'meteor/ejson'
import { userCfgDb, getUserCfg } from './account/account.js'
import { initRunnerInfoJson } from '/imports/api/usercfg.js'
import { myContract } from "/server/smartcontract.js";
import { handleJupyterReq } from "/server/model/cluster.js";

const enclaveList = new Mongo.Collection("enclaveList");

import { msgType } from '/imports/api/msg_type.js'

var get_report_options = {
    rejectUnauthorized : false,
    headers: {
        'Content-Type': 'application/json'
    },
    agentOptions: {
      //cert: fs.readFileSync(certFile),
      //key: fs.readFileSync(keyFile),
      //ca: fs.readFileSync(caFile)
    },
    json: true, // Automatically stringifies the body
    resolveWithFullResponse:true //Get the full response instead of just the body
}

Meteor.method("enclaveKernel",
  function (body) {
    //console.log(EJSON.stringify(body));
    const resp = handleJupyterReq(body);//runKernelCode(body)
    return resp
  },
  {
    url: "enclaveKernel",
    httpMethod: "post"
  }
);

//Use the Meteor package simple:rest to provide a Restful API. See details on: https://atmospherejs.com/simple/rest
//It is actually define a method with can be called remotely through HTTP Restful API.
Meteor.method("dapp_endpoint", 
  function (body) {
    console.log(EJSON.stringify(body));
    let rp_result={
      msgType:msgType.TYPE_DAPP_REG_RSP,
      result:msgType.DAPP_RESULT_NO_MATCH,
      runnerEth:"",
      trainerEth:"",
    };
    if (body.msgType === msgType.TYPE_DAPP_REG) {
      const usr = Meteor.users.findOne({username:body.username})
      if (undefined === usr || null === usr){
         rp_result.result = msgType.DAPP_RESULT_NO_USER;
         return rp_result;
      }
      //const cfg = userCfgDb.findOne({_id:usr._id});
      const cfg = getUserCfg(usr._id);
      if (undefined === cfg || null === cfg || undefined === cfg.enclaves){
         rp_result.result = msgType.DAPP_RESULT_NO_CFG
         return rp_result;
      }
      var found = false;
      var i;
      if (0 === cfg.enclaves.length) {
         rp_result.result = msgType.DAPP_RESULT_NO_ENCLAVE;
      } else {
        for (i in cfg.enclaves)
          if (cfg.enclaves[i].pubKey === body.pubKey) {
            found = true;
            break;
          }
      }
      if (!found){
         rp_result.result = msgType.DAPP_RESULT_NO_MATCH;
         i = cfg.enclaves.length
         cfg.enclaves.push({infoJson:EJSON.stringify(initRunnerInfoJson),
            pubKey:body.pubKey,
            model_id:"",
            url:body.enclaveUrl,
            ruleInfos:[],
          })
      } else {
        var ethAccount
        const roleInfos = cfg.enclaves[i].roleInfos
        if (undefined !== roleInfos)
          ethAccount = roleInfos[0].ethAddr;
        if (undefined === ethAccount || "" === ethAccount){
          cfg.enclaves[i].infoJson=EJSON.stringify(initRunnerInfoJson)
          cfg.enclaves[i].model_id=""
          rp_result.result = msgType.DAPP_RESULT_NO_MATCH;
          return rp_result
        } else if (undefined === cfg.enclaves[i].roleInfos){
          rp_result.result = msgType.DAPP_RESULT_NO_MATCH;
          return rp_result
        } else {
          rp_result.runnerEth = ethAccount;
          rp_result.trainerEth = cfg.enclaves[i].roleInfos[1].ethAddr;
          rp_result.result = msgType.DAPP_RESULT_SUCCESS;
        }
        cfg.enclaves[i].url = body.enclaveUrl;
        var enclaveEntry = enclaveList.findOne({pubKey: body.pubKey});
        if (enclaveEntry === undefined || enclaveEntry ==  null) {
            let enclaveInfo = cfg.enclaves[i];
            enclaveInfo['group'] = body.username;
            enclaveInfo['roleInfos'] = cfg.enclaves[i].roleInfos
            enclaveList.insert(enclaveInfo)
            try {
                let enclave = enclaveList.findOne({pubKey: body.pubKey});
                try{
                    enclave["username"] = Meteor.users.findOne({_id: cfg._id}).username;
                    enclave["timestamp"] =  new Date().getTime();
                    //registerEnclaveByCreator(enclave.roleInfos[0].ethAddr, body.pubKey, enclave, usr._id)
                }
                catch (e) {
                    console.log("Registration on Blockchain Failed")
                    rp_result.result = msgType.DAPP_RESULT_NO_MATCH;
                }
            }catch (e) {
                console.log("No Eth Addresses assigned to the enclave.")
            }
        }
      }
      userCfgDb.update({_id:usr._id},{$set:{enclaves:cfg.enclaves}});

    }
    else {
      this.setHttpStatusCode(401);
      rp_result = {error:"Unkown meesage type"}
    }
    return rp_result
  }, 
  {
    url: "dapp_endpoint",
    httpMethod: "post"
  }
)


Meteor.method("enclaveRegistry",
    function () {
        return enclaveList.find().fetch()
    },
    {
        url: "enclaveRegistry",
        httpMethod: "get"
    }
);


Meteor.method("veifySign",
    function (body) {
        console.log(EJSON.stringify(body));
        //var contractAddress = "0x22c259a106575d4c4dd93d6aecb3736a2631352d";
        //var myContract = new web3.eth.Contract(abiArray,contractAddress);

        var Future = Npm.require('fibers/future');
        var result = new Future();

        myContract.methods.verify(body.msgHash, body.sign.v, body.sign.r, body.sign.s, body.ethAddress).call({from: ethAccount})
            .then((response) => {
                result.return(response);
            })
        return result.wait();
    }
    );
