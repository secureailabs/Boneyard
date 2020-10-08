/*
 * session.cpp
 *
 *  Created on: Nov 19, 2018
 *      Author: accuser
 */

#include "string"
#include <stdio.h>
#include <stdlib.h>

#include "gt_enclave_t.h"

#include "sgx_utils.h"
#include "sgx_tcrypto.h"

#include "session.h"
#include "data_management.h"
#include "dir_management.h"
#include "data_share.h"
#include "enclave_view.h"
#include "task_manager_enclave.h"

#include "data_inspector.h"
#include "gt_enclave.h"
//#include "rvm.h"
//#include "./pyvm/Include/Python.h"

#include "cluster.h"
#include "projects.h"

#include "queue.h"
#include "crypto.h"

#include "stream.h"
#include "util.h"

#include "conversion.h"

sgx_thread_mutex_t sessionIdUpdateLock = SGX_THREAD_MUTEX_INITIALIZER;
sgx_thread_mutex_t sessionTableUpdateLock = SGX_THREAD_MUTEX_INITIALIZER;
sgx_thread_mutex_t vmBufferLock = SGX_THREAD_MUTEX_INITIALIZER;

static SessionTable *sessionTable=NULL;
static uint32_t userStakeholderId=-1;
static sgx_enclave_id_t enclaveId;
static uint32_t apiPermissions=0;
static sgx_key_128bit_t connectionKey;

static uint8_t *runCmdResult=NULL;
static size_t runCmdResultLen=0;

sgx_enclave_id_t getEnclaveId(){
	return enclaveId;
}

#define MAX_SEND_SIZE 1024*1024*1024
char *oc_http_send(const char *req){
	sgx_status_t ret, retval;
	uint8_t *rspPtr = NULL;
	uint8_t *rspBuf = NULL;
	size_t rspSize = 0;
	ret = ocall_http_send(&retval, req, strnlen(req, MAX_SEND_SIZE), &rspPtr, &rspSize);
	if (SGX_SUCCESS == ret && SGX_SUCCESS == retval){
		if (0 != rspSize && NULL != rspPtr){
			rspBuf = (uint8_t *)malloc(rspSize+1);
			if (NULL == rspBuf){
				ocall_get_data(&retval, &rspPtr, NULL, 0); // free rspPtr
			} else {
				ret = ocall_get_data(&retval, &rspPtr, rspBuf, rspSize);
				*(rspBuf+rspSize) = '\0';
			}
		}
	}
	return (char *)rspBuf;
}

sgx_status_t initSession(uint32_t stId, sgx_enclave_id_t eid, uint32_t permissions, bool initSessionTab, sgx_key_128bit_t connKey){
	if (-1 != userStakeholderId || NULL != sessionTable) //preventing duplicated initialization
		return (sgx_status_t)-1;
	if (initSessionTab) {
		sessionTable = new SessionTable();
		if (NULL == sessionTable){
			return (sgx_status_t)-1;
		}
	}
	stId = stId & 0x0ffff;
	userStakeholderId = stId;
	enclaveId = eid;
	apiPermissions = permissions;
	memcpy(connectionKey, connKey, sizeof(sgx_key_128bit_t));
	return SGX_SUCCESS;
}

void resetSession(){
	if (NULL != sessionTable){
		delete sessionTable;
		sessionTable = NULL;
	}
	userStakeholderId = -1;
	enclaveId = 0;
	apiPermissions = 0;
	memset(connectionKey, 0, sizeof(sgx_key_128bit_t));
	if (NULL != runCmdResult)
		free(runCmdResult);
	runCmdResultLen = 0;
}
/*
 * Constructor to initialize the SessionTable class
 * Populates a list of SESSION_TABLE_SIZE number of sessions nodes with default values
 */
SessionTable::SessionTable(){
	for(int i=0; i<SESSION_TABLE_SIZE; i++ )
		pSessions[i]=NULL;
	lastSessionId=1;
	lastScanSecond=0;
	scanCycleTime=60;
}
/*
 * Destructor to destroy the session Table object of SessionTable class
 */
SessionTable::~SessionTable(){
	LOCK(&sessionTableUpdateLock);
	for(int i=0; i<SESSION_TABLE_SIZE; i++ ){
		SessionNode *node = pSessions[i];
		while (NULL != node){
			SessionNode *n = node->next;
			delete node;
			node = n;
		}
	}
	UNLOCK(&sessionTableUpdateLock);
}
/**
 * Scans the table for session nodes,
 *     curentSecond : the current second on the clock
 */
void SessionTable::scanTable(uint64_t currentSecond){
	LOCK(&sessionTableUpdateLock);
	for(int i=0; i<SESSION_TABLE_SIZE; i++ ){
		SessionNode *node = pSessions[i];
		SessionNode **pp = &(pSessions[i]);
		while (NULL != node){
			SessionNode *n = node->next;
			if ((node->confirmed && (currentSecond - node->lastActiveSecond)>60*scanCycleTime) ||
					(!node->confirmed && (currentSecond - node->lastActiveSecond)>scanCycleTime)){
				*pp = n;
				delete node;
			} else
				pp = &(node->next);
			node = n;
		}
	}
	lastScanSecond = currentSecond;
	UNLOCK(&sessionTableUpdateLock);
}

/**
 * Insert a node into the table
 *     node : the node to be inserted
 */
sgx_status_t SessionTable::insertNode(SessionNode *node){
	LOCK(&sessionTableUpdateLock);
	node->sessionId = ((lastSessionId++) & 0x0ffff) + (userStakeholderId << 16);
	int index = node->sessionId%SESSION_TABLE_SIZE;
	SessionNode *ptr = pSessions[index];
	while (NULL != ptr){
		if (ptr->sessionId == node->sessionId){// There is a session id conflic.
			UNLOCK(&sessionTableUpdateLock);
			return (sgx_status_t)-1;
		}
		ptr = ptr->next;
	}
	node->next = pSessions[index];
	pSessions[index] = node;
	UNLOCK(&sessionTableUpdateLock);
	return SGX_SUCCESS;
}

/*
 * Get session node with the given session id
 *     sId : the id whose session node is summoned
 *     currentSecond : current second on the clock
 */
SessionNode * SessionTable::getNode(uint32_t sId, uint64_t currentSecond){
	SessionNode *node;
	int index = sId%SESSION_TABLE_SIZE;
	node =  pSessions[index];
	while (NULL != node){
		if (node->sessionId == sId){
			node->lastActiveSecond = currentSecond;
			return node;
		}
		node = node->next;
	}
	return NULL;
}

sgx_status_t SessionTable::deleteNode(uint32_t sId){ //need a lock
	LOCK(&sessionTableUpdateLock);
	SessionNode *node;
	int index = sId%SESSION_TABLE_SIZE;
	node =  pSessions[index];
	SessionNode **pp = &(pSessions[index]);
	while (NULL != node){
		if (node->sessionId == sId){
			*pp = node->next;
			return SGX_SUCCESS;
		}
		pp = &(node->next);
		node = node->next;
	}
	UNLOCK(&sessionTableUpdateLock);
	return (sgx_status_t)-1;
}
/*
 * Get stake holder id with the given thread id
 *     threadId : thread id which gives the stake holder id
 */
uint32_t getStakeHolderId(){
	return userStakeholderId;
}

uint8_t * ssNewSession(cJSON *msgJson, uint32_t currentSecond, size_t *pResultLen, uint8_t *iv, size_t ivLen, uint32_t *pSessionId){
	sgx_status_t ret=SGX_SUCCESS;

	//char buff[256];
	uint32_t msgType;
	if (SGX_SUCCESS != getJsonNumber(msgJson, "msgType", &msgType)){
		return NULL;
	}
	if (SESSION_INIT != msgType){
		return NULL;
	}
	char * ethPubkey = getJsonStringPtr(msgJson, "ethPubKey");
	if (NULL == ethPubkey){
		return NULL;
	}
	uint32_t stId, roleId;
	if (SGX_SUCCESS != ec_getStakeholder(ethPubkey, &stId, &roleId))
		return NULL;

	uint8_t rspBuf[1024], orig_iv[CTR_LEN];
	size_t bufLen = 1024;
	char *message = cJSON_PrintUnformatted(msgJson);
	if (NULL == message)
		return NULL;

	size_t msgLen = strnlen(message, bufLen)+1;

	if (roleId != userStakeholderId && roleId != 0){//need to connect to other enclave
		uint8_t *outBuf = (uint8_t *)malloc(bufLen);
		if (NULL == outBuf)
			return NULL;
		size_t rspLen;
		sgx_status_t retval;
		ret = ocall_new_session(&retval, roleId, message, msgLen, pSessionId, iv, ivLen, outBuf, bufLen, pResultLen);
		//ocall_new_session(uint32_t stId, const char *message, size_t msgLen, uint32_t *pSessionId, uint8_t *iv, size_t ivLen,
		//		uint8_t *outBuf, size_t bufLen, size_t *pResultLen)
		if (SGX_SUCCESS == ret && SGX_SUCCESS == retval){
			return outBuf;
		}
		free(outBuf);
		return NULL;
	}
	secp256k1_ecdsa_signature sig;

	char *sign = getJsonStringPtr(msgJson, "sign");
	if (NULL == sign){
		return NULL;
	}
	hex2bin(sign, sig.data, sizeof(secp256k1_ecdsa_signature));

	uint8_t serializedPubkey[65];
	char *pubKey = getJsonStringPtr(msgJson, "pubkey");
	if (NULL == pubKey){
		return NULL;
	}
	hex2bin(pubKey, serializedPubkey, 65);

	SessionNode *node = new SessionNode(currentSecond);
	if (NULL == node)
		return NULL;

	node->role = (roleId < 2) ? roleId : 4; //0: runner; 1:host; 4:guest
	node->stakeholderId = stId;

	//Temporary remove the signature checking for E2E debugging
	//if (SGX_SUCCESS != verifyEthereumSign(&sig, ethPubkey, buff, strnlen(buff, 255)))
	//	return NULL;

	sgx_ec256_private_t privkey;
	if (SGX_SUCCESS != dmGetPrivateKey(&privkey)){
		delete node;
		return NULL;
	}

	ret = gen_ecdh_key(serializedPubkey, privkey, node->secretKey);

	if (NULL == sessionTable || SGX_SUCCESS != sessionTable->insertNode(node)){
		delete node;
		return NULL;
	}

	snprintf((char *)rspBuf, bufLen, "{\"session_id\":%d,\"msgType\":%d,\"ack\":\"OK\"}", node->sessionId,
			SESSION_INIT_RSP);
	*pSessionId = node->sessionId;

	*pResultLen=strnlen((const char *)rspBuf, bufLen);
	uint8_t *encBuff = encryptData(rspBuf, *pResultLen, node->secretKey, orig_iv, true);

	if (NULL == encBuff){
		return NULL;
	}
	memcpy(iv, orig_iv, ivLen>CTR_LEN?CTR_LEN:ivLen);

	return encBuff;
}

