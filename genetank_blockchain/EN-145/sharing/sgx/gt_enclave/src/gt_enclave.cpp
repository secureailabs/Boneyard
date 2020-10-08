/*
 *   Copyright(C) 2011-2017 Intel Corporation All Rights Reserved.
 *
 *   The source code, information  and  material ("Material") contained herein is
 *   owned  by Intel Corporation or its suppliers or licensors, and title to such
 *   Material remains  with Intel Corporation  or its suppliers or licensors. The
 *   Material  contains proprietary information  of  Intel or  its  suppliers and
 *   licensors. The  Material is protected by worldwide copyright laws and treaty
 *   provisions. No  part  of  the  Material  may  be  used,  copied, reproduced,
 *   modified, published, uploaded, posted, transmitted, distributed or disclosed
 *   in any way  without Intel's  prior  express written  permission. No  license
 *   under  any patent, copyright  or  other intellectual property rights  in the
 *   Material  is  granted  to  or  conferred  upon  you,  either  expressly,  by
 *   implication, inducement,  estoppel or  otherwise.  Any  license  under  such
 *   intellectual  property  rights must  be express  and  approved  by  Intel in
 *   writing.
 *
 *   *Third Party trademarks are the property of their respective owners.
 *
 *   Unless otherwise  agreed  by Intel  in writing, you may not remove  or alter
 *   this  notice or  any other notice embedded  in Materials by Intel or Intel's
 *   suppliers or licensors in any way.
 *
 */


#include "../gt_enclave_t.h"
#include "sgx_tkey_exchange.h"
#include "sgx_tcrypto.h"
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include "sgx_tprotected_fs.h"
#include "sgx_utils.h"

#include "data_management.h"
#include "dir_management.h"

#include "msg_def.h"
#include "mtcsv2R.h"
#include "gt_enclave.h"

#include "rvm.h"
#include "pyvm.h"
#include "vmoutput.h"
#include "Python.h"
#include "cluster.h"
#include "util.h"
#include "crypto.h"
#include "session.h"
#include "conversion.h"

static bool pyInitialized=false;
static bool R_Initialized=false;
#ifdef __cplusplus
 extern "C" {
#endif
void set_module_search_path(char *path);
#ifdef __cplusplus
 }
#endif
sgx_status_t enclave_sign(const uint8_t* pData, size_t dataSize, uint8_t hash[32], uint8_t *pubkey, size_t pkSize, uint8_t *pSign, size_t signSize){
	sgx_ecc_state_handle_t ecc_handle;
	sgx_ec256_private_t enclave_private;
	//sgx_ec256_public_t enclave_public;
	//sgx_ec256_signature_t enclave_sign;
	if (sizeof(sgx_ec256_public_t) > pkSize || sizeof(sgx_ec256_signature_t) > signSize)
		return (sgx_status_t)-1;
	sgx_status_t ret = sgx_sha256_msg(pData, dataSize, (sgx_sha256_hash_t *)hash);
	if (SGX_SUCCESS != ret)
		return (ret);
	if (SGX_SUCCESS != dmGetPrivateKey(&enclave_private))
		return (sgx_status_t)-1;
	if (SGX_SUCCESS != dmGetPublicKey((sgx_ec256_public_t *)pubkey))
		return (sgx_status_t)-1;

	ret = sgx_ecc256_open_context(&ecc_handle);
	if (ret != SGX_SUCCESS)
		return ret;
	ret = sgx_ecdsa_sign(hash, 32, &enclave_private, (sgx_ec256_signature_t *)pSign, ecc_handle);
	if (ret != SGX_SUCCESS) {
		sgx_ecc256_close_context(ecc_handle);
		return ret;
	}
	ret = sgx_ecc256_close_context(ecc_handle);
	return ret;
}

sgx_status_t ec_R_clean(){
	//R_Initialized = false;
	return SGX_SUCCESS;
}
sgx_status_t R_init() {
	if (R_Initialized)
		return SGX_SUCCESS;
    Rf_initialize_R();
    setup_Rmainloop();

    R_Initialized = true;
    return SGX_SUCCESS;
}

PyMODINIT_FUNC initcparse(void);

sgx_status_t Py_init(char *model) {
	if (pyInitialized)
		return SGX_SUCCESS;
	envlist = (char**)calloc(ENVLIMIT, sizeof(char*));

	stdin=sgx_fopen_auto_key("stdin","w");
    stdout=sgx_fopen_auto_key("stdout","w");
    stderr=sgx_fopen_auto_key("stderr","w");

    add_native();
    add_numpy();
    add_numpy_tests();
    add_scipy();
    add_sklearn();
    add_pandas();
    add_matplotlib();
    PyImport_AppendInittab("initcparse", initcparse);

    Py_Initialize();

    if (model != NULL)
    	set_module_search_path(model);

	pyInitialized = true;
    return SGX_SUCCESS;
}

