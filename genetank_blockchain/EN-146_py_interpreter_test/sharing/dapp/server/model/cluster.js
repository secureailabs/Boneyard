import { Meteor } from "meteor/meteor";
import { endpoint, roles } from "/imports/api/enum.js";
import { userStatus } from "../account/acc_status";
import {
  enclaveCheck,
  sendData2Enclave,
  updateEnclave
} from "../model/enclave.js";
import { msgType } from "/imports/api/msg_type.js";
import { getUserCfg, userCfgDb, validateUserRole } from "../account/account";
import { runKernelCode, directoryOps } from "/server/model/project.js";

var fs = require("fs");
var Future = Npm.require("fibers/future");
var exec = require("child_process").exec;
const homedir = require("os").homedir();
const crypto = Npm.require("crypto");

var jupyterStates = [];

function deleteE2E(userId, hubEnclave, subEnclave) {
  var { error, userStIdx, enclave, sessionIdx } = enclaveCheck(
    userId,
    endpoint.RUNNER,
    hubEnclave.pubKey,
    roles.RUNNER,
    hubEnclave.roleInfos[0].ethAddr
  );
  if (undefined !== error) return { error: error };
  const infoJson = JSON.parse(subEnclave.infoJson);
  var dataJson = {
    session_id: userStatus[userStIdx].enclave_session[sessionIdx].id,
    msgType: msgType.DEL_SUB_ENCLAVE,
    role: roles.RUNNER,
    desturl: subEnclave.url,
    pubkey: subEnclave.pubKey,
    name: infoJson.name
  };

  const json = sendData2Enclave(userId, enclave, sessionIdx, dataJson);

  if (json.error) return json;
  if (msgType.DEL_SUB_ENCLAVE_SUCCESS === json.msgType)
    return { result: json.result };
  else return { error: "Deleting a sub enclave responses failed." };
}

function setupE2E(userId, hubEnclave, subEnclave) {
  const ethUtil = require("ethereumjs-util");
  var { error, userStIdx, enclave, sessionIdx } = enclaveCheck(
    userId,
    endpoint.RUNNER,
    hubEnclave.pubKey,
    roles.RUNNER,
    hubEnclave.roleInfos[0].ethAddr
  );
  if (undefined !== error) return { error: error };
  const infoJson = JSON.parse(subEnclave.infoJson);
  var dataJson = {
    session_id: userStatus[userStIdx].enclave_session[sessionIdx].id,
    msgType: msgType.ADD_SUB_ENCLAVE,
    role: roles.RUNNER,
    desturl: subEnclave.url,
    pubkey: subEnclave.pubKey,
    name: infoJson.name
  };

  const json = sendData2Enclave(userId, enclave, sessionIdx, dataJson);

  if (json.error) return json;
  if (msgType.ADD_SUB_ENCLAVE_SUCCESS !== json.msgType) {
    return { error: "Adding a sub enclave hub responses failed." };
  }

  var { error, userStIdx, enclave, sessionIdx } = enclaveCheck(
    userId,
    endpoint.RUNNER,
    subEnclave.pubKey,
    roles.RUNNER,
    subEnclave.roleInfos[0].ethAddr
  );
  if (undefined !== error) return { error: error };

  const hubEthAddr = ethUtil
    .pubToAddress(Buffer.from(hubEnclave.pubKey, "hex"))
    .toString("hex");
  var dataJson = {
    session_id: userStatus[userStIdx].enclave_session[sessionIdx].id,
    msgType: msgType.ADD_GUEST_USER,
    role: roles.RUNNER,
    ethAddress: hubEthAddr,
    userStakeHolders: subEnclave.hosts,
    signByRunner: "no sign yet"
  };

  const json1 = sendData2Enclave(userId, enclave, sessionIdx, dataJson);
  if (json1.error) return json1;
  if (msgType.ADD_GUEST_USER_SUCCESS !== json1.msgType) {
    return { error: "Adding a sub enclave sub responses failed." };
  }
  return { result: json.result, subResult: json1.result };
}

function checkPid(pid, command) {
  const cmd = "ps -p " + pid.toString() + " -f";
  const child = exec(cmd);
  if (undefined === child) {
    return false;
  }
  fut = new Future();
  child.stdout.on("data", data => {
    //console.log("ps stdout:", data);
    if (data.includes(command)) {
      fut.return(true);
    } else fut.return(false);
  });
  const ret = fut.wait();
  return ret;
}