sgx_status_t ec_new_session(uint32_t *pSessionId, const char *message, size_t len, void *resVecPtr,
		uint8_t *iv, size_t ivLen, uint64_t currentSecond){
	cJSON *msgJson;

	msgJson = cJSON_Parse(message);
	if (NULL == msgJson)
		return (sgx_status_t)-1;

	size_t resultLen;
	uint8_t *pRes = ssNewSession(msgJson, currentSecond, &resultLen, iv, ivLen, pSessionId);
	if (NULL == pRes){
		cJSON_Delete(msgJson);
		return (sgx_status_t)-1;
	}
	sgx_status_t oc_ret;
	ocall_send_result(&oc_ret, pRes, resultLen, resVecPtr);

	free(pRes);

	cJSON_Delete(msgJson);
	return SGX_SUCCESS;
}

//Simple remote attestation:
// 1. The application calls the enclave to create a report of the enclave
// 2. The application calls QE to generate a quote for the report
// 3. The application sends the quote to the service provider with HTTP POST request
//    and the quote JSON as the body of the request
// 4. The service provider (SAIL) calls Intel IAS to verify the quote and response
//    the signed attestation report
// 5. The service provider response the application with the attestation report
// 6. The application sends the attestation report to the runner Dapp
sgx_status_t simple_ra(const char *server_url, const char *spid, char  *attestation_report, size_t reportSize) {
	sgx_status_t ret;
	sgx_target_info_t target_info;
	sgx_epid_group_id_t gid;
	sgx_report_t report;
	sgx_spid_t spid_a;// = { 0x7B,0xC0,0x33,0x8D,0x24,0x78,0x1F,0x74, 0x48,0x75,0x17,0x8F,0xE7,0x38,0x1F,0x76 };
	sgx_status_t retval;

	//if (strnlen(spid, 50) != 16 * 2 + 2)
	//	return SGX_ERROR_UNEXPECTED;//something wrong in the SPID

	hex2bin(spid, spid_a.id, 16);

	//ret = sgx_init_quote(&target_info, &gid);
	ret = oc_init_quote(&retval, &target_info, &gid);

	if (SGX_SUCCESS == ret && SGX_SUCCESS == retval) {
		ret = get_report(&target_info, &report);
	}

	if (SGX_SUCCESS != ret) {
		return ret;
	}

	//bin2hex(report.body.mr_enclave.m, 32, mrenclave);
	//bin2hex(report.body.report_data.d, 64, pubkey);
    //sgx_status_t oc_sendAttReq([in, string] const char * server_url, [in] sgx_report_t *pReport, [in] sgx_spid_t *pSpid_a,
	//[out, string] char *attestation_report, size_t reportSize);

    ret = oc_sendAttReq(&retval, server_url, &report, &spid_a, attestation_report, reportSize);

	if (SGX_SUCCESS == ret && SGX_SUCCESS == retval) {
		return SGX_SUCCESS;
	}
	return (sgx_status_t)-1;
}

sgx_status_t runnerCmd(cJSON *msgJson, SessionNode *node, uint32_t msgType, char * rspBuf, size_t maxLen, size_t *pCurLen){
	sgx_status_t ret = SGX_SUCCESS;
	size_t curLen = *pCurLen;
	switch (msgType){
	case ATTESTATION:{
		char *serverUrl = getJsonStringPtr(msgJson, "server_url");
		if (NULL == serverUrl){
			return (sgx_status_t)-1;
		}
		char *spidPtr = getJsonStringPtr(msgJson, "spid");
		if (NULL == spidPtr){
			return (sgx_status_t)-1;
		}

		char result[20000];
		//size_t resultSize = 20000-1;
		//utility::string_t server_url = obj.at(U("server_url")).as_string();
		//utility::string_t spid = obj.at(U("spid")).as_string();

		ret = simple_ra(serverUrl, spidPtr, result, 20000-1);
		if (ret != SGX_SUCCESS) {
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", ATTESTATION_RSP_FAIL);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		}
		else {
			//printf("Attestation succeeded. \n");
			//rply_obj[U("msgType")]=json::value(ATTESTATION_RSP_SUCCESS);
			//rply_obj[U("Report")] = json::value::parse(result);
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", ATTESTATION_RSP_SUCCESS);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"Report\":%s", result);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		}
	  }
		break;
		case ADD_HOST_USER: {
			char *addrPtr;
			eth_addr_t ethAddr;
			eth_addr_t runnerAddr;
			uint32_t stId;
			addrPtr = getJsonStringPtr(msgJson, "ethAddress");
			if (NULL == addrPtr){
				return (sgx_status_t)(-1);
			}
			hex2bin(addrPtr, ethAddr.addr, 20);
			addrPtr = getJsonStringPtr(msgJson, "runnerEth");
			if (NULL == addrPtr){
				return (sgx_status_t)(-1);
			}
			hex2bin(addrPtr, runnerAddr.addr, 20);
			if (SGX_SUCCESS != getJsonNumber(msgJson, "stId", &stId)) {
				ret = (sgx_status_t)(-1);
			}
			enclave_set_eth(&runnerAddr, &ethAddr, &stId);
			if (SGX_SUCCESS != ret) {
				snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", ADD_HOST_USER_RSP_FAIL);
				curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
			} else {
				snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", ADD_HOST_USER_RSP_SUCCESS);
				curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
			}
		}
		break;
	case UPDATE_HOST_USER: {
			eth_addr_t runnerAddr;
			eth_addr_t ethAddress;
			StakeholderRec_t stRec;
			StakeholderRec_t oldRec;
			char *addrPtr = getJsonStringPtr(msgJson, "runnerEth");
			if (NULL == addrPtr){
				return (sgx_status_t)(-1);
			}
			hex2bin(addrPtr, runnerAddr.addr, 20);

			addrPtr = getJsonStringPtr(msgJson, "ethAddress");
			if (NULL == addrPtr){
				return (sgx_status_t)(-1);
			}
			hex2bin(addrPtr, ethAddress.addr, 20);
			hex2bin(addrPtr, stRec.ethAddr, 20);

			addrPtr = getJsonStringPtr(msgJson, "oldEthAddress");
			if (NULL == addrPtr){
				return (sgx_status_t)(-1);
			}
			hex2bin(addrPtr, oldRec.ethAddr, 20);

			int check = memcmp(oldRec.ethAddr, stRec.ethAddr, 20);

			if (SGX_SUCCESS != getJsonNumber(msgJson, "stId", &stRec.stId)) {
				ret = (sgx_status_t)(-1);
			}

			oldRec.stId = stRec.stId;
			ret = dmRemoveStake(&oldRec);
			for(int role = 1; role <= 3; role++) {
				if(SGX_SUCCESS == ret) {
					stRec.role = role;
					ret = dmUpdateStake(&stRec, false);
				} else { ret = (sgx_status_t)(-1); }
			}

			if(SGX_SUCCESS == ret) {
				uint32_t seqNumber;
				seqNumber = 1 + (3 * (stRec.stId - 1));
				ret = enclave_set_eth(&runnerAddr, &ethAddress, &stRec.stId);
				if(SGX_SUCCESS == ret) {
					dmInitDataDir(seqNumber);
				} else { ret = (sgx_status_t)(-1); }
			}

			if (SGX_SUCCESS != ret) {
				snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", UPDATE_HOST_USER_RSP_FAIL);
				curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
			} else {
				snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", UPDATE_HOST_USER_RSP_SUCCESS);
				curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
			}
		}
		break;
	case DELETE_HOST_USER: {
			StakeholderRec_t stRec;
			char *addrPtr = getJsonStringPtr(msgJson, "ethAddress");
			if (NULL == addrPtr){
				return (sgx_status_t)-1;
			}
			hex2bin(addrPtr, stRec.ethAddr, 20);
			if (SGX_SUCCESS != getJsonNumber(msgJson, "stId", &stRec.stId)) {
				ret = (sgx_status_t)(-1);
			}

			ocall_clear_enclave_node(&ret, stRec.stId);
			ret = dmRemoveStake(&stRec);

			if (SGX_SUCCESS != ret) {
				snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", DELETE_HOST_USER_RSP_FAIL);
				curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
			} else {
				snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", DELETE_HOST_USER_RSP_SUCCESS);
				curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
			}
		}
		break;
	case ADD_SUB_ENCLAVE:
	case DEL_SUB_ENCLAVE:
	do{
		//uint64_t cs;
		uint32_t sessionId;
		uint8_t secretKey[16];

		Cluster *pSubEnclave = new Cluster;
		// initiate session set up
		char *destUrl = getJsonStringPtr(msgJson, "desturl");
		if (NULL == destUrl){
			ret = (sgx_status_t)-1;
			break;
		}

		char *destPubkeyStr = getJsonStringPtr(msgJson, "pubkey");
		if (NULL == destPubkeyStr){
			ret = (sgx_status_t)-1;
			break;
		}

		char *name = getJsonStringPtr(msgJson, "name");
		if (NULL == name){
			ret = (sgx_status_t)-1;
			break;
		}

		sgx_ec256_public_t destPk;
		hex2bin(destPubkeyStr, (uint8_t *)&destPk, sizeof(sgx_ec256_public_t));

		Cluster *subEncl = NULL;
		if (NULL == enclaveCluster)
			enclaveCluster = loadEnclaveClusterFile();
		if (NULL != enclaveCluster)
			subEncl = enclaveCluster->searchSubEnclave(&destPk);

		if (ADD_SUB_ENCLAVE == msgType){
			if (NULL == subEncl){
				subEncl = new Cluster();
				if (NULL == subEncl){
					ret = (sgx_status_t)-1;
					break;
				}
				subEncl->addSubEnclave(&enclaveCluster);
			}
			memcpy(&(subEncl->subEnclave.pubKey), &destPk, sizeof(sgx_ec256_public_t));
			strncpy((char *)(subEncl->subEnclave.url), destUrl, strnlen(destUrl, URL_BUFF_SIZE-1)+1);
			strncpy((char *)(subEncl->subEnclave.name), name, strnlen(name, URL_BUFF_SIZE-1)+1);
		}
		else {
			if (NULL != subEncl)
				deleteSubEnclave(&enclaveCluster, subEncl);
		}

		saveEnclaveClusterFile(enclaveCluster);

		snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", msgType+1);
		curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
	} while(false);
	if (SGX_SUCCESS != ret){
		snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", msgType+2);
		curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
	}
	break;
	case ADD_GUEST_USER:{
		char *sign = getJsonStringPtr(msgJson, "signByRunner");
		//if (SGX_SUCCESS != verifySign(addrPrt, sing))
		//	ret = (sgx_status_t)-1;
		//	snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", ADD_GUEST_USER_FAIL);
		//	curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		//	break;
		//}
		cJSON *stakeholderJson = getJsonObject(msgJson, "userStakeHolders");
		if (NULL == stakeholderJson){ ret = (sgx_status_t)(-1); break; }

		char *addrPtr = getJsonStringPtr(msgJson, "ethAddress");
		if (NULL == addrPtr){
			ret = (sgx_status_t)-1;
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", ADD_GUEST_USER_FAIL);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
			break;
		}
		eth_addr_t ethAddr;
		hex2bin(addrPtr, ethAddr.addr, 20);

		int stId = dmAddUserEthAddr(&ethAddr, -1, ROLE_GUEST);
		sgx_status_t p1;
		if (stId < 0){
			ret = (sgx_status_t)-1;
		} else {
			ret = dmInitDataDir(stId);
			if (SGX_SUCCESS == ret){
				ocall_update_enclave_node(&ret, stId, ROLE_GUEST);
				SymLinkInfo_t symInfo;
				symInfo.blockNumber = 0;
				symInfo.fileId = SHAREDIN_INDEX;
				symInfo.stId = stId;
				int32_t dstFileId;
				uint32_t dstKeyId, dstStId;
				char dirName[MAX_PATH];
				char fileName[MAX_PATH];
				cJSON *hostJson;
				uint32_t id = -1;
				uint32_t seqNumber = -1;
				for(int i = 0; i < cJSON_GetArraySize(stakeholderJson); i++) {
					hostJson = cJSON_GetArrayItem(stakeholderJson, i);
					if (NULL != hostJson) {
						getJsonNumber(hostJson, "stId", &id);
						if (id > 0) {
							seqNumber = 1 + (3 * (id - 1));
							snprintf(dirName,MAX_PATH-1,"data%d.dir", seqNumber);
							toLowerCase(addrPtr, 42);
							if (*addrPtr == '0' && (*(addrPtr+1) == 'x'))
								addrPtr += 2;
							snprintf(fileName, MAX_PATH, "sharedout/%s", addrPtr);

							ret = dmAddDir(dirName, fileName, &symInfo, &dstKeyId, &dstStId, &dstFileId);
							if (SGX_SUCCESS != ret || -1 == dstFileId) {
								ret = (sgx_status_t)(-1);
								break;
							}
						}
					}
				}
			}
		}
		if (SGX_SUCCESS == ret){
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d,\"stId\":%d", ADD_GUEST_USER_SUCCESS, stId);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		} else {
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", ADD_GUEST_USER_FAIL);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		}
	}
	break;
	default:
		break;
	}

	*pCurLen = curLen;
	return ret;
}

