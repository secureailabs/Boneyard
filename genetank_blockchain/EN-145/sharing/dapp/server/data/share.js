import { homeDir } from '../account/account.js';
import { userStatus } from '../account/acc_status.js';
import { enclaveCheck, sendData2Enclave } from '/server/model/enclave.js';
import { msgType } from '/imports/api/msg_type.js'
import { endpoint, roles } from '/imports/api/enum.js'
import { EJSON } from 'meteor/ejson'

var fs = require('fs')

export function shareData(uid, pubKey, dataIdx, ethAccount) {
      var {error, cfg, userStIdx, enclaveIdx, sessionIdx}
        =  enclaveCheck(uid, endpoint.RUNNER, pubKey, roles.DONOR, ethAccount)

      if (undefined !== error)
        return ({error:error});
    const dataInfo = EJSON.parse(cfg.data[dataIdx].infoJson)

    const cfgData = cfg.data[dataIdx]
    const filename = homeDir + "/" + uid + "/data/" +
      cfgData._id + "_data_" +cfgData.filename
    const data = fs.readFileSync(filename)

    var dataJson = {
        session_id:userStatus[userStIdx].enclave_session[sessionIdx].id,
        msgType:msgType.UPLOAD_DATA,
        role:roles.DONOR,
        dataName:dataInfo.name,
        ethAddress:ethAccount,
        data:data.toString('base64'),
    }
    const json = sendData2Enclave(uid, enclaveIdx, sessionIdx, dataJson)
    if (json.error)
      return json
    if (msgType.UPLOAD_DATA_RSP_SUCCESS === json.msgType)
      return ({receipt:json.receipt})
    else
      return ({error:"Sharing data to enclave failed."})
}
