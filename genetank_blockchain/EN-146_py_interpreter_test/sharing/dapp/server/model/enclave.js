import { Meteor } from "meteor/meteor";
import { userCfgDb, getUserCfg, validateUserRole } from "../account/account.js";
import { userStatus, getUserSt } from "../account/acc_status.js";

import { endpoint, roles } from "/imports/api/enum.js";
import { msgType, msgName } from "/imports/api/msg_type.js";

import { EJSON } from "meteor/ejson";

import { calculateECSign } from "../data/ecdsaSignature";
import { createDappPublicKey, computeDHKey } from "../computeDHKey";
import { aes_encrypt, aes_decrypt } from "../aes_encryption.js";
import { verifyAttestation } from "../data/attestation";
import { myContract } from "/server/smartcontract.js";

function setupSession(
  url,
  enclavePubKey,
  userStIdx,
  roleInfo,
  uid,
  sessionIdx
) {
  syncHttp = Meteor.wrapAsync(HTTP.call);

  const enclave = getEnclaveByKey(enclavePubKey);

  if (undefined === enclave) return { error: "Enclave not found" };

  if (
    !verifyAttestation(
      enclave.attestationReport,
      enclave.attSignature,
      enclave.attSignCert
    )
  ) {
    //return({error:"Attestation verification failed"})
  }

  var ecObj = createDappPublicKey();
  console.log("url:" + url);
  var dataJsonTmp = calculateECSign(roleInfo.ethAddr, ecObj.publicKey, uid);

  var result, error;
  try {
    result = syncHttp("POST", url, {
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
    });
  } catch (err) {
    error = JSON.stringify(err);
  }

  if (error) return { error: error };

  var secretKey = computeDHKey(ecObj.ecInstance, enclavePubKey);

  var session_id = parseInt(result.headers["session_id"]);
  if (0 === session_id) return { error: "Session setup request fail. Re-init enclave. Retry!!" };
  var iv = result.headers["aes_iv"];
  var result_json = aes_decrypt(result.content, secretKey, iv);
  //result_json = EJSON.parse(result.content);
  if (undefined === result_json || undefined === result_json.session_id) {
    return { error: "Bad response." };
  }
  if (result_json.session_id === 0) {
    return { error: "Setup session request refused by the enclave." };
  }
  if (msgType.SESSION_INIT_RSP !== result_json.msgType) {
    return { error: "Bad response message type." };
  }

  var msg3 = {
    session_id: result_json.session_id,
    msgType: msgType.SESSION_CONFIRM
  };
  const cypher = aes_encrypt(msg3, secretKey);
  var result, error;
  try {
    result = syncHttp("POST", url, {
      headers: {
        session_id: result_json.session_id,
        aes_iv: cypher.iv
      },
      data: {
        session_id: result_json.session_id,
        aes_iv: cypher.iv,
        content: cypher.cypherText
      }
    });
  } catch (err) {
    error = JSON.stringify(err);
  }

  if (error) return { error: error };
  session_id = parseInt(result.headers["session_id"]);
  if (0 === session_id) return { error: "Session setup request fail." };
  iv = result.headers["aes_iv"];
  var confirm_json = aes_decrypt(result.content, secretKey, iv);

  if (undefined === confirm_json || undefined === confirm_json.session_id) {
    return { error: "Bad confirm response." };
  }
  if (confirm_json.session_id !== result_json.session_id) {
    return { error: "Setup session confirm error." };
  }
  if (msgType.SESSION_CONFIRM_RSP !== confirm_json.msgType) {
    return { error: "Bad confirm response message type." };
  }
  var idx; //=  userStatus[userStIdx].enclave_session.length
  const sessions = userStatus[userStIdx].enclave_session;
  if (undefined === sessionIdx) {
    var found = false;
    for (idx in sessions)
      if (
        enclavePubKey === sessions[idx].pubKey &&
        roleInfo.ethAddr === sessions[idx].ethAccount
      ) {
        found = true;
        break;
      }
    if (!found) idx = sessions.length;
  } else {
    if (undefined === sessions[sessionIdx]) idx = sessionIdx;
    else if (undefined === sessions[sessionIdx].pubKey) idx = sessionIdx;
    else if (enclavePubKey !== sessions[sessionIdx].pubKey)
      return { error: "Session table entry is occupied" };
    else idx = sessionIdx;
  }

  userStatus[userStIdx].enclave_session[idx] = {
    pubKey: enclavePubKey,
    ethAccount: roleInfo.ethAddr,
    url: url,
    role: roleInfo.role,
    stId: roleInfo.stId,
    id: result_json.session_id,
    key: secretKey
    //iv:iv
  };

  return { sessionIdx: idx };
}

