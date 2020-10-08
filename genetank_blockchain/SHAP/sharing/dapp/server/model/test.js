import { Meteor } from 'meteor/meteor';
import { userCfgDb, getUserCfg, homeDir } from '../account/account.js';
import { userStatus, getUserSt } from '../account/acc_status.js';
import { endpoint, roles } from '/imports/api/enum.js'
import { msgType } from '/imports/api/msg_type.js'
import { Random } from 'meteor/random'
import { EJSON } from 'meteor/ejson'
import { reSetupSession } from '/server/model/enclave.js'
import { findRole } from '/server/model/enclave.js'
import { calculateECSign } from "../data/ecdsaSignature";
import {createDappPublicKey, computeDHKey} from "../computeDHKey";
import {aes_encrypt, aes_decrypt } from "../aes_encryption.js"
import {verifyAttestation} from "../data/attestation";
import { myContract } from "/server/smartcontract.js";
import { enclaveCheck } from "/server/model/enclave.js";
var fs = require('fs')

const crypto = require('crypto');
const hash = crypto.createHash('sha256');


/**
 * [sendData2EnclaveASync send the data in a json in the enclave asynchronously]
 * @param  {string} uid      the user id of the user on the dapp
 * @param  {string} pubKey   public key of the enclave
 * @param  {json} dataJson json containg the message to be sent to the enclave
 *
 * @return {error: err, result: response}          json containing error and the
 *                                                 response of the enclave
 */
export function sendData2EnclaveASync(uid,pubKey,dataJson) {
  var { error, cfg, userStIdx, enclaveIdx, sessionIdx }
  = enclaveCheck(uid, endpoint.RUNNER, pubKey, roles.DATA_SCIENTIST)
  if (undefined !== error)
  return ({ error: error });

  if(dataJson == null){
    dataJson = {
      session_id: userStatus[userStIdx].enclave_session[sessionIdx].id,
      msgType: msgType.RUN_MODEL,
      role: roles.DATA_SCIENTIST,
      name: 'tab.py',
      data: 'print \'abc\'\n'
      // data:'f = open(\'SAIL.txt\',\'w\')\r\n\nf.close()\r\n\r\nprint \'abc\''//f = open(\'SAIL.txt\',\'r\')\r\nstr = f.read()\r\nf.close()\r\n\r\nprint \'read and write\'\n'
    }
  }

  const url = cfg.enclaves[enclaveIdx].url+"/"+endpoint.RUNNER
  var json={error:"Fail to install model"}
  const secretKey = userStatus[userStIdx].enclave_session[sessionIdx].key
  const cypher = aes_encrypt(dataJson, secretKey)
  for(var i = 0; i < 3; i++){
    HTTP.call("POST", url, {
      headers: {
        session_id: userStatus[userStIdx].enclave_session[sessionIdx].id,
        aes_iv: cypher.iv
      },
      data: {
        session_id: userStatus[userStIdx].enclave_session[sessionIdx].id,
        aes_iv: cypher.iv,
        content: cypher.cypherText
      }
    }, function(error, result) {
      if (null !== error) {
        console.log(error)
      } else {
        session_id = parseInt(result.headers['session_id'])
        var ret
        if (0 === session_id) {
          ret = reSetupSession(userStIdx, sessionIdx, uid)
          if (undefined !== ret.error)
          console.log(ret.error)
          dataJson.session_id = userStatus[userStIdx].enclave_session[sessionIdx].id
        } else {
          const iv = result.headers['aes_iv']
          json = aes_decrypt(result.content, secretKey, iv)
          console.log("Completed a step",json)
        }
      }
    })
  }
  return ({error: null, result: "Test started successfully"})
}

/**
 * [testSessionASync populates an array of fucntions to be run after the session
 *                   is initialized]
 * @param  {string} uid     the user id of the user registered on the dapp
 * @param  {enum} endpnt    the end point of the user (can be endpoint.RUNNER,
 *                          endpoint.TRAINER, endpoint.QUERIER, endpoint.DONOR)
 * @param  {string} pubKey  the public key of the enclave
 * @param  {enum} role      the role of the user (can be roles.RUNNER, TRAINER, DONOR,
 *                          DATA_SCIENTIST, USER)
 * @param  {string} ethAddr ethereum address of the account being used
 *
 * @param  {number} testCode number whose binary bits will indicate which test is to be run
 *                  [eg. 1001 - run test number 0 and 3]
 * @return {error: err, result: response}  json containing error and the
 *                                         response of the enclave
 */