sgx_status_t runOneSubCode(Cluster *subEncl, const char *name, const char *dataPath, const char *secAlgo){
	char msg2sub[256];
	//snprintf(msg2sub, 255, "{\"session_id\":0,\"msgType\":%d,\"sandboxPath\":\"/\", \"modelPath\":\"/\", \"bgfg\":\"true\", \
	//		\"command\":{\"session_id\":0,\"msgType\":%d,\"name\":\"%s\"}}",
	//		CREATE_ENCLAVE, RUN_MODEL_FILE, name);
	snprintf(msg2sub, 255, "{\"session_id\":0,\"msgType\":%d,\"name\":\"%s\",\"dataPath\":\"%s\",\"privateAlgo\":\"%s\",%s}",
			RUN_MODEL_FILE, name, dataPath, secAlgo,"\"bgfg\":\"y\"");
	size_t msgLen = strnlen(msg2sub, 255);

	subEncl->running = false;
	oc_print("Run one sub cycle request:", msg2sub);
	uint8_t *rspMsgBuf;
	sgx_status_t ret = sendE2Emessage(&(subEncl->subEnclave.pubKey), (uint8_t *)msg2sub, &rspMsgBuf, &msgLen);
	if (SGX_SUCCESS == ret) {
		oc_print("Run sub cycle response:", (const char *)rspMsgBuf);
		cJSON *rspJson = cJSON_Parse((char *)rspMsgBuf);
		free(rspMsgBuf);
		if (NULL != rspJson){
			uint32_t rspMsgType;
			if (SGX_SUCCESS == getJsonNumber(rspJson, "msgType", &rspMsgType)){
				if (START_TASK_SUCCESS == rspMsgType){
					uint32_t tidx;
					if (SGX_SUCCESS == getJsonNumber(rspJson, "tidx", &tidx)){
						subEncl->tidx = tidx;
						subEncl->running = true;
					}
				}
			}
			cJSON_Delete(rspJson);
		}
	}
	return ret;
}

void runAllSubCode(Cluster *subEncl, char *name, const char *dataPath){
	Cluster *tmpEncl = subEncl;
	while (NULL != tmpEncl){ //wake up sub enclaves
		if (0 == tmpEncl->sessionId)
			initE2Esession((char *)(tmpEncl->subEnclave.url), &(tmpEncl->sessionId),
					&(tmpEncl->subEnclave.pubKey), tmpEncl->secretKey);
		tmpEncl = tmpEncl->next;
	}
	while (NULL != subEncl){
		runOneSubCode(subEncl, name, dataPath, "");
		subEncl = subEncl->next;
	}
}

bool sendDataToSub(Cluster *subEncl, int msgType, int enclaveID, uint32_t dataType, char *data, size_t dataLen) {
	char msg2sub[256];
	size_t msgLen;
	uint8_t *rspMsgBuf;
	uint32_t rspMsgType;

	while(enclaveID-- != 0) {
		if (subEncl == NULL) {
			return false;
		}
		subEncl = subEncl->next;
	}

    while (!subEncl->running) {
		sgx_status_t retv;
		ocall_sleep(&retv, 2);
	}

	snprintf(msg2sub, 255, "{\"session_id\":0,\"msgType\":%d,\"tidx\":%d,\"dataType\":%d,\"dataLen\":%lu,\"data\":\"%s\"}",
			msgType, subEncl->tidx, dataType, dataLen, data);
	msgLen = strnlen(msg2sub, 255);

	sgx_status_t ret = sendE2Emessage(&(subEncl->subEnclave.pubKey),
                                     (uint8_t *)msg2sub, &rspMsgBuf, &msgLen);
	if (SGX_SUCCESS != ret) {
		return false;
	}

	cJSON *rspJson = cJSON_Parse((const char *)rspMsgBuf);
	free(rspMsgBuf);
	if (NULL == rspJson){
		return false;
	}
	if (SGX_SUCCESS != getJsonNumber(rspJson, "msgType", &rspMsgType)){
		cJSON_Delete(rspJson);
		return false;
	}
	if (SIMFL_DATA_HUB_TO_SUB_SUCCESS != rspMsgType){
		cJSON_Delete(rspJson);
		return false;
	}

	cJSON_Delete(rspJson);
	return true;
}

bool getDataFromSub(Cluster *subEncl, int msgType, int enclaveID, uint32_t dataType){
	char msg2sub[256];
	size_t msgLen;
	uint8_t *rspMsgBuf;
	uint32_t rspMsgType;

	while(enclaveID-- != 0) {
		if (subEncl == NULL) {
			return false;
		}
		subEncl = subEncl->next;
	}

    while (!subEncl->running) {
		sgx_status_t retv;
		ocall_sleep(&retv, 2);
	}

	snprintf(msg2sub, 255, "{\"session_id\":0,\"msgType\":%d,\"dataType\":%d,\"tidx\":%d}",
			msgType, dataType, subEncl->tidx);
	msgLen = strnlen(msg2sub, 255);
	sgx_status_t ret = sendE2Emessage(&(subEncl->subEnclave.pubKey),
                                     (uint8_t *)msg2sub, &rspMsgBuf, &msgLen);
	if (SGX_SUCCESS != ret) {
		return false;
	}

	cJSON *rspJson = cJSON_Parse((const char *)rspMsgBuf);
	free(rspMsgBuf);
	if (NULL == rspJson){
		return false;
	}
	if (SGX_SUCCESS != getJsonNumber(rspJson, "msgType", &rspMsgType)){
		cJSON_Delete(rspJson);
		return false;
	}
	if (SIMFL_DATA_SUB_TO_HUB_SUCCESS != rspMsgType){
		cJSON_Delete(rspJson);
		return false;
	}

	char *output = getJsonStringPtr(rspJson, "SimFLData");
	if (NULL != output) {
		uint32_t inLen;
		size_t outLen;
		if (SGX_SUCCESS == getJsonNumber(rspJson, "dataSize", &inLen)) {
			unsigned char *decOut = base64_decode(output, inLen, &outLen);
			if (NULL != decOut){
				subSimFlLSHQueue.enQueue((uint8_t *)decOut, outLen, dataType);
			}
		}
	} else {
		return false;
	}
	cJSON_Delete(rspJson);
	return true;
}