function reSetupSession(userStIdx, sessionIdx, uid) {
  const session = userStatus[userStIdx].enclave_session[sessionIdx];
  const roleInfo = {
    role: session.role,
    stId: session.stId,
    ethAddr: session.ethAccount
  };
  console.log("stId: ");
  console.log(roleInfo);
  return setupSession(session.url, session.pubKey, userStIdx, roleInfo, uid);
}

export function findRole(role, roleInfos, ethAddr) {
  var roleInfo = {
    ethAddr: ethAddr,
    role: role
  };
  if (undefined === roleInfos) return roleInfo;
  const ri = roleInfos.find(ri => ri.ethAddr === ethAddr);
  if (undefined !== ri) {
    roleInfo = ri;
  }
  return roleInfo;
}

export function enclaveCheck(uid, endpnt, pubKey, role, ethAddr) {
  var userStIdx, sessionIdx;
  if (undefined === uid) return { error: "Login required." };
  const enclave = getEnclaveByKey(pubKey);
  if (undefined === enclave) return { error: "Enclave cannot be found." };
  var roleInfo = findRole(role, enclave.roleInfos, ethAddr);

  found = false;
  userStIdx = getUserSt(uid);
  sessionIdx = 0;
  const sessions = userStatus[userStIdx].enclave_session;
  for (sessionIdx in sessions) {
    if (
      sessions[sessionIdx].pubKey === pubKey &&
      ethAddr === sessions[sessionIdx].ethAccount
    ) {
      found = true;
      break;
    }
  }
  var ret = { sessionIdx: sessionIdx };
  if (!found) {
    const url = enclave.url + "/" + endpnt;
    ret = setupSession(url, pubKey, userStIdx, roleInfo, uid);
    if (undefined !== ret.error) return { error: ret.error };
  }
  return { userStIdx: userStIdx, enclave: enclave, sessionIdx: ret.sessionIdx, stId: roleInfo.stId };
}

export function sendData2Enclave(uid, enclave, sessionIdx, dataJson) {
  const userStIdx = getUserSt(uid);
  const url = enclave.url + "/" + endpoint.RUNNER;
  var json = { error: "Fail to install model" };
  const syncHttp = Meteor.wrapAsync(HTTP.call);
  for (var i in [0, 1, 2]) {
    //retry max 3 times
    const secretKey = userStatus[userStIdx].enclave_session[sessionIdx].key;
    const cypher = aes_encrypt(dataJson, secretKey);
    var result, error;
    try {
      var result = syncHttp("POST", url, {
        headers: {
          session_id: dataJson.session_id,
          aes_iv: cypher.iv
        },
        data: {
          session_id: dataJson.session_id,
          aes_iv: cypher.iv,
          content: cypher.cypherText
        }
      });
    } catch (err) {
      error = err;
    }
    if (error) return { error: error };

    session_id = parseInt(result.headers["session_id"]);
    var ret;
    if (0 === session_id) {
      ret = reSetupSession(userStIdx, sessionIdx, uid);
      if (undefined !== ret.error) return { error: ret.error };
      dataJson.session_id =
        userStatus[userStIdx].enclave_session[sessionIdx].id;
    } else {
      const iv = result.headers["aes_iv"];
      json = aes_decrypt(result.content, secretKey, iv);
      break;
    }
  }
  return json;
}