export function testSessionASync(uid, endpnt, pubKey, role,testCode){
  var arrayOfFunc = []
  if((testCode >> 1) % 2){
    arrayOfFunc.push(pyVMTest)
  }
  if((testCode >> 2) % 2){
    arrayOfFunc.push(dataUploadTest)
  }
	if((testCode >> 3) % 2){
		arrayOfFunc.push(getInfoTest)
	}
	console.log(arrayOfFunc)
  //runTest(uid, endpnt, pubKey, role,arrayOfFunc)
	return({error: null, result: "Test startted..."})
}


/**
 * [testSessionASync Asynchronously initializes new sessions and call
 * functions with different messages to the enclave through the created sessions]
 * @param  {string} uid     the user id of the user registered on the dapp
 * @param  {enum} endpnt    the end point of the user (can be endpoint.RUNNER,
 *                          endpoint.TRAINER, endpoint.QUERIER, endpoint.DONOR)
 * @param  {string} pubKey  the public key of the enclave
 * @param  {enum} role      the role of the user (can be roles.RUNNER, TRAINER, DONOR,
 *                          DATA_SCIENTIST, USER)
 * @param  {string} ethAddr ethereum address of the account being used
 *
 * @return {error: err, result: response}  json containing error and the
 *                                         response of the enclave
 */
export function runTest(uid, endpnt, pubKey, role,arrayOfFunc) {
  var cfg, userStIdx, enclaveIdx, sessionIdx, ethAddr
  var enclavePubKey = pubKey
  if (undefined === uid)
  console.log ({ error: "Login required."});
  cfg = getUserCfg(uid)
  if (undefined === cfg)
  console.log ({error: "Can't find user configuration."});
  var found1 = false
  for (enclaveIdx in cfg.enclaves)
  if (cfg.enclaves[enclaveIdx].pubKey === enclavePubKey) {
    found1 = true;
    enclave = cfg.enclaves[enclaveIdx]
    break
  }
  if (!found1)
  return ({ error: "Enclave not found"});
  if (undefined === ethAddr) {
    ethAddr = cfg.defaultEthAccount
  }

  var roleInfo = findRole(role, cfg.enclaves[enclaveIdx].roleInfos, ethAddr)

  found = false
  userStIdx = getUserSt(uid)
  sessionIdx = 0
  const sessions = userStatus[userStIdx].enclave_session
  for (sessionIdx in sessions)
  if ((sessions[sessionIdx].pubKey === pubKey) &&
  (roleInfo.ethAddr === sessions[sessionIdx].ethAccount) &&
  (roleInfo.role === sessions[sessionIdx].role)) {
    found = true;
    break;
  }
  var ret = {
    sessionIdx: sessionIdx
  }

  const url = cfg.enclaves[enclaveIdx].url + "/" + endpnt
  if (!verifyAttestation(enclave.attestationReport, enclave.attSignature, enclave.attSignCert)) {
    //return({error:"Attestation verification failed"})
  }

  var ecObj = createDappPublicKey();
  //var ecObj = createDappPublicKey("bff7b62b4480cbb033e2c442c757b30e49e194b28cdb4c61067007e3a2a65584");
  console.log("url:" + url)
  var dataJsonTmp = calculateECSign(roleInfo.ethAddr, ecObj.publicKey);
  for (var i = 0; i < 5; i++) {
    HTTP.call("POST", url, {
      headers: {
        session_id: 0
      },
      data: {
        session_id: 0,
        msgType: msgType.SESSION_INIT,
        role: roleInfo.role,
        stakeholderId: roleInfo.stId,
        pubKey: ecObj.publicKey,
        sign: dataJsonTmp.ethSign,
        ethPubKey: dataJsonTmp.publicKey //Eth public key
      }
    }, function(error, result) {
      // send call back to confirm the encalve creation
      var secretKey = computeDHKey(ecObj.ecInstance, enclavePubKey);

      var session_id = parseInt(result.headers['session_id'])
      if (0 === session_id)
      console.log({
        error: "Session setup request fail."
      })
      var iv = result.headers['aes_iv']
      var result_json = aes_decrypt(result.content, secretKey, iv)
      //result_json = EJSON.parse(result.content);
      if (undefined === result_json || undefined === result_json.session_id) {
        console.log({
          error: "Bad response."
        })
      }
      if (result_json.session_id === 0) {
        console.log({
          error: "Setup session request refused by the enclave."
        })
      }
      if (msgType.SESSION_INIT_RSP !== result_json.msgType) {
        console.log({
          error: "Bad response message type."
        })
      }
      console.log(result_json)
      var msg3 = {
        session_id: result_json.session_id,
        msgType: msgType.SESSION_CONFIRM
      }

      const cypher = aes_encrypt(msg3, secretKey)
      HTTP.call("POST", url, {
        headers: {
          session_id: result_json.session_id,
          aes_iv: cypher.iv
        },
        data: {
          session_id: result_json.session_id,
          aes_iv: cypher.iv,
          content: cypher.cypherText
        }
      }, function(error, result) {

        session_id = parseInt(result.headers['session_id'])
        if (0 === session_id)
        console.log ({
          error: "Session setup request fail."
        })
        iv = result.headers['aes_iv']
        var confirm_json = aes_decrypt(result.content, secretKey, iv)

        if (undefined === confirm_json || undefined === confirm_json.session_id) {
          console.log ({
            error: "Bad confirm response."
          })
        }
        if (confirm_json.session_id !== result_json.session_id) {
          return ({
            error: "Setup session confirm error."
          })
        }
        if (msgType.SESSION_CONFIRM_RSP !== confirm_json.msgType) {
          console.log ({
            error: "Bad confirm response message type."
          })
        }
        var idx //=  userStatus[userStIdx].enclave_session.length
        var found = false
        const sessions = userStatus[userStIdx].enclave_session
        for (idx in sessions)
        if ((enclavePubKey === sessions[idx].pubKey) &&
        (roleInfo.ethAddr === sessions[idx].ethAccount)
      ) {
        found = true
        //debugger;
        break;
      }
      if (!found)
      idx = sessions.length

      userStatus[userStIdx].enclave_session[idx] = ({
        pubKey: enclavePubKey,
        ethAccount: roleInfo.ethAddr,
        url: url,
        role: roleInfo.role,
        stId: roleInfo.stId,
        id: result_json.session_id,
        key: secretKey
      })
      for(var i = 0; i < arrayOfFunc.length; i++){
        arrayOfFunc[i](userStatus,userStIdx,idx,sessionIdx)
      }
      // pyVMTest(userStatus,userStIdx,idx,sessionIdx)
      // dataUploadTest(userStatus,userStIdx,idx,sessionIdx)
    })

  })
}
return ({error: null, result: "Test started successfully"})
}