bool checkSubStatus(Cluster *subEncl){
	char msg2sub[256];
	size_t msgLen;
	bool allFinished = true;
	while (NULL != subEncl){
		if (!subEncl->running){
			subEncl = subEncl->next;
			continue;
		}

		subEncl->running = false;

		snprintf(msg2sub, 255, "{\"session_id\":0,\"msgType\":%d,\"tidx\":%d}",
				GET_TASK_STATUS, subEncl->tidx);
		msgLen = strnlen(msg2sub, 255);
		uint8_t *rspMsgBuf;
		sgx_status_t ret = sendE2Emessage(&(subEncl->subEnclave.pubKey), (uint8_t *)msg2sub, &rspMsgBuf, &msgLen);
		if (SGX_SUCCESS != ret) {
			subEncl = subEncl->next;
			continue;
		}
		cJSON *rspJson = cJSON_Parse((const char *)rspMsgBuf);
		free(rspMsgBuf);
		if (NULL == rspJson){
			subEncl = subEncl->next;
			continue;
		}
		uint32_t rspMsgType;
		if (SGX_SUCCESS != getJsonNumber(rspJson, "msgType", &rspMsgType)){
			subEncl = subEncl->next;
			cJSON_Delete(rspJson);
			continue;
		}
		if (GET_TASK_STATUS_SUCCESS != rspMsgType){
			subEncl = subEncl->next;
			cJSON_Delete(rspJson);
			continue;
		}
		cJSON *taskJson = getJsonObject(rspJson, "taskDetails");
		if (NULL != taskJson){
			cJSON *statusJson = cJSON_GetArrayItem(taskJson, 0);
			if (NULL != statusJson) {
				char* status = getJsonStringPtr(statusJson, "taskStatus");
				if (NULL != status){
					if (!strncmp(status, "FINISHED", 10)){
						char *output = getJsonStringPtr(statusJson, "federatedData");
						if (NULL != output) {
							uint32_t inLen;
							size_t outLen;
							if (SGX_SUCCESS == getJsonNumber(statusJson, "dataSize", &inLen)){
								unsigned char *decOut = base64_decode(output, inLen, &outLen);
								if (NULL != decOut){
									subResultQueue.enQueue((uint8_t *)decOut, outLen, SIMFL_DATA_UNKNOWN);
								}
							}
						}
					} else {
						subEncl->running=true;
						allFinished = false;
					}
				}
			}
		}
		cJSON_Delete(rspJson);
		subEncl = subEncl->next;
	}
	return allFinished;
}

sgx_status_t hostCmd(cJSON * msgJson, SessionNode *node, uint32_t msgType, char * rspBuf, size_t maxLen, size_t *pCurLen){
	sgx_status_t ret = SGX_SUCCESS;

	size_t curLen = *pCurLen;
	switch (msgType){
	case SHARE_DATA_TO_GUEST:{
		char *srcDir = getJsonStringPtr(msgJson, "sourceDir");
		if (NULL == srcDir){
			ret = (sgx_status_t)-1;
			break;
		}
		char *dataName = getJsonStringPtr(msgJson, "dataName");
		if (NULL == dataName){
			ret = (sgx_status_t)-1;
			break;
		}
		char *dataInfo = getJsonStringPtr(msgJson, "dataInfo");

		char *guestPubKey = getJsonStringPtr(msgJson, "guestPubKey");
		if (guestPubKey == NULL){
			if (NULL != dataInfo){
				ret = saveDataInfo(srcDir, dataName, dataInfo, node->stakeholderId);
				if (SGX_SUCCESS == ret){
					snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d",SHARE_DATA_TO_GUEST_SUCCESS);
					curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
				}
				break;
			} else {
				ret = (sgx_status_t)-1;
				break;
			}
		}
		char *dstDir = getJsonStringPtr(msgJson, "destDir");
		if (NULL == dstDir){
			ret = (sgx_status_t)-1;
			break;
		}

		uint64_t startTime, endTime;
		if (SGX_SUCCESS != getJsonNumberDouble(msgJson, "startTime", &startTime)){
			ret = (sgx_status_t)-1;
			break;
		}
		if (SGX_SUCCESS != getJsonNumberDouble(msgJson, "endTime", &endTime)){
			ret = (sgx_status_t)-1;
			break;
		}

		ret = shareData2Guest(node->stakeholderId, srcDir, dstDir, dataName, guestPubKey, startTime, endTime, dataInfo, false);
		if (SGX_SUCCESS != ret)
			break;

		shareData2Guest(node->stakeholderId, srcDir, dstDir, dataName, guestPubKey, startTime, endTime, dataInfo, true);

		snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d",SHARE_DATA_TO_GUEST_SUCCESS);
		curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		break;
	}
	break;
	case STOP_SHARE_DATA_TO_GUEST:{
		snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d",STOP_SHARE_DATA_TO_GUEST_FAIL);
		curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		break;
	}
	case SEND_TO_SUB:{
		char *msg2sub = getJsonStringPtr(msgJson, "messageToSub");
		if (NULL == msg2sub){
			ret = (sgx_status_t)-1;
			break;
		}
		if (NULL == enclaveCluster)
			enclaveCluster = loadEnclaveClusterFile();
		Cluster *subEncl = enclaveCluster;
//#define SEND2SUB_BUFSIZE 16*1024
//		uint8_t *buf = (uint8_t *)malloc(SEND2SUB_BUFSIZE);
		if (maxLen-curLen < 128){
			ret = (sgx_status_t)-1;
			break;
		}

//		uint8_t *buf = rspBuf+curLen;
		size_t msgLen = strnlen(msg2sub, 1024);
		snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"subEnclaves\":[");
		curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		while (NULL != subEncl){
			char pubKey[130];
			bin2hex((uint8_t *)&(subEncl->subEnclave.pubKey), 64, pubKey);
			pubKey[128]='\0';
			snprintf((rspBuf+curLen), maxLen-curLen-1, "{\"pubKey\":\"%s\",\"resp\":", pubKey);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
			size_t trLen = msgLen;
			strncpy((char *)(rspBuf+curLen), msg2sub, msgLen);
			uint8_t *rspMsgBuf;
			ret = sendE2Emessage(&(subEncl->subEnclave.pubKey), (uint8_t *)(rspBuf+curLen), &rspMsgBuf, &trLen);
			if (SGX_SUCCESS != ret) {
				snprintf((rspBuf+curLen), maxLen-curLen-1, "{}");
				curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
				ret = SGX_SUCCESS;
			} else {
				trLen = (maxLen-curLen > trLen)? trLen:(maxLen-curLen);
				memcpy(rspBuf+curLen, rspMsgBuf, trLen);
				free(rspMsgBuf);
				curLen += trLen;
				if (curLen > maxLen-128){
					ret = (sgx_status_t)-1;
					break;
				}
			}
			snprintf((rspBuf+curLen), maxLen-curLen-1, "},");
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
			subEncl = subEncl->next;
			if (NULL == subEncl)
				curLen--;
		}
		*(rspBuf+curLen) = ']';
		curLen++;
		snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d",SEND_TO_SUB_SUCCESS);
		curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
	}
	break;
	case SEND_TO_A_SUB:{
		char *msg2sub = getJsonStringPtr(msgJson, "messageToSub");
		if (NULL == msg2sub){
			ret = (sgx_status_t)-1;
			break;
		}
		if (maxLen-curLen < 128){
			ret = (sgx_status_t)-1;
			break;
		}
		size_t msgLen = strnlen(msg2sub, 1024*1024);
		snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"subResp\":");
		curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		size_t retLen = 0;
		char *subKey = getJsonStringPtr(msgJson, "subPubKey");
		if (NULL != subKey){
			size_t trLen = msgLen;
			sgx_ec256_public_t pubKey;
			hex2bin(subKey, (uint8_t *)&pubKey, sizeof(sgx_ec256_public_t));
			strncpy((char *)(rspBuf+curLen), msg2sub, msgLen);
			uint8_t *rspMsgBuf=NULL;
			ret = sendE2Emessage(&pubKey, (uint8_t *)(rspBuf+curLen), &rspMsgBuf, &trLen);
			if (SGX_SUCCESS == ret) {
				if (maxLen-curLen > trLen){
					if (NULL != rspMsgBuf){
						memcpy(rspBuf+curLen, rspMsgBuf, trLen);
						free(rspMsgBuf);
					}
					else
						trLen = 0;
				}
				else
					trLen = 0;
				retLen = trLen;
			}
		}
		curLen += retLen;
		if (0 == retLen){
			snprintf((rspBuf+curLen), maxLen-curLen-1, "{},\"msgType\":%d",SEND_TO_A_SUB_FAIL);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		} else {
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d",SEND_TO_A_SUB_SUCCESS);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		}
	}
	break;
	case SIMFL_DATA_HUB_TO_SUB_INIT:{
		uint32_t enclaveID;
        uint32_t dataType;
        uint32_t dataLen;
        bool status = true;

		char *data = getJsonStringPtr(msgJson, "data");
        if (NULL == data) {
            status = false;
        }
        if(SGX_SUCCESS != getJsonNumber(msgJson, "dataType", &dataType)) {
            status = false;
        }
        if(SGX_SUCCESS != getJsonNumber(msgJson, "dataLen", &dataLen)) {
            status = false;
        }
        if(SGX_SUCCESS != getJsonNumber(msgJson, "EnclaveID", &enclaveID)) {
            status = false;
        }
        if (status == true) {
            status = sendDataToSub(enclaveCluster, SIMFL_DATA_HUB_TO_SUB, enclaveID, dataType, data, dataLen);
        }

		if (status){
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", SIMFL_DATA_HUB_TO_SUB_SUCCESS);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		} else {
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d",SIMFL_DATA_HUB_TO_SUB_FAIL);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		}
	}
	break;
	case SIMFL_DATA_SUB_TO_HUB_INIT:{
		uint32_t enclaveID;
        uint32_t dataType;
        bool status = true;

		if(SGX_SUCCESS != getJsonNumber(msgJson, "EnclaveID", &enclaveID)) {
            status = false;
        }
		if(SGX_SUCCESS != getJsonNumber(msgJson, "dataType", &dataType)) {
            status = false;
        }

		if (status == true) {
    		status = getDataFromSub(enclaveCluster, SIMFL_DATA_SUB_TO_HUB, enclaveID, dataType);
        }

		if (status){
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d",SIMFL_DATA_SUB_TO_HUB_SUCCESS);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		} else {
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d",SIMFL_DATA_SUB_TO_HUB_FAIL);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		}
	}
	break;
	case HOST_RUN_SUB_CODE_CYCLE:{
		char nullStr[]="";
		char *name = getJsonStringPtr(msgJson, "name");
		if(NULL==name)
			name = nullStr;
		char *dataPath = getJsonStringPtr(msgJson, "dataPath");
		if (NULL == dataPath)
			dataPath = nullStr;

		if ('\0' != name[0]) //run sub code cycle with a project name for starting checking results
			runAllSubCode(enclaveCluster, name, dataPath);

		while (true){
			sgx_status_t retv;
			ret = ocall_sleep(&retv, 2);
			bool allFinished = checkSubStatus(enclaveCluster);
			if (allFinished){
				subResultQueue.stopQueue();
				break;
			}
		}
		if (SGX_SUCCESS == ret){
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d",HOST_RUN_SUB_CODE_CYCLE_SUCCESS);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		} else {
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d",HOST_RUN_SUB_CODE_CYCLE_FAIL);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		}
	}
	break;
	case RUN_FEDERATED_MODEL:{
		char* name = getJsonStringPtr(msgJson, "name");
		if(NULL==name){
			ret= (sgx_status_t)(-1);
		}
		char* hubModel = getJsonStringPtr(msgJson, "hubcode");
		if(NULL==hubModel){
			ret= (sgx_status_t)(-1);
		}

		char** subModel;
		char** subKeys;

		int subsize = getJsonStrArr(msgJson, "subcode", &subModel);
		if(NULL==subModel){
			ret= (sgx_status_t)(-1);
		}

		getJsonStrArr(msgJson, "subkeys", &subKeys);
		if(NULL==subKeys){
			ret = (sgx_status_t)(-1);
		}

		if (NULL == enclaveCluster)
			enclaveCluster = loadEnclaveClusterFile();
		Cluster* subs = enclaveCluster;

		while(subs!=NULL){
			int index = findKeyIndex((uint8_t*)&(subs->subEnclave.pubKey), subKeys, subsize);
			if (-1 != index)
				uploadSubCode(subKeys[index], name, subModel[index]);
			subs=subs->next;
	    }

		free(subModel);
		free(subKeys);

		if (SGX_SUCCESS != ret){
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", RUN_FED_MODEL_INIT_FAIL);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
			break;
		}

		subResultQueue.startQueue();
		subSimFlLSHQueue.startQueue();

		//execute whole model on hub enclave.
        LOCK(&vmBufferLock);
        node->threadId = sgx_thread_self();
        char re_buf[1024];

		size_t len = strnlen(name,128);
		if (!strncmp(name+(len-3), ".r", 3))
			run_code(hubModel, re_buf, 1024, "0");
		else
			run_code(hubModel, re_buf, 1024, "1");

		size_t buf_size = 1024;
    	UNLOCK(&vmBufferLock);

		jsonEscape((uint8_t*)re_buf, &buf_size, buf_size);

		snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"result\":\"%s\"", re_buf);
		curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);

		snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", RUN_FED_MODEL_RSP_SUCCESS);
		curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
	}
	break;
	default:
		break;
	}
	if (SHARE_DATA_TO_GUEST == msgType && SGX_SUCCESS != ret){
		snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d",SHARE_DATA_TO_GUEST_FAIL);
		curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
	}
	*pCurLen = curLen;
	return ret;
}