export function getEnclavesByHost(userId) {
  const adminUserId = Meteor.users.findOne({ username: "admin" })._id;
  var cfg = userCfgDb.findOne({ _id: adminUserId });
  if (undefined === cfg || null === cfg) return [];
  const user = Meteor.users.findOne({ _id: userId });
  if (undefined === user) return [];
  if (undefined === user.username) return [];
  const ownEnclaves = cfg.enclaves.filter(e => {
    if (e.hosts) {
      return e.hosts.find(host =>
        host.host === user.username
      );
    }
  });
  var subEnclaves = [];
  ownEnclaves.forEach(encl => {
    if (encl.subEnclaves)
      encl.subEnclaves.forEach(pubKey => {
        const subEncl = cfg.enclaves.find(e => e.pubKey === pubKey);
        if (subEncl) subEnclaves.push(subEncl);
      });
  });
  return [...ownEnclaves, ...subEnclaves];
}

export function getEnclaveByKey(pubKey) {
  const adminUserId = Meteor.users.findOne({ username: "admin" })._id;
  var cfg = userCfgDb.findOne({ _id: adminUserId });
  if (undefined === cfg || null === cfg) return;
  const enclave = cfg.enclaves.find(e => e.pubKey === pubKey);
  return enclave;
}

export function updateEnclave(userId, enclave) {
  var cfg = userCfgDb.findOne({ _id: userId });
  if (undefined === cfg || null === cfg) return;
  const enclaves = cfg.enclaves.filter(e => e.pubKey !== enclave.pubKey);
  enclaves.push(enclave);
  userCfgDb.update({ _id: userId }, { $set: { enclaves: enclaves } });
}

