import { Meteor } from "meteor/meteor";
import { userCfgDb, getUserCfg } from "../account/account.js";
import { userStatus, getUserSt } from "../account/acc_status.js";
import { enclaveCheck, sendData2Enclave } from "/server/model/enclave.js";
import { msgType } from "/imports/api/msg_type.js";
import { endpoint, roles } from "/imports/api/enum.js";

function lengthInUtf8Bytes(str) {
  // Matches only the 10.. bytes that are non-initial characters in a multi-byte sequence.
  var m = encodeURIComponent(str).match(/%[89ABab]/g);
  return str.length + (m ? m.length : 0);
}

export function directoryOps(msg) {
  userId = msg.userId;
  pubKey = msg.pubKey;
  ethAddr = msg.ethAddr;
  if (!userId || !pubKey || !ethAddr) return;
  var { error, userStIdx, enclave, sessionIdx } = enclaveCheck(
    userId,
    endpoint.RUNNER,
    pubKey,
    roles.DATA_SCIENTIST,
    ethAddr
  );
  var dataJson = {
    session_id: userStatus[userStIdx].enclave_session[sessionIdx].id,
    msgType: msgType.DIR_OP,
    opCode: msg.opCode,
    name: msg.filename
  };

  var json;

  if (msg.subPubKey){
    const dataJson2Hub = {
      session_id:userStatus[userStIdx].enclave_session[sessionIdx].id,
      msgType:msgType.SEND_TO_A_SUB,
      role:roles.HOST,
      subPubKey: msg.subPubKey,
      messageToSub: JSON.stringify(dataJson),
    }
    json = sendData2Enclave(this.userId,enclave,sessionIdx, dataJson2Hub)
    if (json.error)
        return json
    if (msgType.SEND_TO_A_SUB_SUCCESS === json.msgType)
      json = json.subResp
    else return { error: "Talking to the Sub Enclave failed." };
  } else
    json = sendData2Enclave(userId, enclave, sessionIdx, dataJson);

  if (json.error) return json;
  if (msgType.DIR_OP_RSP_SUCCESS === json.msgType) {
    return { result: json.result, image: json.image };
  } else return { error: "Kernel responses failed." };
}

export function runKernelCode(msg) {
  code = msg.code;
  userId = msg.userId;
  pubKey = msg.pubKey;
  ethAddr = msg.ethAddr;
  if (!userId || !pubKey || !ethAddr) return;
  var { error, userStIdx, enclave, sessionIdx } = enclaveCheck(
    userId,
    endpoint.RUNNER,
    pubKey,
    roles.DATA_SCIENTIST,
    ethAddr
  );
  var dataJson = {
    session_id: userStatus[userStIdx].enclave_session[sessionIdx].id,
    msgType: msgType.RUN_MODEL,
    role: roles.DATA_SCIENTIST,
    name: "kernel.py",
    data: code
  };

  var json;

  if (msg.subPubKey){
    const dataJson2Hub = {
      session_id:userStatus[userStIdx].enclave_session[sessionIdx].id,
      msgType:msgType.SEND_TO_A_SUB,
      role:roles.HOST,
      subPubKey: msg.subPubKey,
      messageToSub: JSON.stringify(dataJson),
    }
    json = sendData2Enclave(this.userId,enclave,sessionIdx, dataJson2Hub)
    if (json.error)
        return json
    if (msgType.SEND_TO_A_SUB_SUCCESS === json.msgType)
      json = json.subResp      
    else return { error: "Talking to the Sub Enclave failed." };
  } else
    json = sendData2Enclave(userId, enclave, sessionIdx, dataJson);

  if (json.error) return json;
  if (msgType.RUN_MODEL_RSP_SUCCESS === json.msgType) {
    return { result: json.result, image: json.image };
  } else return { error: "Enclave kernel response failed." };
}

