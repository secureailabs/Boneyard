import {Meteor} from "meteor/meteor";
import { endpoint, roles } from '/imports/api/enum.js'
import {userStatus} from "../account/acc_status";
import { enclaveCheck, sendData2Enclave } from "../model/enclave.js"
import { msgType } from "/imports/api/msg_type.js"
import {deRegisterDataByOwner} from "../alphaReleaseSmartContracts/dataDeRegistrationContract";
import {registerDataByOwner} from "../alphaReleaseSmartContracts/dataRegistrationContract";
import {getUserCfg} from "../account/account";

Meteor.methods({
    userShareData2Guest(pubKey, ethAcc, srcDir, dstDir, dataName, shareInfo, guestPubKey){
        if (undefined === this.userId)
            return ({ error: "Login required." })
        
        var {error, userStIdx, enclave, sessionIdx} = enclaveCheck(
          this.userId, endpoint.RUNNER, pubKey, roles.DONOR, ethAcc)
        if (undefined !== error)
            return ({error:error});

        var dataJson = {
            session_id:userStatus[userStIdx].enclave_session[sessionIdx].id,
            msgType:msgType.SHARE_DATA_TO_GUEST,
            role:roles.HOST,
            sourceDir:srcDir,
            destDir:dstDir,
            dataName:dataName,
            guestPubKey: guestPubKey,
            startTime:shareInfo.startTime,
            endTime:shareInfo.endTime,
            //shareInfo:JSON.stringify(shareInfo)
        }
        if (undefined !== shareInfo.dataInfo){
            dataJson.dataInfo=JSON.stringify(shareInfo.dataInfo)
        }
       
        const json = sendData2Enclave(this.userId,enclave,sessionIdx, dataJson)
        if (json.error)
            return json
        if (msgType.SHARE_DATA_TO_GUEST_SUCCESS === json.msgType){
            //console.log("Result from Data share")
            //console.log(json.receipt)
            return ({response:json})
        }
        else
            return ({error:"Sharing data to enclave failed."})
    },

    userPushPage(pubKey, ethAddr, streamId, pageNo, data){
        if (undefined === this.userId)
            return ({ error: "Login required." })
        
        var {error, userStIdx, enclave, sessionIdx} = enclaveCheck(
          this.userId, endpoint.RUNNER, pubKey, roles.DONOR, ethAddr)
       
        if (undefined !== error)
            return ({error:error});
       
        var dataJson = {
            session_id:userStatus[userStIdx].enclave_session[sessionIdx].id,
            msgType:msgType.STREAM_PUSH,
            role:roles.DONOR,
            streamId:streamId,
            pageNo:pageNo,
            data: data,
        }
       
        const json = sendData2Enclave(this.userId, enclave, sessionIdx, dataJson)
        if (json.error)
            return json
        if (msgType.STREAM_PUSH_RSP_SUCCESS === json.msgType){
            return ({receipt:json.receipt, missingPage:json.missingPage})
        }
        else
            return ({error:"Sharing data to enclave error."})
    },

    userShareDataFrontEnd(pubKey, ethAccount, uploadPath,
      dataInfo, data, startStream) {
        if (undefined === this.userId)
            return ({ error: "Login required." })
        
        var {error, userStIdx, enclave, sessionIdx} = enclaveCheck(
          this.userId, endpoint.RUNNER, pubKey, roles.DONOR, ethAccount)
       
        if (undefined !== error)
            return ({error:error});
       
        var dataJson = {
            session_id:userStatus[userStIdx].enclave_session[sessionIdx].id,
            msgType:msgType.UPLOAD_DATA,
            role:roles.DONOR,
            uploadPath:uploadPath,
            dataName:dataInfo.name,
            ethAddress:ethAccount,
            data: data,
            fileSize: dataInfo.size,
            dataInfo:JSON.stringify(dataInfo),
            startStream:startStream
        }
       
        const json = sendData2Enclave(this.userId, enclave, sessionIdx, dataJson)
        if (json.error)
            return json
        if (msgType.UPLOAD_DATA_RSP_SUCCESS === json.msgType){
            console.log("Result from Data Upload")
            console.log(json.receipt)
            return ({receipt:json.receipt, streamReady:json.streamReady})
        }
        else
            return ({error:"Sharing data to enclave failed."})
    },

    callRegisterDataByOwner(ethAcc, dataInfo){
        console.log("Call from front end recieved.")
        const auditData = JSON.parse(JSON.stringify(dataInfo));
        auditData.audit = "audit";
        Meteor.call("registerDataPublic", ethAcc, dataInfo, (error, result) => {
            if(!error){
                console.log("Public data sharing success");
            }
            else{
                console.log(error);
            }
        });

        // registerDataByOwner(ethAcc, auditData)
    },

    userShareDataFrontEndE2E(pubKey, ethAccount, dataName, data, offset = 0, fileSize = 0) {
        if (undefined === this.userId)
            return ({ error: "Login required." })
        
        var {error, userStIdx, enclave, sessionIdx}
            =  enclaveCheck(this.userId, endpoint.RUNNER, pubKey, roles.DONOR, ethAccount)
       
        if (undefined !== error)
            return ({error:error});
       
        var dataJson = {
            session_id:userStatus[userStIdx].enclave_session[sessionIdx].id,
            msgType:msgType.UPLOAD_DATA_E2E,
            role:roles.DONOR,
            dataName:dataName,
            ethAddress:ethAccount,
            data: data,
            fileSize: fileSize,
            offset:offset
        }
       
        const json = sendData2Enclave(this.userId,enclave,sessionIdx,dataJson)
        if (json.error)
            return json
        if (msgType.UPLOAD_DATA_E2E_RSP_SUCCESS === json.msgType)
            return ({receipt:json.receipt})
        else
            return ({error:"Sharing data to enclave failed."})
    },

    userDelDataFrontEnd(pubKey, ethAccount, dataName) {
        if (undefined === this.userId)
            return ({ error: "Login required." })
        
        var {error, userStIdx, enclave, sessionIdx}
            =  enclaveCheck(this.userId, endpoint.RUNNER, pubKey, roles.DONOR, ethAccount)
       
        if (undefined !== error)
            return ({error:error});
       
        var dataJson = {
            session_id:userStatus[userStIdx].enclave_session[sessionIdx].id,
            msgType:msgType.DELETE_DATA,
            role:roles.DONOR,
            dataName:dataName,
            ethAddress:ethAccount
        }
       
        const json = sendData2Enclave(this.userId,enclave,sessionIdx,dataJson)
        if (json.error)
            return json
        if (msgType.DELETE_DATA_RSP_SUCCESS === json.msgType){
            let dataInfo = {
                dataName: dataName,
                ethAddress: ethAccount,
                enclavePubKey: pubKey,
                username: Meteor.users.findOne({_id: this.userId}).username,
                timestamp: new Date().getTime()
            }
            // deRegisterDataByOwner(ethAccount, dataInfo)
            return ({result:"Delete shared data successed"})
        }
        else
            return ({error:"Delete data from enclave failed."})
    }
})