sgx_status_t guestCmd(cJSON * msgJson, SessionNode *node, uint32_t msgType, char * rspBuf, size_t maxLen, size_t *pCurLen){
	sgx_status_t ret = SGX_SUCCESS;
	//Projects* projectsList = NULL;

	size_t curLen = *pCurLen;
	switch (msgType){
	case SHARE_DATA_TO_HOST:{
		snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", SHARE_DATA_TO_HOST_FAIL);
		curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		break;
	}
	case STOP_SHARE_DATA_TO_HOST:{
		snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d",STOP_SHARE_DATA_TO_HOST_FAIL);
		curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		break;
	}
	case GUEST_GET_PUB_DATA_INFO:{
		sgx_ec256_public_t pubKey;
		if (SGX_SUCCESS != dmGetPublicKey(&pubKey)){
			break;
		}
		char pubKeyStr[64*2+1];
		bin2hex((uint8_t *)&pubKey, 64, pubKeyStr);
		pubKeyStr[64*2] = '\0';
		snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"pubKey\":\"%s\",\"tree\":", pubKeyStr);
		curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		size_t len = getElementInfoJson(userStakeholderId, (char *)(rspBuf+curLen), maxLen-curLen-1, "/data/sharedin");
		curLen += len;
	}
	break;
/*
	case RECEIVE_SUB_CODE:{
		char* name = getJsonStringPtr(msgJson, "name");
		if(NULL==name){
			ret = (sgx_status_t)(-1);
		}
		char* subModel = getJsonStringPtr(msgJson, "subcode");
		if(NULL==subModel){
		    ret = (sgx_status_t)(-1);
		}
		oc_print("Receive sub code:", subModel);

		if(SGX_SUCCESS!=ret){
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", RECEIVE_SUB_CODE_FAIL);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
			break;
		}
		//store the variables in the enclave info.
		else{
			Projects* pProj = projectsList->searchInProjList(&projectsList, name, userStakeholderId);
			if (NULL != pProj){
				projectsList->deletFromProjList(&projectsList, name, userStakeholderId);
			}
			pProj = new Projects(subModel, name, userStakeholderId);
			if (NULL == pProj)
				ret = (sgx_status_t)-1;
			pProj->addToProjList(&projectsList);

			if (SGX_SUCCESS != ret){
				snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", RECEIVE_SUB_CODE_FAIL);
				curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
			} else {
				snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", RECEIVE_SUB_CODE_SUCCESS);
				curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
			}
		}
	}
	break;
	case RUN_SUB_CODE_CYCLE:{
		//run the code stored previously, prepare for the return msg.
		char* name = getJsonStringPtr(msgJson, "name");
	    if(NULL==name){
			ret = (sgx_status_t)(-1);
		}

	    Projects* res = Projects::searchInProjList(&projectsList, name, userStakeholderId);
	    if(NULL==res){
	    	ret = (sgx_status_t)(-1);
	    }

	    if (SGX_SUCCESS != ret){
	        snprintf((rspBuf+curLen), maxLen-curLen-1, "\"msgType\":%d", RUN_SUB_CODE_CYCLE_FAIL);
	    	curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
	    	break;
	    }

	    LOCK(&vmBufferLock);
	    char re_buf[1024];
	    size_t buf_size=1024;

	    node->threadId = sgx_thread_self();
	    //size_t len = strnlen(name,128);
	    //if (!strncmp(name+(len-3), ".py", 3))
	    	run_code(res->code, re_buf, 1024, "1");//no result parameter
	    //else
	    //    run_code(res->code, re_buf, 1024, "0");//no result parameter

	    UNLOCK(&vmBufferLock);

		jsonEscape((uint8_t*)re_buf, &buf_size, buf_size);

		snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"result\":\"%s\"", re_buf);
		curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);

	    snprintf((rspBuf+curLen), maxLen-curLen-1, "\"msgType\":%d", RUN_SUB_CODE_CYCLE_SUCCESS);
	    curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
	}
	break;
*/
	default:
		break;
	}
	*pCurLen = curLen;
	return ret;
}

char *fetchImage(){
	int ret;
	SGX_FILE *fp = fopen("/data/plot.png", "r");
	char *b64Buff=NULL;
	if (NULL != fp){
		do {
			ret = fseek(fp, 0, SEEK_END);
			if (0 != ret)
				break;
			int64_t imgSize = ftell(fp);
			if (imgSize < 0)
				break;
			ret = fseek(fp, 0, SEEK_SET);
			if (0 != ret)
				break;
			uint8_t *buff = (uint8_t *)malloc(imgSize);
			size_t cnt = fread(buff, imgSize, 1, fp);
			if (1 != cnt){
				free(buff);
				break;
			}
			size_t encodeSize;
			b64Buff = base64_encode(buff, imgSize, &encodeSize);
			free(buff);
		} while(false);
		fclose(fp);
	}
	remove("/data/plot.png");
	return b64Buff;
}

sgx_status_t runSandboxCommand(sgx_enclave_id_t& destEID, const char *cmdStr, sgx_key_128bit_t& connKey, char **result){
	sgx_status_t ret, retval;
	size_t cmdLen = strnlen(cmdStr, 1024)+1;
	uint8_t orig_iv[CTR_LEN];
	uint8_t *encBuff = encryptData((uint8_t *)cmdStr, cmdLen, connKey, orig_iv, false);
	if (NULL == encBuff)
		return (sgx_status_t)-1;
	uint8_t *cmdRespBuf;
	size_t respSize;
	sgx_status_t ret1 = ocall_run_command(&retval, enclaveId, destEID, userStakeholderId, encBuff, cmdLen,
			&cmdRespBuf, &respSize, orig_iv, CTR_LEN);
	free(encBuff);
	*result = NULL;
	if (SGX_SUCCESS == ret1 && SGX_SUCCESS == retval){
		uint8_t *resBuf = (uint8_t *)malloc(respSize+1);
		if (NULL != resBuf){
			ret1 = ocall_get_data(&retval, &cmdRespBuf, resBuf, respSize);
			if (SGX_SUCCESS == ret1 && SGX_SUCCESS == retval){
				uint8_t dec_iv[CTR_LEN];
				memcpy(dec_iv, orig_iv, CTR_LEN);
				uint8_t *decMsg = decryptData(resBuf, respSize+1, connKey, dec_iv);
				if (NULL != decMsg){
					decMsg[respSize] = '\0';
					*result = (char *)decMsg;
				}
			}
			free(resBuf);
		} else //free external buffer
			ocall_get_data(&retval, &cmdRespBuf, NULL, 0);
	}
	return ret;
}

