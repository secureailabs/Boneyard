import { Meteor } from 'meteor/meteor';
import { Mongo } from 'meteor/mongo';
import { userCfgDb, getUserCfg } from '../account/account.js';
import { userStatus, getUserSt } from '../account/acc_status.js';
import { enclaveCheck, sendData2Enclave } from '/server/model/enclave.js';
import { msgType } from '/imports/api/msg_type.js'
import { endpoint, roles } from '/imports/api/enum.js'
import { EJSON } from 'meteor/ejson'
import { Random } from 'meteor/random'

function genData(json, fileName){
  var data = [];
  for (file in json.filenames){
    if (json.filenames[file] !== fileName) {
      data.push({fileName:json.filenames[file],
        attributesName: "", value: "", ratio:0})
    }
    else {
      for (attr in json.attributes){
        for (val in json.attributes[attr].attrValue){
          data.push({fileName:fileName,
            attributesName: json.attributes[attr].name,
            value: json.attributes[attr].attrValue[val].value,
            ratio:json.attributes[attr].attrValue[val].ratio})
        }
      }
    }
  }
  return data;
}

function genSingleData(json){
  var data = [];
  for (attr in json.attributes){
    for (val in json.attributes[attr].attrValue){
      data.push({
        attributesName: json.attributes[attr].name,
        value: json.attributes[attr].attrValue[val].value,
        ratio:json.attributes[attr].attrValue[val].ratio})
    }
  }
  return data;
}

Meteor.methods({
    userLoadInspectData(fileName, condictions, pubKey, ethAddr) {
      var {error, userStIdx, enclave, sessionIdx}
        =  enclaveCheck(this.userId, endpoint.RUNNER, pubKey, roles.DATA_SCIENTIST, ethAddr)
      if (undefined !== error)
        return ({error:error});

    if (undefined === fileName)
      fileName = ""
    if (undefined === condictions)
      condictions = ""
    var dataJson = {
      session_id:userStatus[userStIdx].enclave_session[sessionIdx].id,
      msgType:msgType.DI_GET_DATA,
      role:roles.DATA_SCIENTIST,
      dataName:fileName,
      conditions:condictions,
    }

    //const url = cfg.enclaves[enclaveIdx].url+"/"+endpoint.RUNNER

    const json = sendData2Enclave(this.userId, enclave, sessionIdx, dataJson)

    if (json.error)
      return json
    if (msgType.DI_GET_DATA_RSP_SUCCESS === json.msgType)
      return ({result:genData(json, fileName)})
    else
      return ({error:"Data inspector responses failed."})
    },

    userLoadSingleDataInfo(fileName, condictions, pubKey, ethAddr) {
      var {error, userStIdx, enclave, sessionIdx}
        =  enclaveCheck(this.userId, endpoint.RUNNER, pubKey, roles.DATA_SCIENTIST, ethAddr)
      if (undefined !== error)
        return ({error:error});

    if (undefined === fileName)
      fileName = ""
    if (undefined === condictions)
      condictions = ""
    var dataJson = {
      session_id:userStatus[userStIdx].enclave_session[sessionIdx].id,
      msgType:msgType.DI_GET_DATA,
      role:roles.DONOR,
      dataName:fileName,
      singleData:"true",
      conditions:condictions,
    }

    const json = sendData2Enclave(this.userId, enclave, sessionIdx, dataJson)

    if (json.error)
      return json
    if (msgType.DI_GET_DATA_RSP_SUCCESS === json.msgType)
      return ({result:genSingleData(json)})
    else
      return ({error:"Data inspector responses failed."})
    },

    userLoadSingleDataView(fileName, pubKey, ethAddr) {
        var {error, userStIdx, enclave, sessionIdx}
            =  enclaveCheck(this.userId, endpoint.RUNNER, pubKey, roles.DATA_SCIENTIST, ethAddr)
        if (undefined !== error)
            return ({error:error});

        if (undefined === fileName)
            fileName = ""
        var dataJson = {
            session_id:userStatus[userStIdx].enclave_session[sessionIdx].id,
            msgType:msgType.DV_GET_DATA,
            role:roles.DONOR,
            dataName:fileName,
        }

        const json = sendData2Enclave(this.userId, enclave, sessionIdx, dataJson)

        if (json.error)
            return json
        if (msgType.DV_GET_DATA_RSP_SUCCESS === json.msgType){
            console.log(json)
            return json
        }
        else
            return ({error:"Data inspector responses failed."})
    },
    userLoadFullFile(fileName, pubKey, ethAddr) {
      console.log("~~~~~~~~~~~~~~~~~~~~~~~~~~~~`````````````")
      console.log(fileName)
      console.log(pubKey)
      console.log(ethAddr)
        var {error, userStIdx, enclave, sessionIdx}
            =  enclaveCheck(this.userId, endpoint.RUNNER, pubKey, roles.DATA_SCIENTIST, ethAddr)
        if (undefined !== error)
            return ({error:error});

        if (undefined === fileName)
            fileName = ""
        var dataJson = {
            session_id:userStatus[userStIdx].enclave_session[sessionIdx].id,
            msgType:msgType.DV_GET_ALL_DATA,
            role:roles.DONOR,
            dataName:fileName,
        }

        const json = sendData2Enclave(this.userId, enclave, sessionIdx, dataJson)

        if (json.error)
            return json
        if (msgType.DV_GET_ALL_DATA_RSP_SUCCESS === json.msgType){
            console.log(json)
            return json
        }
        else
            return ({error:"Data inspector responses failed."})
    },
})