/**
 * [pyVMTest sends commands to the pyvm asynchronously]
 @param  {json} userStatus   contains the details of the user
 * @param  {number} userStIdx  index of the user in the userstatus table
 * @param  {number} idx        index of the session in the enclave_session table
 *                             which the user is connected through
 * @param  {number} sessionIdx session Index of the user on the dapp
 *
 * @return {json}              message to notify the dapp and show the output of pyVM
 */
function pyVMTest(userStatus,userStIdx,idx,sessionIdx){
  var session_id = userStatus[userStIdx].enclave_session[idx].id
  var url = userStatus[userStIdx].enclave_session[idx].url
  var role = userStatus[userStIdx].enclave_session[idx].role
  var ethAddr = userStatus[userStIdx].enclave_session[idx].ethAccount
  var secretKey = userStatus[userStIdx].enclave_session[idx].key

  var dataJson = {
    session_id: userStatus[userStIdx].enclave_session[idx].id,
    msgType: msgType.RUN_MODEL,
    role: roles.DATA_SCIENTIST,
    name: 'tab.py',
    data: 'print \'abc\'\n'
  }

  const cypher = aes_encrypt(dataJson, secretKey)
  HTTP.call("POST", url, {
    headers: {
      session_id: userStatus[userStIdx].enclave_session[sessionIdx].id,
      aes_iv: cypher.iv
    },
    data: {
      session_id: userStatus[userStIdx].enclave_session[sessionIdx].id,
      aes_iv: cypher.iv,
      content: cypher.cypherText
    }
  }, function(error, result) {
    if (null !== error) {
      console.log(error)
    } else {
      session_id = parseInt(result.headers['session_id'])
      var ret
      if (0 === session_id) {
        ret = reSetupSession(userStIdx, sessionIdx, uid)
        if (undefined !== ret.error)
        console.log(ret.error)
        dataJson.session_id = userStatus[userStIdx].enclave_session[sessionIdx].id
      } else {
        const iv = result.headers['aes_iv']
        json = aes_decrypt(result.content, secretKey, iv)
        console.log("Completed a step",json)
      }
    }
  })
}