sgx_status_t createSandbox(const char *sandboxPath, const char *modelPath, sgx_key_128bit_t *connKey,
		sgx_enclave_id_t *destEID){
	sgx_status_t ret = SGX_SUCCESS;
	StakeholderRec_t stRec;
	stRec.seqNumber = userStakeholderId;
	if (SGX_SUCCESS != dmReadStake(&stRec))
		ret = (sgx_status_t)-1;
	if(stRec.isDeleted) {
		return (sgx_status_t)(-1);
	}
	sgx_key_128bit_t startKey;
	char startKeyHex[33];
	if (SGX_SUCCESS != sgx_read_rand((unsigned char *)&startKey, sizeof(sgx_key_128bit_t)))
		ret = (sgx_status_t)-1;
	bin2hex(startKey, 16, startKeyHex);
	startKeyHex[32]='\0';


	char reqBuf[1024];
	uint8_t respBuf[1024];
	size_t bufSize = 1024;
	size_t respSize;
	snprintf(reqBuf, bufSize, "{\"stakeholderId\":%d, \"sandboxPath\":\"%s\", \"modelPath\":\"%s\", \"startKey\":\"%s\", \"initSession\":\"n\"}",
			userStakeholderId, sandboxPath, modelPath, startKeyHex);

	uint8_t orig_iv[CTR_LEN];
	size_t reqLen = strnlen(reqBuf, bufSize-1)+1;
	uint8_t *encBuff = encryptData((uint8_t *)reqBuf, reqLen, stRec.encKey, orig_iv, true);
	if (NULL == encBuff)
		return (sgx_status_t)-1;

	sgx_status_t retval, ret1;
	ret1 = ocall_create_enclave(&retval, enclaveId, userStakeholderId, encBuff, reqLen,
			respBuf, bufSize, &respSize, orig_iv, CTR_LEN);
	free(encBuff);

	if (SGX_SUCCESS == ret1 && SGX_SUCCESS == retval)
		ret = SGX_SUCCESS;
	else
		ret = (sgx_status_t)-1;

	if (SGX_SUCCESS == ret){
		ret = (sgx_status_t)-1;
		uint8_t dec_iv[CTR_LEN];
		memcpy(dec_iv, orig_iv, CTR_LEN);
		uint8_t *decMsg = decryptData(respBuf, respSize+1, startKey, dec_iv);
		if (NULL == decMsg)
			return ret;
		decMsg[respSize] = '\0';
		cJSON *rspJson = cJSON_Parse((const char *)decMsg);
		free(decMsg);
		if (NULL != rspJson){
			char *connKeyStr = getJsonStringPtr(rspJson, "connKey");
			if (NULL != connKeyStr){
				hex2bin(connKeyStr, (uint8_t *)connKey, sizeof(sgx_key_128bit_t));
				ret = getJsonNumberDouble(rspJson, "destEID", destEID);
			}
			cJSON_Delete(rspJson);
		}
	}
	return ret;
}

sgx_status_t runTrainingSandbox(const char *sandboxPath, const char *modelPath, const char *cmdStr, char **output){
	sgx_status_t ret = (sgx_status_t)-1;
	sgx_key_128bit_t connKey;
	sgx_enclave_id_t destEID;
	if (SGX_SUCCESS == createSandbox(sandboxPath, modelPath, &connKey, &destEID)){
		sgx_status_t retval;
		ret = runSandboxCommand(destEID, cmdStr, connKey, output);
		ocall_destroy_enclave(&retval, enclaveId, destEID, userStakeholderId);
	}
	return ret;
}