Meteor.methods({
  userGetEnclaveList(username) {
    if (undefined === this.userId) return { error: "Login required." };
    var userId;
    if (undefined === username) userId = this.userId;
    else {
      const user = Meteor.users.findOne({ username: username });
      if (undefined === user) return { error: "User not found." };
      userId = user._id;
    }
    const enclaves = getEnclavesByHost(userId);
    if (undefined !== enclaves)
      enclaves.map(e => {
        if (typeof e.infoJson === 'string') {
          e.infoJson = EJSON.parse(e.infoJson);
        }
      });
    return { enclaveList: enclaves };
  },

  killTask(pubKey, ethAdd, tid) {
    var { error, userStIdx, enclave, sessionIdx } = enclaveCheck(
      this.userId,
      endpoint.RUNNER,
      pubKey,
      roles.DATA_SCIENTIST,
      ethAdd
    );
    if (undefined !== error) return { error: error };
    const sessions = userStatus[userStIdx].enclave_session;
    var dataJson = {
      msgType: msgType.KILL_TASK,
      role: roles.USER,
      session_id: sessions[sessionIdx].id,
      //ethAdd:ethAdd,
      tid: Number(tid)
    };
    var resultJson = sendData2Enclave(
      this.userId,
      enclave,
      sessionIdx,
      dataJson
    );
    if (resultJson.msgType != msgType.KILL_TASK_SUCCESS) {
      return { error: "Error : Unable to kill the task" };
    }
    return resultJson;
  },

  clearTaskList(pubKey, ethAdd) {
    var { error, userStIdx, enclave, sessionIdx } = enclaveCheck(
      this.userId,
      endpoint.RUNNER,
      pubKey,
      roles.DATA_SCIENTIST,
      ethAdd
    );
    if (undefined !== error) return { error: error };
    const sessions = userStatus[userStIdx].enclave_session;
    var dataJson = {
      msgType: msgType.CLEAR_TASK_LIST,
      role: roles.USER,
      session_id: sessions[sessionIdx].id
      //ethAdd:ethAdd,
    };
    var resultJson = sendData2Enclave(
      this.userId,
      enclave,
      sessionIdx,
      dataJson
    );
    if (resultJson.msgType != msgType.CLEAR_TASK_LIST_SUCCESS) {
      return { error: "Error : Unable to get the clear task details" };
    }
    return resultJson;
  },

  getTaskStatus(pubKey, ethAdd, tidx) {
    var { error, userStIdx, enclave, sessionIdx } = enclaveCheck(
      this.userId,
      endpoint.RUNNER,
      pubKey,
      roles.DATA_SCIENTIST,
      ethAdd
    );
    if (undefined !== error) return { error: error };
    const sessions = userStatus[userStIdx].enclave_session;
    var dataJson = {
      msgType: msgType.GET_TASK_STATUS,
      role: roles.USER,
      session_id: sessions[sessionIdx].id,
      tidx: tidx
      //ethAdd:ethAdd,
    };
    var resultJson = sendData2Enclave(
      this.userId,
      enclave,
      sessionIdx,
      dataJson
    );
    if (resultJson.msgType !== msgType.GET_TASK_STATUS_SUCCESS) {
      return { error: "Error: Unable to get the task status" };
    }

    //return resultJson.taskDetails

    var result = resultJson.taskDetails;
    //result = [{"taskType":906,"startTime":38423237,"taskStatus":"ACTIVE"}]
    for (res in result) {
      date = new Date(
        (result[res].startTime + 1514786400) * 1000
      ).toLocaleString("en-US", { timeZone: "America/New_York" });
      result[res].startTime = date;
      result[res].taskType = msgName[result[res].taskType];
      if (result[res].msgType != undefined)
        result[res].msgType = msgName[result[res].msgType];
    }
    //return result
    return resultJson.taskDetails;
  },

  getEnclaveInfo(pubKey, ethAcc, path) {
    var { error, userStIdx, enclave, sessionIdx, stId } = enclaveCheck(
      this.userId,
      endpoint.RUNNER,
      pubKey,
      roles.DATA_SCIENTIST,
      ethAcc
    );
    if (undefined !== error) {
      return { error: error };
    }
    if (undefined === path) {
      path = "/";
    }
    var dataJson = {
      session_id: userStatus[userStIdx].enclave_session[sessionIdx].id,
      msgType: msgType.GET_INFO,
      role: roles.USER,
      ethAddress: ethAcc,
      path: path,
      stId: stId
    };
    const json = sendData2Enclave(this.userId, enclave, sessionIdx, dataJson);

    if (json.error) {
      return json;
    }
    if (msgType.GET_INFO_RSP_SUCCESS === json.msgType) {
      console.log("Tree structure");
      console.log(json.result);
      return { result: json.result };
    } else {
      return { error: "Get enclave information responses failed." };
    }
  },

  enclaveAttestation(server_url, pubKey, runnerEth) {
    var { error, userStIdx, enclave, sessionIdx } = enclaveCheck(
      this.userId,
      endpoint.RUNNER,
      pubKey,
      roles.RUNNER,
      runnerEth
    );
    if (undefined !== error) return { error: error };
    const dataJson = {
      session_id: userStatus[userStIdx].enclave_session[sessionIdx].id,
      msgType: msgType.ATTESTATION,
      role: roles.RUNNER,
      server_url: server_url,
      //spid:"0x7BC0338D24781F744875178FE7381F76"
      //spid:"0xF145FE5E7F1C943DFD6B2A2D9ECBDB11"
      spid: "0xB442130A5EA5E87514F31E4857FA4EAF"
    };
    const json = sendData2Enclave(this.userId, enclave, sessionIdx, dataJson);
    if (json.error) return json;
    if (json.msgType === msgType.ATTESTATION_RSP_SUCCESS) {
      var infoJson = EJSON.parse(enclave.infoJson);
      infoJson.attestationReport = json.Report.attestation_report;
      infoJson.attSignature = json.Report["X-IASReport-Signature"];
      infoJson.attSignCert = json.Report["X-IASReport-Signing-Certificate"];
      enclave.infoJson = EJSON.stringify(infoJson);
      const adminUserId = Meteor.users.findOne({ username: "admin" });
      updateEnclave(adminUserId, enclave);
      //userCfgDb.update({_id:adminUserId, enclaves:{pubKey:enclave.pubKey}},
      //  {$set:{"enclaves.$[]":enclave}})
      return { result: json.Report };
    }
    if (json.msgType === msgType.ATTESTATION_RSP_FAIL)
      return { error: "Attestation fail." };
    return { error: "Bad response message to attestation request." };
  },

  adminAddEnclave(infoJson, pubkey, roleInfos, hosts, url) {
    if (!validateUserRole(this.userId, "admin"))
      return { error: "Must login as an administrator." };
    var cfg = getUserCfg(this.userId);
    if (undefined === cfg) return { error: "Can't find user configuration." };
    const infoJsonString = EJSON.stringify(infoJson);
    var warning;
    var enclave = cfg.enclaves.find(e => e.pubKey === pubkey);
    if (undefined === enclave)
      enclave = {
        infoJson: infoJsonString,
        infoJsonHash: "", //infoJsonHash,
        pubKey: pubkey,
        url: "",
        model_id: "",
        roleInfos: roleInfos,
        hosts: hosts
      };
    else {
      if (undefined !== enclave.registerTxId) {
        warning = "Registered enclave information wll not be modified";
        enclave.url = url;
        enclave.roleInfos = roleInfos;
        enclave.hosts = hosts;
      } else {
        enclave.infoJson = infoJsonString;
        enclave.infoJsonHash = ""; //infoJsonHash
        enclave.url = url;
        enclave.roleInfos = roleInfos;
        enclave.hosts = hosts;
      }
    }
    updateEnclave(this.userId, enclave);
    //userCfgDb.update({_id:this.userId, enclaves:{pubKey:pubkey}},
    //  {$set: {'enclaves.$':enclave}}, {upsert:true})
    return { result: "Add Enclave success.", warning: warning };
  },

  adminDelEnclave(pubKey) {
    if (!validateUserRole(this.userId, "admin"))
      return { error: "Must login as an administrator." };
    userCfgDb.update(
      { _id: this.userId },
      { $pull: { enclaves: { pubKey: pubKey } } }
    );
    return { result: "Delete enclave " + pubKey + " success." };
  },

  adminAddHost(pubKey, newHost, stId, runnerEth) {
    if (!validateUserRole(this.userId, "admin")) {
      return { error: "Must login as an administrator." };
    }
    var {error, userStIdx, enclave, sessionIdx} = enclaveCheck(
          this.userId, endpoint.RUNNER, pubKey, roles.RUNNER, runnerEth)
    if (undefined !== error) {
        return ({ error:error });
    }
    var dataJson = {
        session_id: userStatus[userStIdx].enclave_session[sessionIdx].id,
        msgType: msgType.ADD_HOST_USER,
        role: roles.RUNNER,
        runnerEth: runnerEth,
        ethAddress: newHost.hostEth,
        stId: stId,
    }
    const json = sendData2Enclave(this.userId, enclave, sessionIdx, dataJson)
    if (json.error) { return json; }
    if (msgType.ADD_HOST_USER_RSP_SUCCESS === json.msgType){
        console.log("Result from ADD HOST");
        console.log(json.receipt);
        return { result: json.Report };
    }
    else
        return { result: "Add host " + newHost.host + " success." };
  },

  adminUpdateHost(pubKey, newHost, oldEth, runnerEth) {
    if (!validateUserRole(this.userId, "admin")) {
      return { error: "Must login as an administrator." };
    }
    var {error, userStIdx, enclave, sessionIdx} = enclaveCheck(
          this.userId, endpoint.RUNNER, pubKey, roles.RUNNER, runnerEth)
    if (undefined !== error) {
        return ({ error:error });
    }
    console.log("oldeth: " + oldEth);
    console.log("new eth: "+ newHost.hostEth);
    var dataJson = {
        session_id: userStatus[userStIdx].enclave_session[sessionIdx].id,
        msgType: msgType.UPDATE_HOST_USER,
        role: roles.RUNNER,
        runnerEth: runnerEth,
        ethAddress: newHost.hostEth,
        oldEthAddress: oldEth,
        stId: newHost.stId,
    }
    const json = sendData2Enclave(this.userId, enclave, sessionIdx, dataJson)
    if (json.error) { return json; }
    if (msgType.UPDATE_HOST_USER_RSP_SUCCESS === json.msgType){
        console.log("Result from HOST UPDATE");
        console.log(json.receipt);
        return { result: json.Report };
    }
    else
        return { result: "Update host " + newHost.host + " success." };
  },

  adminDelHost(pubKey, deletedHost, stId, runnerEth) {
    console.log("role id: \n");
    console.log(stId);
    if (!validateUserRole(this.userId, "admin")) {
      return { error: "Must login as an administrator." };
    }
    var {error, userStIdx, enclave, sessionIdx} = enclaveCheck(
          this.userId, endpoint.RUNNER, pubKey, roles.RUNNER, runnerEth)
    if (undefined !== error) {
        return ({ error:error });
    }
    var dataJson = {
        session_id:userStatus[userStIdx].enclave_session[sessionIdx].id,
        msgType:msgType.DELETE_HOST_USER,
        role:roles.RUNNER,
        ethAddress:deletedHost.hostEth,
        stId: stId,
    }
    const json = sendData2Enclave(this.userId, enclave, sessionIdx, dataJson)
    if (json.error) { return json; }
    if (msgType.DELETE_HOST_USER_RSP_SUCCESS === json.msgType){
        console.log("Result from HOST DELETE");
        console.log(json.receipt);
        return { result: json.Report };
    }
    else
        return { result: "Delete host " + deletedHost.host + " success." };
  },

  updateTxnHashEnclave(pubKey, txnHash) {
    if (undefined === this.userId) return { error: "Login required." };
    var cfg = getUserCfg(this.userId);
    //var cfg = userCfgDb.findOne({_id:this.userId})
    if (undefined === cfg) return { error: "Can't find user configuration." };

    var found = false,
      idx;
    for (idx in cfg.enclaves)
      if (cfg.enclaves[idx].pubKey === pubKey) {
        found = true;
        break;
      }

    if (found) {
      cfg.enclaves[idx].registerTxId = txnHash;
      userCfgDb.update(
        { _id: this.userId },
        { $set: { enclaves: cfg.enclaves } }
      );
      return { result: "Txn hash set successfully" };
    } else {
      return { error: "Package Id does not match" };
    }
  },

  getSampleCodeList() {
    const fs = require('fs');
    let data = fs.readFileSync(process.env.PWD+'/private/sampleCode.json', 'utf-8');
    let json_obj = JSON.parse(data);
    return { result: Object.keys(json_obj) };
  },

  getSampleCode(sampleName, pubkey, ethAddr) {
    if (undefined === sampleName) {
      return { error: "Invalid Sample Name" };
    }
    const fs = require('fs');
    let data = fs.readFileSync(process.env.PWD+'/private/sampleCode.json', 'utf-8');
    let json_obj = JSON.parse(data);
    let ret = json_obj[sampleName];
    subencs = Meteor.call("getSublistByHub", pubkey);
    if (subencs.error) {
      return { error: subencs.error};
    } else {
      let datasetcount = 0;
      ret.sub2codemap = {};
      for (let i = 0; i < subencs.result.length; i++) {
        if (subencs.result[i].key === pubkey) {
          continue;
        }
        if (!(ret.datafiles[datasetcount])) {
          return { error: "Not enough datafiles for subenclaves"};
        }
        let data = fs.readFileSync(process.env.PWD+'/private/sampleData/' + ret.datafiles[datasetcount]);
        if (!data) {
          console.log("Cannot open dataset file:", "/private/sampleData/" + ret.datafiles[datasetcount]);
          return { error: "Cannot find dataset file."};
        }
        ret.sub2codemap[subencs.result[i].name] = ret.datafiles[datasetcount];
        const dataInfo = {
          name: ret.datafiles[datasetcount],
          size: data.length,
          type: "csv",
          license: "Open source",
          tags: "",
          keywords: "",
          descriptions: "",
          stats: { score: "", ratings: "", comments: "", views: "" },
          owned: true
        };
        copyRet = Meteor.call("userShareDataFrontEnd", subencs.result[i].key,
                    ethAddr, "/", dataInfo, data.toString('base64'), 0);
        if (copyRet.error) {
          return { error: copyRet.error};
        } else {
          if (copyRet.receipt === "0x239492347934875349") {
            console.log("Upload Successful");
          }
        }
        datasetcount++;
      }
    }
    return { result: ret};
  },

  getSublistByHub(key) {
    let ret = [];
    let currEnc = getEnclaveByKey(key);
    if (undefined === currEnc) return { error: "Enclave not found." };
    let currInfoJson = JSON.parse(currEnc.infoJson);
    ret.push({ name: currInfoJson.name, key: key });
    let subs = currEnc.subEnclaves;
    if (!subs) return { result: ret };
    //if (subs.length === 0) return { error: "No subenclaves found" };
    subs.forEach(item => {
      let itemJson = JSON.parse(getEnclaveByKey(item).infoJson);
      ret.push({ name: itemJson.name, key: item });
    });
    return { result: ret };
  },

  registerEnclave(
    ethAccount,
    infoJson,
    pubKey,
    runnerAddress = "0xb480c3233e1852ea00fd0eec7149b2a6e67c23b2"
  ) {
    var txnHash = null;

    //var infoJSON = {"infoJSON": packageJson};
    var hashInfoJSON = web3.utils.sha3(JSON.stringify(infoJson));
    web3.eth.defaultAccount = ethAccount;

    console.log("hash infoJSON", hashInfoJSON);

    var Future = Npm.require("fibers/future");
    var passwordFut = new Future();

    Meteor.call("getPassword", ethAccount, (error, result) => {
      if (!error) {
        passwordFut.return(result.password);
      } else {
        console.log(error);
        passwordFut.return(null);
      }
    });

    const ethpassword = passwordFut.wait();

    web3.eth.personal.unlockAccount(ethAccount, ethpassword).then(response => {
      myContract.methods
        .registerEnclaveByTrainer(runnerAddress, hashInfoJSON, pubKey)
        .send({ from: ethAccount, gas: 200000 })
        .then(function(receipt) {
          // receipt can also be a new contract instance, when coming from a "contract.deploy({...}).send()"
          console.log("Success-Receipt", receipt);
          txnHash = receipt.transactionHash;

          var postData = {
            key: hashInfoJSON,
            value: { infoJSON: JSON.stringify(infoJson) }
          };
          HTTP.post(
            "http://localhost:4100/entry/insert",
            {
              //HTTP.post('https://hub.secureailabs.com/entry/insert', {
              headers: {
                "Content-Type": "application/json"
              },
              // npmRequestOptions: {
              //     rejectUnauthorized: false // TODO remove when deploy
              // },
              data: postData
            },
            (error, result) => {
              if (!error) {
                console.log(result);
                web3.eth.personal
                  .lockAccount(ethAccount)
                  .then(response => {
                    //console.log(response);
                    //console.log("outside",txnHash);
                    Meteor.call(
                      "updateTxnHashEnclave",
                      pubKey,
                      txnHash,
                      (error, result) => {
                        if (error) {
                          console.log("error:" + error);
                        } else {
                          console.log(result);
                        }
                      }
                    );
                  })
                  .catch(error => {
                    console.log(error);
                  });
              } else {
                console.log(error);
              }
            }
          );
        })
        .catch(error => {
          console.log(error);
        });
    });
  }
});