export function handleJupyterReq(body) {
  const state = jupyterStates[body.id];
  if (undefined === state) {
    return {
      result:
        "Error: No state found. Please close the Jupyter page and re-run the Jupyterlab from the dApp."
    };
  }
  if (state.token !== body.token) {
    return { result: "Error: Bad token. Please re-run the Jupyterlab" };
  }
  const cfg = getUserCfg(state.userId);
  if (!cfg) return { result: "Error: user configuration is not found." };
  if (undefined === state.username) {
    const user = Meteor.users.findOne({ _id: state.userId });
    if (!user) return { result: "Error: user not found." };
    state.username = user.username;
  }
  let hub, sub;
  const enclaves = cfg.enclaves;
  for (encl in enclaves) {
    const enclave = encl;
    const info = JSON.parse(enclave.infoJson);
    if (info.name === body.enclave) {
      if (enclave.hosts) {
        const found = enclave.hosts.find(host => {
          host.host === state.username
        });
        if(found) { hub = enclave; }
      }
      // if (enclave.host === state.username) hub = enclave;
    } else if (info.name === body.subEnclave) sub = enclave;
    if (hub && (sub || body.subEnclave === "")) break;
  }
  if (!hub) return { result: "Error: Hub enclace is not found." };
  var msg = {
    userId: state.userId,
    pubKey: hub.pubKey,
    ethAddr: state.ethAddr
  };
  if (sub) msg.subPubKey = sub.pubKey;
  switch (body.opCode) {
    case "run":
      msg.code = body.code;
      return runKernelCode(msg);
    case "ls":
    case "cd":
    case "rm":
      msg.opCode = body.opCode;
      msg.filename = body.filename;
      return directoryOps(msg);
    case "upload":
      msg.filename = body.filename;
      msg.code = body.code;
      msg.codeLen = body.codeLen;
      return saveFile(msg);
    default:
      return {
        result: "Error: opCode '" + body.opCode + "' is unrecognizable."
      };
  }
}

function saveFile(msg) {
  userId = msg.userId;
  pubKey = msg.pubKey;
  ethAddr = msg.ethAddr;
  if (!userId || !pubKey || !ethAddr) return;
  var { error, userStIdx, enclave, sessionIdx, stId } = enclaveCheck(
    userId,
    endpoint.RUNNER,
    pubKey,
    roles.DATA_SCIENTIST,
    ethAddr
  );
  if (error)
    return { result: "Error while setting a connection with the Enclave." };

  var dataJson = {
    session_id: userStatus[userStIdx].enclave_session[sessionIdx].id,
    msgType: msgType.SAVE_FILE,
    opCode: msg.opCode,
    name: msg.filename,
    code: msg.code,
    dataSize: msg.codeLen,
    stId: stId
  };

  var json;

  if (msg.subPubKey) {
    const dataJson2Hub = {
      session_id: userStatus[userStIdx].enclave_session[sessionIdx].id,
      msgType: msgType.SEND_TO_A_SUB,
      role: roles.HOST,
      subPubKey: msg.subPubKey,
      messageToSub: JSON.stringify(dataJson)
    };
    json = sendData2Enclave(this.userId, enclave, sessionIdx, dataJson2Hub);
    if (json.error) return json;
    if (msgType.SEND_TO_A_SUB_SUCCESS === json.msgType) json = json.subResp;
    else return { error: "Talking to the Sub Enclave failed." };
  } else json = sendData2Enclave(userId, enclave, sessionIdx, dataJson);

  if (json.error) return json;
  if (msgType.SAVE_FILE_RSP_SUCCESS === json.msgType) {
    return { result: json.result, image: json.image };
  } else return { error: "Talking to the Enclave failed." };
}