sgx_status_t ec_Py_clean(){
	if (!pyInitialized)
		return SGX_SUCCESS;
    Py_Finalize();

	sgx_fclose(stdin);
	sgx_fclose(stdout);
	sgx_fclose(stderr);

	for(int i=0;i<ENVLIMIT;i++)
		if(envlist[i]!=NULL)
		    free(envlist[i]);
	free(envlist);
	pyInitialized = false;
	return SGX_SUCCESS;
}

sgx_status_t ec_enclave_reset(){
	char folder[]="/folderNotExist";
	dmInitDirSetting(folder, folder);

	ec_R_clean();

	//ec_Py_clean();

	resetSession();

	return SGX_SUCCESS;
}

sgx_status_t ec_enclave_init(const char *home, size_t path_size, uint32_t stakeholderId, sgx_enclave_id_t eid,
		const uint8_t *message, size_t msgLen, uint8_t *iv, size_t iv_len,
		uint8_t *respBuff, size_t respBuffSize, size_t *respSize) {

	if (SGX_SUCCESS != dmInit(home))
		return (sgx_status_t)-1;

	char sandboxBase[]="/data";
	char sandboxTrainBase[]="/data/training";
	char *sandbox=sandboxBase;
	char initModelPath[]="/lib/:/model/"; //paths for module searching
	char *model=initModelPath;
	sgx_key_128bit_t startKey;
	bool initSessionTab=(0 == msgLen);

	bool success = false;

	if (msgLen>0){
		StakeholderRec_t stRec;
		stRec.seqNumber = stakeholderId;
		if (SGX_SUCCESS == dmReadStake(&stRec)){
      if(stRec.isDeleted) {
    		return (sgx_status_t)(-1);
    	}
			uint8_t dec_iv[16];
			memcpy(dec_iv, iv, 16);
			uint8_t *decMsg = decryptData(message, msgLen+1, stRec.encKey, dec_iv);
			if (decMsg != NULL){
				*(decMsg+msgLen) = '\0';
				cJSON *msgJson = cJSON_Parse((const char *)decMsg);
				free(decMsg);
				if (msgJson != NULL){
					uint32_t secureStId;
					if (SGX_SUCCESS == getJsonNumber(msgJson, "stakeholderId", &secureStId)){
						char *sandboxStr = sandboxTrainBase;
						char *startKeyStr = getJsonStringPtr(msgJson, "startKey");
						if (NULL != startKeyStr)
								hex2bin(startKeyStr, (uint8_t *)&startKey, sizeof(sgx_key_128bit_t));
						char *initSessionTabStr = getJsonStringPtr(msgJson, "initSession");
						if (NULL != initSessionTabStr){
							initSessionTab = (*initSessionTabStr == 'y') || (*initSessionTabStr == 'Y');
						}
						success = (stakeholderId == secureStId) && (NULL != startKeyStr);
					}
					free(msgJson);
				}
			}
		}
		if (!success)
			return (sgx_status_t)-1;
	}

	uint32_t apiPermissions;
	//Temporarily provide all permissions for debugging
	apiPermissions = RUN_INTERACTIVE_SCRIPT | CALL_HTTP | RUN_FEDERATED_CYCLE | SUBMIT_FEDERATED_DATA | RETRIEVE_FEDERATED_DATA | CREATE_CHILD_ENCLAVE
	  | PRINT_PLOT;

	sgx_key_128bit_t connKey;
	sgx_status_t ret = sgx_read_rand((unsigned char *)&connKey, sizeof(sgx_key_128bit_t));
	if (SGX_SUCCESS != ret){
		return ret;
	}
	if (success){
		sandbox = sandboxTrainBase;
		char respMsg[256];
		char keyHex[33];
		bin2hex((const uint8_t *)&connKey, 16, keyHex);
		keyHex[32] = '\0';
		snprintf(respMsg, 256, "{\"destEID\":%lu, \"connKey\":\"%s\"}", eid, keyHex);
		size_t msgLen = strnlen(respMsg, 255)+1;
		uint8_t *encBuff = encryptData((uint8_t *)respMsg, msgLen, startKey, iv, false);
		memcpy(respBuff, encBuff, msgLen);
		*respSize = msgLen;
		free(encBuff);
	} else
		*respSize = 0;

	if (SGX_SUCCESS != initSession(stakeholderId, eid, apiPermissions, initSessionTab, connKey))
		return (sgx_status_t)-1;

	//dmInitDirSetting(sandbox, model);

	if (HOST_STAKEHOLDER_ID == stakeholderId) //R is available for host user only
		R_init();

	if (0 != stakeholderId)
		Py_init(model);

	return SGX_SUCCESS;
}