sgx_status_t commonCmd(cJSON * msgJson, SessionNode *node, uint32_t msgType, char * rspBuf, size_t maxLen, size_t *pCurLen){
	sgx_status_t ret = SGX_SUCCESS;

	size_t curLen = *pCurLen;
	switch (msgType){
	case CREATE_ENCLAVE:{ //"{\"msgType\":%d,\"sandboxPath\":\"/\",\"modelPath\":\"/model/\"}",CREATE_ENCLAVE)
		if (!isGlobalSandbox())
			ret = (sgx_status_t)-1;
		char *sandboxPath = getJsonStringPtr(msgJson, "sandboxPath");
		if (NULL == sandboxPath)
			ret = (sgx_status_t)-1;
		char *modelPath = getJsonStringPtr(msgJson, "modelPath");
		if (NULL == modelPath)
			ret = (sgx_status_t)-1;

		char *cmdStr=NULL;
		cJSON *command = getJsonObject(msgJson, "command");
		if (NULL == command)
			ret = (sgx_status_t)-1;
		else {
			cmdStr = cJSON_PrintUnformatted(command);
			if (NULL == cmdStr){
				ret = (sgx_status_t)-1;
			}
		}

		sgx_key_128bit_t connKey;
		sgx_enclave_id_t destEID;
		if (SGX_SUCCESS == ret){
			if (SGX_SUCCESS == createSandbox(sandboxPath, modelPath, &connKey, &destEID)){
				char *result;
				sgx_status_t retval;
				ret = runSandboxCommand(destEID, cmdStr, connKey, &result);
				ocall_destroy_enclave(&retval, enclaveId, destEID, userStakeholderId);
				cJSON *resultJson = cJSON_Parse(result);
				free(result);
				if (NULL != resultJson){
					char *outputStr = getJsonStringPtr(resultJson, "result");
					char *federatedDataStr = getJsonStringPtr(resultJson, "federatedData");
					size_t fedDataLen;
					char oc_buf[128];
					if (SGX_SUCCESS != getJsonNumberDouble(resultJson, "dataSize", &fedDataLen))
						fedDataLen =0;
					uint64_t tid = sgx_thread_self();
					TaskTableNode *tmNode = taskTable.getNodeByETID(tid);
					if (NULL != tmNode){
						if (NULL != federatedDataStr){
							tmNode->result = (uint8_t *)malloc(fedDataLen);
							if (NULL != tmNode->result){
								memcpy(tmNode->result, federatedDataStr, fedDataLen);
								tmNode->resultLen = fedDataLen;
								ret = SGX_SUCCESS;
							}
						}
						if (NULL != outputStr){
							size_t outSize = strnlen(outputStr, 1024*1024);
							tmNode->taskOutput = (char *)malloc(outSize);
							if (NULL != tmNode->taskOutput)
								memcpy(tmNode->taskOutput, outputStr, outSize);
						}
					}
					cJSON_Delete(resultJson);
				}
			}
		}

		if (SGX_SUCCESS != ret){
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", CREATE_ENCLAVE_RSP_FAIL);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
			break;
		} else {
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", CREATE_ENCLAVE_RSP_SUCCESS);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		}
		break;
	}
	case DELETE_MODEL:{
		char *name = getJsonStringPtr(msgJson, "name");
		if (NULL == name){
			//return (sgx_status_t)-1;
			break;
		}

		char dirName[MAX_PATH];
		snprintf(dirName, MAX_PATH-1, "model%d.dir",node->stakeholderId);
		ret = dmDelDirEntryByName(dirName, name);

		if (SGX_SUCCESS != ret){
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", DELETE_MODEL_RSP_FAIL);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		} else {
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", DELETE_MODEL_RSP_SUCCESS);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		}
		break;
	}

	case DNLOAD_MODEL:{
		char *name = getJsonStringPtr(msgJson, "name");
		if (NULL == name){
			//return (sgx_status_t)-1;
			break;
		}

		size_t buf_size = 10000;
		uint8_t *buf = (uint8_t *)malloc(buf_size);
		if (NULL == buf)
			break;
		char dirName[MAX_PATH];
		snprintf(dirName,MAX_PATH-1,"model%d.dir",node->stakeholderId);
		ret = dmReadDataOrModel((const char *)dirName, (const char *)name, buf, &buf_size);
	    if (SGX_SUCCESS == ret){
		    jsonEscape(buf, &buf_size, 10000);
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"content\":\"%s\"", buf);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", DNLOAD_MODEL_RSP_SUCCESS);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
	    } else {
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", DNLOAD_MODEL_RSP_FAIL);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
	    }
	    free(buf);
		break;
	}

	case UPLOAD_MODEL:{
		ret = SGX_SUCCESS;
		char *title = getJsonStringPtr(msgJson, "title");
		if (NULL == title)
			ret = (sgx_status_t)-1;
		char *data = getJsonStringPtr(msgJson, "code");
		if (NULL == data)
			ret = (sgx_status_t)-1;

		if (SGX_SUCCESS == ret){
			ret = dmWriteWithId(node->stakeholderId, "model", title, (uint8_t*)data, strnlen(data,MAX_CODE_LEN)+1);
			cJSON *subEnclaves = getJsonObject(msgJson, "subEnclaves");
			if (NULL != subEnclaves){
				int size = cJSON_GetArraySize(subEnclaves);
				for (int i=0; i<size; i++){
					cJSON *subObj = cJSON_GetArrayItem(subEnclaves, i);
					if (NULL != subObj){
						char *key = getJsonStringPtr(subObj, "key");
						char *code = getJsonStringPtr(subObj, "code");
						if (NULL != key && NULL != code)
							ret = uploadSubCode(key, title, code);
					}
				}
			}
		}

		if (SGX_SUCCESS != ret){
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", UPLOAD_MODEL_RSP_FAIL);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		} else {
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", UPLOAD_MODEL_RSP_SUCCESS);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		}
		break;
	}

	case RUN_MODEL:{
		if (!(apiPermissions && RUN_INTERACTIVE_SCRIPT)){
			ret = (sgx_status_t)-1;
		}
		char *name = getJsonStringPtr(msgJson, "name");
		if (NULL == name){
			ret = (sgx_status_t)-1;
		}
		char *data = getJsonStringPtr(msgJson, "data");
		if (NULL == data){
			ret = (sgx_status_t)-1;
		}
		if (SGX_SUCCESS != ret){
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", RUN_MODEL_RSP_FAIL);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
			break;
		}
        //ret = SGX_SUCCESS;//dmWriteWithId(node->stakeholderId, "model", name, (uint8_t*)data, strnlen(data,MAX_CODE_LEN)+1);

        LOCK(&vmBufferLock);
        if (userStakeholderId != 1)
        	dmSetSandbox("anonymized");
        else
        	dmSetSandbox("");
		char re_buf[2048];
		size_t buf_size = 2048;
        node->threadId = sgx_thread_self();
		size_t len = strnlen(name,128);
		if (!strncmp(name+(len-3), ".r", 3))
			run_code(data, re_buf, buf_size, "0");//no result parameter
		else
			run_code(data, re_buf, buf_size, "1");//no result parameter
    	UNLOCK(&vmBufferLock);

		jsonEscape((uint8_t*)re_buf, &buf_size, buf_size);

		snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"result\":\"%s\"", re_buf);
		curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);

		char *imgData = fetchImage();

		if (NULL != imgData){
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"image\":\"%s\"", imgData);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
			free(imgData);
		}

		if (SGX_SUCCESS != ret){
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", RUN_MODEL_RSP_FAIL);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		} else {
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", RUN_MODEL_RSP_SUCCESS);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		}
		break;
	}

	case RUN_MODEL_FILE :{
		ret = (sgx_status_t)-1;
		do {
			char *name = getJsonStringPtr(msgJson, "name");
			if (NULL == name)
				break;
			/* [EN-98]
			char fullName[MAX_PATH];
			if (strncmp(name, "/model/", 7))
				snprintf(fullName, MAX_PATH-1, "/model/%s", name);
			else
				snprintf(fullName, MAX_PATH-1, "%s", name);
			*/
			bool isOPAL= !strncmp(name, "/model/OPAL/", 12);
			/*
			SGX_FILE *fp = fopen(fullName, "r");
			if (NULL == fp)
				break;
			fseek(fp, 0, SEEK_END);
			size_t size = ftell(fp);
			fseek(fp, 0, SEEK_SET);
			*/
			/* Assuming that the code is not larger than 1MB */
			size_t size = 1024*1024;
			uint8_t *data = (uint8_t *)malloc(size+1);
			if (NULL == data){
				//fclose(fp);
				break;
			}
			/*
			int rlen = fread(data, size, 1, fp);
			fclose(fp);
			data[size]='\0';
			if (1 != rlen)
				break;
			*/
			ret = dmReadWithId(node->stakeholderId, "model", name, data, &size);
			if(ret != SGX_SUCCESS) {
				return ret;
			}

			char *dataPath = getJsonStringPtr(msgJson, "dataPath");
			char *privateAlgo = getJsonStringPtr(msgJson, "privateAlgo");
			char re_buf[1024];
			LOCK(&vmBufferLock);
	        if (userStakeholderId == 1 || isOPAL)
	        	dmSetSandbox("");
		    else if (NULL == dataPath || 		//training is a folder containing raw data
		    		!strncmp(dataPath, "training", 8)) //Prohibited in the dataPath, it can only be in the base path
	        	dmSetSandbox("anonymized");
	        else
	        	dmSetSandbox(dataPath);
	        node->threadId = sgx_thread_self();

			size_t len = strnlen(name,128);
			if (!strncmp(name+(len-3), ".r", 3))
				run_code((const char *)data, re_buf, 1024, "0");//no result parameter
			else
				run_code((const char *)data, re_buf, 1024, "1");//no result parameter
			size_t outLen;
			if (NULL != runCmdResult) {
				char *encRes = base64_encode(runCmdResult, runCmdResultLen, &outLen);
				if (NULL != encRes){
					snprintf((rspBuf+curLen), maxLen-curLen-1, ", \"federatedData\":\"%s\",\"dataSize\":%lu", encRes, outLen);
					curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
					*pCurLen = curLen;
					free(encRes);
				}
				free(runCmdResult);
				runCmdResult = NULL;
				runCmdResultLen=0;
			}
			UNLOCK(&vmBufferLock);
			free(data);

			size_t buf_size = 10000;
			jsonEscape((uint8_t*)re_buf, &buf_size, 10000);

			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"result\":\"%s\"", re_buf);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
			ret = SGX_SUCCESS;
		} while(false);

		if (SGX_SUCCESS != ret){
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", RUN_MODEL_FILE_RSP_FAIL);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		} else {
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", RUN_MODEL_FILE_RSP_SUCCESS);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		}
		break;
	}
	case STREAM_PUSH:{
		uint32_t streamId;
		sgx_status_t ret1 = getJsonNumber(msgJson, "streamId", &streamId);

		uint32_t pageNo = 0;
		sgx_status_t ret2 = getJsonNumber(msgJson, "pageNo", &pageNo);

		char *dataPtr = getJsonStringPtr(msgJson, "data");

		ret = (sgx_status_t)-1;
		if (SGX_SUCCESS == ret1 && SGX_SUCCESS == ret2 && NULL != dataPtr){
			PaginationStream *pStream = pageStreams.getStreamById(streamId);
			if (NULL != pStream){
				size_t rawLen;
				uint8_t *rcvBuf = pStream->getPageBuff(pageNo);
				if (NULL != rcvBuf){
					uint32_t pageSize = pStream->getPageSize();
					uint32_t dataLen = (uint32_t)base64_decode2buff(dataPtr, strnlen(dataPtr, pageSize<<1),
							(char *)rcvBuf, pageSize);
					if (dataLen > 0) {
						ret = pStream->receivePage(pageNo, dataLen, node->lastActiveSecond);
					}
				}
				ret = SGX_SUCCESS;
				snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"missingPage\":%d", pStream->getFirstPage());
				curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
			}
		}

		if (SGX_SUCCESS != ret){
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", STREAM_PUSH_RSP_FAIL);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		} else {
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", STREAM_PUSH_RSP_SUCCESS);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		}

	}
	break;
	case UPLOAD_DATA:{
		char *dataInfo = getJsonStringPtr(msgJson, "dataInfo");
		//if (NULL == dataInfo){
		//	return (sgx_status_t)-1;
		//}

		uint64_t fileSize=0;
		if(SGX_SUCCESS != getJsonNumberDouble(msgJson, "fileSize", &fileSize)){
			fileSize = 0;
		}

		uint32_t offset = 0;
		if(SGX_SUCCESS != getJsonNumber(msgJson, "offset", &offset)){
			offset = 0;
		}

		char *dataPtr = getJsonStringPtr(msgJson, "data");
		char *dataName = getJsonStringPtr(msgJson, "dataName");
		if (NULL != dataName){
			if (NULL == dataPtr){
				cJSON *startStreamJson = getJsonObject(msgJson, "startStream");
				if (NULL == startStreamJson)
					ret = (sgx_status_t)-1;
				else {
					PaginationStream *pStream = newDataStream(node->stakeholderId, startStreamJson, "data", dataName, dataInfo);
					if (NULL != pStream){
						size_t len = pStream->genStartStreamRspJSON(rspBuf+curLen, maxLen-curLen);
						if (len > 0){
							curLen += len;
						} else
							ret = (sgx_status_t)-1;
					}
				}
			} else {
				size_t rawLen;
				uint8_t *rawData = (uint8_t *)base64_decode(dataPtr, strnlen(dataPtr, 1<<22), &rawLen);
				if (NULL == rawData)
					ret = (sgx_status_t)-1;
				else if (SGX_SUCCESS == ret){
					char dir[MAX_PATH];
					snprintf(dir, MAX_PATH-1, "data%d.dir", node->stakeholderId);
					ret = dmWriteDataOrModel(dir, dataName, rawData, rawLen, fileSize, offset);
					if (SGX_SUCCESS == ret)
						ret = dmWriteFileInfo(dir, dataName, dataInfo);
					free(rawData);
				}
			}
		} else
			ret = (sgx_status_t)-1;

		if (SGX_SUCCESS != ret){
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", UPLOAD_DATA_RSP_FAIL);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		} else {
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", UPLOAD_DATA_RSP_SUCCESS);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"receipt\":\"0x239492347934875349\"");
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		}
	  }
		break;
	case DELETE_DATA:{
		char *addrPtr = getJsonStringPtr(msgJson, "ethAddress");
		if (NULL == addrPtr){
			return (sgx_status_t)-1;
		}
		uint8_t ethAddr[20];
		hex2bin(addrPtr, ethAddr, 20);

		char *dataName = getJsonStringPtr(msgJson, "dataName");
		if (NULL == dataName){
			return (sgx_status_t)-1;
		}
                if (!strncmp(dataName,"/data", 5)){
                        char dir[MAX_PATH];
                        snprintf(dir, MAX_PATH-1, "data%d.dir", node->stakeholderId);
                        ret = dmDelDirEntryByName(dir, dataName+5);
                } else if (!strncmp(dataName, "/lib", 4)){
                        if (ROLE_RUNNER != node->role){ //only enclave runner are allowed to delete library
                                snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", DELETE_DATA_RSP_FAIL);
                                curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
                                break;
                        }
                        char dir[MAX_PATH];
                        snprintf(dir, MAX_PATH-1, "lib.dir");
                        ret = dmDelDirEntryByName(dir, dataName+4);
                } else
                        ret = (sgx_status_t)-1;

		if (SGX_SUCCESS != ret){
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", DELETE_DATA_RSP_FAIL);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		} else {
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", DELETE_DATA_RSP_SUCCESS);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		}
	 }
	break;
	case GET_INFO:{
		char *path = getJsonStringPtr(msgJson, "path");

		long retLen;
		retLen = getEnclaveInfoJson(node->stakeholderId, rspBuf+curLen, maxLen-curLen-200, (const char *)path);

		if (0 != retLen){
			curLen += retLen;
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", GET_INFO_RSP_SUCCESS);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		} else {
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", GET_INFO_RSP_FAIL);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		}
	  }
		break;
	case DIR_OP:{
		long retLen;
		char *opCode = getJsonStringPtr(msgJson, "opCode");
		char *path = getJsonStringPtr(msgJson, "name");
		if (NULL == path || NULL == opCode){
			retLen = 0;
		} else {
			//if (strncmp(path, "/data/training", 14)) //restrict accessing the training data folder
			if (!strncmp(opCode, "ls", 3)){
				retLen = getFileList(node->stakeholderId, rspBuf+curLen, maxLen-curLen-200, (const char *)path);
			} else if (!strncmp(opCode, "rm", 3)){
				if (userStakeholderId !=1 && (!strncmp(path, "/lib", 4))){
					snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"result\":\"Error: can't delete a library file.\"");
					curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
				} else {
					int ret = dmRemove(path);
					if (ret == 0){
						snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"result\":\"File %s is deleted.\"", path);
						curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
					} else {
						snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"result\":\"Error while deleting %s.\"", path);
						curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
					}
				}
			} else if (!strncmp(opCode, "cd", 3)){
				retLen = getFileList(node->stakeholderId, rspBuf+curLen, maxLen-curLen-200, (const char *)path);
			} else
				retLen = 0;
		}

		if (0 != retLen){
			curLen += retLen;
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", DIR_OP_RSP_SUCCESS);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		} else {
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", DIR_OP_RSP_FAIL);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		}
	}
	break;
	case SAVE_FILE:{
		char *path = getJsonStringPtr(msgJson, "name");
		char *code = getJsonStringPtr(msgJson, "code");
		uint64_t fileSize=0;
		if(SGX_SUCCESS != getJsonNumberDouble(msgJson, "dataSize", &fileSize)){
			fileSize = 0;
		}
		ret = (sgx_status_t)-1;
		if (NULL != path && NULL != code && 0 != fileSize){
			size_t outSize;
			uint8_t *data = base64_decode(code, strnlen(code, 1024*1024*1024), &outSize);
			if (NULL != data && fileSize == outSize){
				char dir[MAX_PATH];
				if (!strncmp(path, "/data", 5)){
					snprintf(dir, MAX_PATH-1, "data%d.dir", node->stakeholderId);
					ret = dmWriteDataOrModel(dir, path+5, data, fileSize, 0, 0);
				} else if (!strncmp(path, "/model", 6)){
					snprintf(dir, MAX_PATH-1, "model%d.dir", node->stakeholderId);
					ret = dmWriteDataOrModel(dir, path+6, data, fileSize, 0, 0);
				}
			}
			if (NULL != data)
				free(data);
		}
		if (SGX_SUCCESS == ret){
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"result\":\"File %s saved.\",\"msgType\":%d",
					path, SAVE_FILE_RSP_SUCCESS);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		} else {
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"result\":\"Error, save file fail.\",\"msgType\":%d",
					SAVE_FILE_RSP_FAIL);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		}


	}
	break;
	default:
		break;
	}
	*pCurLen = curLen;
	return ret;
}

