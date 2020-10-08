import { Meteor } from 'meteor/meteor';
import { Mongo } from 'meteor/mongo';
import { userCfgDb, getUserCfg } from '../account/account.js';
import { userStatus, getUserSt } from '../account/acc_status.js';
import { enclaveCheck,setupSession } from '/server/model/enclave.js';
import { sendData2EnclaveASync } from '/server/model/test.js';
import { testDataUploadASync } from '/server/model/test.js';
import { sendData2Enclave } from '/server/model/enclave.js';
import { testSessionASync } from '/server/model/test.js';
import { msgType } from '/imports/api/msg_type.js'
import { endpoint, roles } from '/imports/api/enum.js'
import { EJSON } from 'meteor/ejson'
import { Random } from 'meteor/random'

import { calculateECSign } from "../data/ecdsaSignature";
import {createDappPublicKey, computeDHKey} from "../computeDHKey";
import {aes_encrypt, aes_decrypt } from "../aes_encryption.js"
import {verifyAttestation} from "../data/attestation";

Meteor.methods({
  runTestOnServer(pubKey) {
	return sendData2EnclaveASync(this.userId,pubKey)
},
	runTestOnServerSession(pubKey,testCode){
		return testSessionASync(this.userId, endpoint.RUNNER,pubKey,roles.RUNNER,testCode)
},
	runTestOnServerDataUpload(pubKey){
		return testDataUploadASync(this.userId,pubKey)
}
})