Meteor.methods({
  getProjectsInfo() {
    if (undefined === this.userId) return "Login rquired.";
    var cfg = userCfgDb.findOne({ _id: this.userId });
    if (undefined === cfg) {
      cfg = getUserCfg(this.userId);
    }
    return cfg.models;
  },

  addProjectInfo(projectInfo) {
    console.log(projectInfo)
    if (undefined === this.userId) return { error: "Login rquired." };
    var cfg = userCfgDb.findOne({ _id: this.userId });
    if (undefined === cfg) {
      cfg = getUserCfg(this.userId);
    }
    let isOld = false
    if (cfg.models)
      if (cfg.models.find(m=>m.name === projectInfo.name))
        isOld = true
    if (!isOld) {
      console.log("new project");
      userCfgDb.update(
        { _id: this.userId },
        { $push: { models: projectInfo } }
      );
    } else {
      console.log("existing project");
      userCfgDb.update(
        {
          _id: this.userId,
          models: {
            $elemMatch: { name: projectInfo.name }
          }
        },
        //{ $set: { "models.$.code": projectInfo.code } }
        { $set: { "models.$": projectInfo } }
        );
    }
  },

  userSaveCodes(infoJson, pubKey, ethAddr) {
    var { error, userStIdx, enclave, sessionIdx } = enclaveCheck(
      this.userId,
      endpoint.RUNNER,
      pubKey,
      roles.DATA_SCIENTIST,
      ethAddr
    );
    if (undefined !== error) return { error: error };

    var dataJson = {
      session_id: userStatus[userStIdx].enclave_session[sessionIdx].id,
      msgType: msgType.UPLOAD_MODEL,
      role: roles.DATA_SCIENTIST,
      title: infoJson.title,
      code: infoJson.hubContent,
      subEnclaves: infoJson.subEnclaves,
    };
  
    const json = sendData2Enclave(this.userId, enclave, sessionIdx, dataJson);
  
    if (!json) return { error: "Save code to enclaves responses failed."};
    if (msgType.UPLOAD_MODEL_RSP_SUCCESS === json.msgType) {
      return { result: "Save code to enclaves responses succeeded."};
    } else return { error: "Save code to enclaves responses failed."};
  },
  
  userRunCode(RwebInfoJson, pubKey, ethAddr, dataName) {
    var { error, userStIdx, enclave, sessionIdx } = enclaveCheck(
      this.userId,
      endpoint.RUNNER,
      pubKey,
      roles.DATA_SCIENTIST,
      ethAddr
    );
    if (undefined !== error) return { error: error };

    var name = RwebInfoJson.title;
    if (undefined === name) name = "";
    var data = RwebInfoJson.content;
    if (undefined === data) data = "";
    var dataJson = {
      session_id: userStatus[userStIdx].enclave_session[sessionIdx].id,
      msgType: msgType.RUN_MODEL,
      role: roles.DATA_SCIENTIST,
      name: name,
      data: data
    };

    const json = sendData2Enclave(this.userId, enclave, sessionIdx, dataJson);

    if (json.error) return json;
    if (msgType.RUN_MODEL_RSP_SUCCESS === json.msgType) {
      let accessInfo = {
        modelName: name,
        dataName: dataName,
        ethAddress: ethAddr,
        enclavePubKey: pubKey,
        resultSize: lengthInUtf8Bytes(json.result),
        username: Meteor.users.findOne({ _id: this.userId }).username,
        timestamp: new Date().getTime()
      };
      // registerDataAccessByTrainer(ethAddr, accessInfo)
      return { result: json.result };
    } else return { error: "R console responses failed." };
  },

  userFederated(infoJson, pubKey, ethAddr, dataName) {
    var { error, userStIdx, enclave, sessionIdx } = enclaveCheck(
      this.userId,
      endpoint.RUNNER,
      pubKey,
      roles.DATA_SCIENTIST,
      ethAddr
    );
    if (undefined !== error) return { error: error };

    var name = infoJson.title;
    if (undefined === name) name = "";
    var subData = infoJson.subContent;
    var hubData = infoJson.hubContent;
    var subKeys = infoJson.sKeys;
    if (undefined === subData) subData = [];
    if (undefined === hubData) hubData = [];
    if (undefined === subKeys) subKeys = [];
    var dataJson = {
      session_id: userStatus[userStIdx].enclave_session[sessionIdx].id,
      msgType: msgType.RUN_FEDERATED_MODEL,
      role: roles.DATA_SCIENTIST,
      bgfg: "true",
      name: name,
      hubcode: hubData,
      subcode: subData,
      subkeys: subKeys
    };

    console.log(dataJson)
    const json = sendData2Enclave(this.userId, enclave, sessionIdx, dataJson);

    if (json.error) return json;
    if (msgType.START_TASK_SUCCESS === json.msgType) {
      let accessInfo = {
        modelName: name,
        dataName: dataName,
        ethAddress: ethAddr,
        enclavePubKey: pubKey,
        resultSize: lengthInUtf8Bytes(json.result),
        username: Meteor.users.findOne({ _id: this.userId }).username,
        timestamp: new Date().getTime()
      };
      // registerDataAccessByTrainer(ethAddr, accessInfo)
      return json;
    } else return { error: "R console responses failed.", tidx: json.tidx };
  },

  userDeleteModel(filename, pubKey, ethAddr) {
    var { error, userStIdx, enclave, sessionIdx } = enclaveCheck(
      this.userId,
      endpoint.RUNNER,
      pubKey,
      roles.DATA_SCIENTIST,
      ethAddr
    );
    if (undefined !== error) return { error: error };

    var dataJson = {
      session_id: userStatus[userStIdx].enclave_session[sessionIdx].id,
      msgType: msgType.DELETE_MODEL,
      role: roles.DATA_SCIENTIST,
      name: filename
    };

    const json = sendData2Enclave(this.userId, enclave, sessionIdx, dataJson);

    if (json.error) return json;
    if (msgType.DELETE_MODEL_RSP_SUCCESS !== json.msgType) {
      return { error: "Delete model responses failed." };
    }
    let modelDelInfo = {
      modelName: filename,
      ethAddress: ethAddr,
      enclavePubKey: pubKey,
      username: Meteor.users.findOne({ _id: this.userId }).username,
      timestamp: new Date().getTime()
    };
    // registerModelDeletionByTrainer(ethAddr, modelDelInfo)
    return { result: "Model deleted." };
  },

  userLoadModel(pubKey, ethAddr, name) {
    var { error, cfg, userStIdx, enclave, sessionIdx } = enclaveCheck(
      this.userId,
      endpoint.RUNNER,
      pubKey,
      roles.DATA_SCIENTIST,
      ethAddr
    );
    if (undefined !== error) return { error: error };

    var dataJson = {
      session_id: userStatus[userStIdx].enclave_session[sessionIdx].id,
      msgType: msgType.DNLOAD_MODEL,
      role: roles.DATA_SCIENTIST,
      name: name
    };

    const json = sendData2Enclave(this.userId, enclave, sessionIdx, dataJson);

    if (json.error) return json;
    if (msgType.DNLOAD_MODEL_RSP_SUCCESS !== json.msgType)
      return { error: "Enclave responses failed." };
    return { result: json.content };
  },

  userRunModelFile(pubKey, ethAddr, name) {
    var { error, userStIdx, enclave, sessionIdx } = enclaveCheck(
      this.userId,
      endpoint.RUNNER,
      pubKey,
      roles.DATA_SCIENTIST,
      ethAddr
    );
    if (undefined !== error) return { error: error };

    var dataJson = {
      session_id: userStatus[userStIdx].enclave_session[sessionIdx].id,
      msgType: msgType.RUN_MODEL_FILE,
      role: roles.DATA_SCIENTIST,
      name: name
    };

    const json = sendData2Enclave(this.userId, enclave, sessionIdx, dataJson);

    if (json.error) return json;
    if (msgType.RUN_MODEL_FILE_RSP_SUCCESS !== json.msgType)
      return { error: "Enclave responses failed." };
    return { result: json.result };
  }
});