sgx_status_t handleSessionCommand(SessionNode *node, const uint8_t *message, size_t len, void *resVecPtr,
		uint8_t *iv, uint64_t currentSecond){
	uint8_t dec_iv[16];
	memcpy(dec_iv, iv, 16);
	uint8_t *decMsg = decryptData((uint8_t *)message, len+1, node->secretKey, dec_iv);
	if (NULL == decMsg)
		return (sgx_status_t)-2; //decryption fail
	*(decMsg+len) = '\0';
	oc_print("Received message:", (const char *)decMsg);
	cJSON *msgJson = cJSON_Parse((const char *)decMsg);
	free(decMsg);
	if (NULL == msgJson){
		return (sgx_status_t)-2; //wrong session, can't decrypt message, bad JSON
	}

	size_t maxLen = 204800;
	char *rspBuf = (char *)malloc(maxLen);
	if (NULL == rspBuf){
		cJSON_Delete(msgJson);
		return (sgx_status_t)-1;
	}
	snprintf(rspBuf,maxLen-1,"{\"session_id\":%d",node->sessionId);
	size_t curLen = strnlen(rspBuf, maxLen);

	uint32_t msgType;
	if (SGX_SUCCESS == getJsonNumber(msgJson, "msgType", &msgType)){
		if (SESSION_CONFIRM == msgType){ //Confirming the session
			node->confirmed = true;
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", SESSION_CONFIRM_RSP);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		} else if (!node->confirmed){ //Unconfirmed session
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", SESSION_UNCONFIRMED);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		} else {
			//uint32_t role=4;
			task_manager_main(node, msgJson, msgType, rspBuf, maxLen, &curLen, currentSecond);
		}
	} else {
		snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"error\":\"No message type\"");
		curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
	}


	snprintf((rspBuf+curLen), maxLen-curLen-1, "}");
	curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);

	//*pResLen = curLen;
	oc_print("Response message:", (const char *)rspBuf);
	uint8_t *encBuff = encryptData((uint8_t *)rspBuf, curLen, node->secretKey, iv, false);
	free(rspBuf);
	if (NULL == encBuff){
		cJSON_Delete(msgJson);
		return (sgx_status_t)-1;
	}

	//memcpy(pResult, encBuff, curLen);
	sgx_status_t oc_ret;
	ocall_send_result(&oc_ret, encBuff, curLen, resVecPtr);

	free(encBuff);
	cJSON_Delete(msgJson);
	return SGX_SUCCESS;
}

sgx_status_t ec_session(uint32_t sessionId, const uint8_t *message, size_t len, void *resVecPtr,
		uint8_t *iv, size_t ivLen, uint64_t currentSecond)
{
	if (NULL == sessionTable)
		return (sgx_status_t)-1;

	SessionNode *node = sessionTable->getNode(sessionId, currentSecond);
	if (NULL == node)
		return (sgx_status_t)-2; //session doesn't exist

	return(handleSessionCommand(node, message, len, resVecPtr, iv, currentSecond));
}

sgx_status_t ec_run_command(uint8_t *encBuff, size_t bufLen, void *resVecPtr, uint8_t *iv, size_t ivLen){
	SessionNode *node = new SessionNode(0);
	if (NULL == node)
		return (sgx_status_t)-1;
	memcpy(node->secretKey, connectionKey, sizeof(sgx_key_128bit_t));
	node->confirmed = true;
	node->sessionId = 1;
	node->role = ROLE_GUEST;
	sgx_status_t ret = handleSessionCommand(node, encBuff, bufLen, resVecPtr, iv, 0);
	delete node;
	return ret;
}

sgx_status_t handleCommand(cJSON *msgJson, SessionNode *node, uint32_t msgType, char * rspBuf, size_t maxLen, size_t *pCurLen){
	size_t oldLen = *pCurLen;
	sgx_status_t ret;
	if (ROLE_RUNNER == node->role)
		ret = runnerCmd(msgJson, node, msgType, rspBuf, maxLen, pCurLen);
	if (ROLE_HOST == node->role && oldLen == *pCurLen)
		ret = hostCmd(msgJson, node, msgType, rspBuf, maxLen, pCurLen);
	if (ROLE_GUEST == node->role && oldLen == *pCurLen)
		ret = guestCmd(msgJson, node, msgType, rspBuf, maxLen, pCurLen);
	if (oldLen == *pCurLen)
		ret = commonCmd(msgJson, node, msgType, rspBuf, maxLen, pCurLen);
	if (oldLen == *pCurLen){
		snprintf((rspBuf+oldLen), maxLen-oldLen-1, ",\"msgType\":%d", UNKNOWN_MSG);
		oldLen += strnlen((char *)(rspBuf+oldLen), maxLen-oldLen);
		*pCurLen = oldLen;
		ret = (sgx_status_t)-1;
	}
	return ret;
}

/*
 * Set the submitResult data. If there is a task then set to the task, otherwise set
 *  to the session command result which will be returned when the command has been completed.
 */
sgx_status_t setSubEnclaveSendQueue(uint8_t *buf, size_t bufLen, simfl_data_t data_type){
	if (taskTable.getLastNode()>=1){
		uint64_t tid = sgx_thread_self();
		TaskTableNode *tmNode = taskTable.getNodeByETID(tid);
		if (NULL != tmNode){
			size_t outLen;
			char *encRes = base64_encode(buf, bufLen, &outLen);
			if (NULL == encRes) {
				return (sgx_status_t)-1;
			}
			if (data_type == SIMFL_DATA_UNKNOWN) {
				tmNode->result = (uint8_t *)encRes;
				tmNode->resultLen = outLen;
				return SGX_SUCCESS;
			} else {
				return tmNode->msgSendQueue.enQueue((uint8_t *)encRes, outLen, (uint32_t)data_type);
			}
		}
	}
	if (NULL != runCmdResult)
		free(runCmdResult);
	runCmdResult = (uint8_t *)malloc(bufLen);
	if (NULL == runCmdResult)
		return (sgx_status_t)-1;
	memcpy(runCmdResult, buf, bufLen);
	runCmdResultLen = bufLen;
	return SGX_SUCCESS;
}

uint8_t *getSubEnclaveRcvQueue(simfl_data_t data_type, size_t *length){
	if (taskTable.getLastNode()>=1) {
		uint64_t tid = sgx_thread_self();
		TaskTableNode *tmNode = taskTable.getNodeByETID(tid);
		if (NULL != tmNode) {
			size_t len, data_type;
			char *rcv_s;
			while((rcv_s = (char*)tmNode->msgRecvQueue.deQueue(&len, (uint32_t *)&data_type)) == NULL) {
				sgx_status_t retv;
				ocall_sleep(&retv, 2);
			}
			unsigned char *decOut = base64_decode(rcv_s, len, length);
			return decOut;
		}
	}
	return NULL;
}

int32_t numSubEnclaves(){
	int count = 0;
	Cluster *subEncl;
	if (NULL == enclaveCluster) {
		enclaveCluster = loadEnclaveClusterFile();
	}
	if (enclaveCluster) {
		subEncl = enclaveCluster;
	} else {
		return -1;
	}

	while(subEncl != NULL) {
		count++;
		subEncl = subEncl->next;
	}

	return count;
}