Meteor.methods({
  adminDelSubEnclave(pubKey, subEnclaveKey) {
    if (!validateUserRole(this.userId, "admin"))
      return { error: "Must login as an administrator." };
    var cfg = getUserCfg(this.userId);
    if (undefined === cfg) return { error: "Can't find user configuration." };
    var found = false,
      idx;
    for (idx in cfg.enclaves)
      if (cfg.enclaves[idx].pubKey === pubKey) {
        found = true;
        break;
      }
    if (!found) return { error: "Enclave not found!" };
    const subEnclave = cfg.enclaves.find(e => e.pubKey === subEnclaveKey);
    const { error } = deleteE2E(this.userId, cfg.enclaves[idx], subEnclave);
    if (error) return { error: error };
    if (undefined === cfg.enclaves[idx].subEnclaves)
      cfg.enclaves[idx].subEnclaves = [];
    else {
      const newEnclaves = cfg.enclaves[idx].subEnclaves.filter(
        e => e !== subEnclaveKey
      );
      cfg.enclaves[idx].subEnclaves = newEnclaves;
    }
    userCfgDb.update(
      { _id: this.userId },
      { $set: { enclaves: cfg.enclaves } }
    );
    return { result: "Update Enclave success." };
  },

  adminAddSubEnclave(pubKey, subEnclaveKey) {
    if (!validateUserRole(this.userId, "admin"))
      return { error: "Must login as an administrator." };
    var cfg = getUserCfg(this.userId);
    if (undefined === cfg) return { error: "Can't find user configuration." };
    var found = false,
      idx;
    for (idx in cfg.enclaves)
      if (cfg.enclaves[idx].pubKey === pubKey) {
        found = true;
        break;
      }
    if (!found) return { error: "Enclave not found!" };
    const subEnclave = cfg.enclaves.find(e => e.pubKey === subEnclaveKey);
    const { error } = setupE2E(this.userId, cfg.enclaves[idx], subEnclave);
    if (error) return { error: error };
    if (undefined === cfg.enclaves[idx].subEnclaves)
      cfg.enclaves[idx].subEnclaves = [];
    const subEncl = cfg.enclaves[idx].subEnclaves.find(e => e == subEnclaveKey);
    if (!subEncl) {
      cfg.enclaves[idx].subEnclaves.push(subEnclaveKey);
      userCfgDb.update(
        { _id: this.userId },
        { $set: { enclaves: cfg.enclaves } }
      );
    }
    return { result: "Update Enclave success." };
  },

  userRunJupyterlab(ethAddr, forceKill) {
    var syncExec = Meteor.wrapAsync(exec);

    if (undefined === this.userId) return { error: "login requried" };
    var cfg = getUserCfg(this.userId);
    if (undefined === cfg) return { error: "Can't find user configuration." };
    //const hub = cfg.enclaves.find(e => e.pubKey === hubPubKey);
    //if (undefined === hub) return { error: "Enclave can't be found." };
    //if (!subPubKey) subPubKey = "";
    const encl_home = homedir + "/jupyter/" + this.userId;
    const token = crypto.randomBytes(64).toString("hex");
    var state = {
      userId: this.userId,
      ethAddr: ethAddr,
      token: token
    };
    var id = jupyterStates.findIndex(s => s.userId === this.userId);
    if (undefined === id || id < 0) {
      id = jupyterStates.push(state) - 1;
    } else {
      const pid = jupyterStates[id].pid;
      if (undefined !== pid) {
        const pidExist = checkPid(pid, pid.toString());
        if (pidExist){
          if (forceKill) {
            const isJupyter = checkPid(pid, "jupyter lab");
            if (isJupyter) {
              try {
                syncExec("pkill -9 -P " + pid);
              } catch (error) {
                console.log(error);
              }
            }
          } else {
            state = jupyterStates[id];
            const token = state.token.substr(0, 19)
            return {
              result:
                "Jupyter is running on: " +
                state.port.toString() +
                ", token: " + token
            };
          }
        }
      }
      jupyterStates[id] = state;
    }
    if (!fs.existsSync(encl_home)) {
      fs.mkdirSync(encl_home);
    }

    const url = process.env.ROOT_URL;
    const leading_str = url.match(/http:\/\/.*:/i);
    var port;
    if (null === leading_str) port = 3000;
    else port = parseInt(url.substring(leading_str[0].length));
    const user = Meteor.users.findOne({ _id: state.userId });
    const cmd =
      "ENCLAVE_PORT=" +
      port.toString() +
      " JUPYTER_USER='" +
      user.username +
      "' JUPYTER_ID='" +
      id.toString() +
      "' JUPYTER_TOKEN='" +
      state.token +
      "' jupyter lab --config='" +
      homedir +
      "/jupyter/jupyter_notebook_config.py'" +
      " --notebook-dir='" +
      encl_home +
      "'" +
      " --port=" +
      (port + 10).toString() +
      " --NotebookApp.token='" +
      state.token.substring(0, 19) +
      "'";

    const child = exec(cmd);
    if (undefined === child) {
      return { error: "Run command:" + cmd + " Error." };
    }
    state.pid = child.pid;
    child.stdout.on("data", data => {
      console.log("Jupyter stdout:", data);
    });
    fut = new Future();
    let returned = false;
    child.stderr.on("data", data => {
      console.log("Jupyter stderr:", data);
      if (data.includes("http://") && !returned) {
        fut.return(data);
        returned = true;
      }
    });
    const data = fut.wait();
    const real_port = data.match(/http:\/\/.*:[0-9]*/i);
    state.port = real_port;
    return {
      result:
        "Jupyter started. Port number: " +
        real_port +
        ", Token: " +
        state.token.substring(0, 19)
    };
  }
});