sgx_status_t get_report(sgx_target_info_t *p_target, sgx_report_t *p_report) {
	sgx_status_t ret;
	sgx_report_data_t report_data;

	sgx_ec256_public_t pubkey;
	ret = dmGetPublicKey(&pubkey);

	memcpy(report_data.d, pubkey.gx, 32);
	memcpy(report_data.d+32, pubkey.gy, 32);
	ret = sgx_create_report(p_target, &report_data, p_report);

	return ret;
}

sgx_status_t enclave_set_eth(eth_addr_t *pRunnerEthAddr, eth_addr_t *pHostUserEthAddr, uint32_t *stId){
  int ret1 = dmAddUserEthAddr(pRunnerEthAddr, 0, 0);
  if (ret1<0) { return (sgx_status_t)-1; }

  int ret2 = dmAddUserEthAddr(pHostUserEthAddr, *stId, 1);
	if (ret2 < 0) { return (sgx_status_t)-1; }
	else if (ret2 > 0){
		dmInitDataDir(ret2);
	}

	int ret3 = dmAddUserEthAddr(pHostUserEthAddr, *stId, 2);
	if (ret3<0) { return (sgx_status_t)-1; }

	int ret4 = dmAddUserEthAddr(pHostUserEthAddr, *stId, 3);
	if (ret4<0) { return (sgx_status_t)-1; }

	// for user group feature
	sgx_status_t retval;
  ocall_update_enclave_node(&retval, 0, ROLE_HOST);
	ocall_update_enclave_node(&retval, *stId, ROLE_HOST);

	return (sgx_status_t)ret1;
}

sgx_status_t ec_get_pubkey(char * enclavePublic, size_t len){
	sgx_ec256_public_t pubkey;

	if (SGX_SUCCESS != dmGetPublicKey(&pubkey))
		return (sgx_status_t)-1;

	memcpy(enclavePublic, (const char *)&pubkey, len>sizeof(sgx_ec256_public_t)?sizeof(sgx_ec256_public_t):len);

	return SGX_SUCCESS;
}

sgx_status_t ec_getStakeholder(const char * ethPubkeyStr, uint32_t *stId, uint32_t *roleId){
	uint8_t ethAddr[20];

	uint8_t ethPubkey[65];
	hex2bin((const char *)ethPubkeyStr, ethPubkey, 65);
	sgx_status_t ret = pubKey2EthAddr(ethPubkey+1, ethAddr);

	*stId=-1;
	if (SGX_SUCCESS != dmSearchRole(ethAddr, stId, roleId))
		return (sgx_status_t)-1;

	return SGX_SUCCESS;
}

sgx_status_t ec_lib_imported(const char *path, size_t size, int *imported){
    uint32_t keyId, stId;
    int32_t dirId;
    DirRec_t dirRec;

    if (SGX_SUCCESS == dmFindDirEntry("lib.dir", path, &keyId, &dirId, &dirRec, &stId)){
    	*imported=1;
    } else {
    	*imported=0;
    }
	return SGX_SUCCESS;
}

sgx_status_t ec_import_lib(const char *fileName, size_t nameSize, const uint8_t *buff, size_t dataSize){
  sgx_status_t ret = dmWriteDataOrModel("lib.dir", fileName, buff, dataSize,0,0);
	return ret;
}

sgx_status_t ec_test_imported(const char *path, size_t size, int *imported){
    uint32_t keyId, stId;
    int32_t dirId;
    DirRec_t dirRec;

    if (SGX_SUCCESS == dmFindDirEntry("test.dir", path, &keyId, &dirId, &dirRec, &stId)){
    	*imported=1;
    } else
    	*imported=0;
	return SGX_SUCCESS;
}

sgx_status_t ec_import_test(const char *fileName, size_t nameSize, const uint8_t *buff, size_t dataSize){
	sgx_status_t ret = dmWriteDataOrModel("test.dir", fileName, buff, dataSize,0,0);
	return ret;
}

sgx_status_t ec_test_code(const char* code, size_t code_size, char* result, size_t result_size, const char* type){
	sgx_status_t ret = SGX_SUCCESS;
	//run_code(code, result, result_size, type);
	memcpy(result, run_py(code, result, result_size), result_size);
	return ret;
}

void run_code(const char *code, char *result, size_t result_size, const char* code_type){
	//code_type=0 run R code
	if(strcmp(code_type, "0")==0){
        run_R_code(code, result, result_size);
	}
	//code_type=1 run python code
	else if(strcmp(code_type, "1")==0){

        run_py(code, result, result_size);
	}
}

sgx_status_t ec_send_in_data(const uint8_t *inBuf, size_t bufSize, uint8_t **dstPtr){
	uint8_t *ptr = (uint8_t *)malloc(bufSize+1);
	if (NULL == ptr)
		return (sgx_status_t)-1;
	memcpy(ptr, inBuf, bufSize);
	ptr[bufSize]='\0';
	*dstPtr = ptr;
	return SGX_SUCCESS;
}