/**
 * [dataUploadTest Uploads data asynchronously to the enclaves for the previously generated sessions]
 * @param  {json} userStatus   contains the details of the user
 * @param  {number} userStIdx  index of the user in the userstatus table
 * @param  {number} idx        index of the session in the enclave_session table
 *                             which the user is connected through
 * @param  {number} sessionIdx session Index of the user on the dapp
 *
 * @return {json}              message to notify the dapp
 */
 function dataUploadTest(userStatus,userStIdx,idx,sessionIdx){
  var session_id = userStatus[userStIdx].enclave_session[idx].id
  var url = userStatus[userStIdx].enclave_session[idx].url
  var role = userStatus[userStIdx].enclave_session[idx].role
  var ethAddr = userStatus[userStIdx].enclave_session[idx].ethAccount
  var secretKey = userStatus[userStIdx].enclave_session[idx].key

  var path = homeDir + "/../../server/test/testUpload/"
  var fileName = "dummyData" + ((parseInt(session_id)%5)+1) + ".txt"
  var file = path + fileName
  const data = fs.readFileSync(file)
  var dataJson = {
    session_id: session_id,
    msgType: msgType.UPLOAD_DATA,
    role: roles.DONOR,
    dataName: fileName,
    ethAddress:ethAddr,
    data: data.toString('base64'),
  }
  /*
  send other commands :
  send data load

  */
  const cypher = aes_encrypt(dataJson, secretKey)
  HTTP.call("POST", url, {
    headers: {
      session_id: userStatus[userStIdx].enclave_session[sessionIdx].id,
      aes_iv: cypher.iv
    },
    data: {
      session_id: userStatus[userStIdx].enclave_session[sessionIdx].id,
      aes_iv: cypher.iv,
      content: cypher.cypherText
    }
  }, function(error, result) {
    if (null !== error) {
      console.log(error)
    } else {
      session_id = parseInt(result.headers['session_id'])
      var ret
      if (0 === session_id) {
        ret = reSetupSession(userStIdx, sessionIdx, uid)
        if (undefined !== ret.error)
        console.log(ret.error)
        dataJson.session_id = session_id
      } else {
        const iv = result.headers['aes_iv']
        json = aes_decrypt(result.content, secretKey, iv)
        console.log("Completed a step",json)
        if (json.error)
        console.log(json)
        if (msgType.UPLOAD_DATA_RSP_SUCCESS === json.msgType)
        console.log ({receipt:json.receipt})
        else
        console.log ({error:"Sharing data to enclave failed."})
      }
    }
  })
}


/**
 * [getInfoTest  get enclave information asynchronously for the previously generated sessions]
 * @param  {json} userStatus   contains the details of the user
 * @param  {number} userStIdx  index of the user in the userstatus table
 * @param  {number} idx        index of the session in the enclave_session table
 *                             which the user is connected through
 * @param  {number} sessionIdx session Index of the user on the dapp
 *
 * @return {json}              message to notify the dapp
 */
 function getInfoTest(userStatus,userStIdx,idx,sessionIdx){
  var session_id = userStatus[userStIdx].enclave_session[idx].id
  var url = userStatus[userStIdx].enclave_session[idx].url
  var role = userStatus[userStIdx].enclave_session[idx].role
  var ethAddr = userStatus[userStIdx].enclave_session[idx].ethAccount
  var secretKey = userStatus[userStIdx].enclave_session[idx].key
  var path
	var dataJson = {
      session_id:session_id,
      msgType:msgType.GET_INFO,
      role:roles.USER,
      ethAddress:ethAddr,
      path:path
    }
  const cypher = aes_encrypt(dataJson, secretKey)
  HTTP.call("POST", url, {
    headers: {
      session_id: userStatus[userStIdx].enclave_session[sessionIdx].id,
      aes_iv: cypher.iv
    },
    data: {
      session_id: userStatus[userStIdx].enclave_session[sessionIdx].id,
      aes_iv: cypher.iv,
      content: cypher.cypherText
    }
  }, function(error, result) {
		if (undefined === result)
        console.log ({error:"No result"})
      //session_id = result.headers['session_id']
      session_id = parseInt(result.headers['session_id'])
      var ret
      if (0 === session_id) {
        ret = reSetupSession(userStIdx, sessionIdx, uid)
        if (undefined !== ret.error)
          console.log ({error:ret.error})
        dataJson.session_id = userStatus[userStIdx].enclave_session[sessionIdx].id
      } else {
        const iv = result.headers['aes_iv']
        json = aes_decrypt(result.content, secretKey, iv)
      }
		if (json.error)
      console.log(json)
    if (msgType.GET_INFO_RSP_SUCCESS === json.msgType)
      console.log ({result:json.result})
    else
     	console.log ({error:"Get enclave information responses failed."})
  })
}
