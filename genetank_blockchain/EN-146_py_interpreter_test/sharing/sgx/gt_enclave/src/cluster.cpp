/*
 * cluster.cpp
 *
 *  Created on: Sep 6, 2019
 *      Author: accuser
 */
#include "cluster.h"
#include "sgx_tprotected_fs.h"
#include "session.h"
#include <stdio.h>
#include "string.h"
#include "data_management.h"
#include "crypto.h"
#include "util.h"
#include "conversion.h"

Cluster *enclaveCluster=NULL;


sgx_status_t Cluster::addSubEnclave(Cluster **head){
	this->next = *head;
	*head = this;
	return SGX_SUCCESS;
}

sgx_status_t deleteSubEnclave(Cluster **head, Cluster *encl){
	Cluster *ptr;
	if (NULL == *head || NULL == encl)
		return(sgx_status_t)-1;
	if (*head == encl){
		*head = encl->next;
	} else {
		ptr = *head;
		while (ptr->next != encl){
			ptr = ptr->next;
			if (NULL == ptr)
				return(sgx_status_t)-1;
		}
		ptr->next = encl->next;
	}
	delete(encl);
	return SGX_SUCCESS;
}

Cluster *Cluster::searchSubEnclave(sgx_ec256_public_t *pubkey){
	Cluster *ptr;
	ptr = this;
	while (NULL != ptr){
		if (!memcmp(&(ptr->subEnclave.pubKey), pubkey, sizeof(sgx_ec256_public_t)))
			break;
		ptr = ptr->next;
	}
	return ptr;
}

sgx_status_t saveEnclaveClusterFile(Cluster *head){
	sgx_status_t ret = SGX_SUCCESS;

	char clusterFileName[MAX_PATH]={'\0'};

	getFullPathFileName(clusterFileName,MAX_PATH,CLUSTER_FILENAME);

	SGX_FILE* file = sgx_fopen_auto_key(clusterFileName, "w");

	if (NULL == file)
		return (sgx_status_t)-1;

	while (NULL != head){
		size_t cnt = sgx_fwrite(&(head->subEnclave), sizeof(SubEnclave_t), 1, file);
		if (1 != cnt){
			ret = (sgx_status_t)-1;
			break;
		}
		head = head->next;
	}
	sgx_fclose(file);
	return ret;
}

Cluster * loadEnclaveClusterFile(){
	sgx_status_t ret = SGX_SUCCESS;
	Cluster *head=NULL;

	char clusterFileName[MAX_PATH]={'\0'};

	//snprintf(clusterFileName,MAX_PATH,"%s/%s",enclaveHome,CLUSTER_FILENAME);
	getFullPathFileName(clusterFileName,MAX_PATH,CLUSTER_FILENAME);

	SGX_FILE* file = sgx_fopen_auto_key(clusterFileName, "r");

	if (NULL == file)
		return NULL;

	while (!sgx_feof(file)){
		Cluster *ptr = new Cluster;
		if (NULL == ptr)
			break;
		size_t cnt = sgx_fread(&(ptr->subEnclave), sizeof(SubEnclave_t), 1, file);
		if (1 != cnt){
			//ret = (sgx_status_t)-1;
			break;
		}
		ptr->addSubEnclave(&head);
	}
	sgx_fclose(file);
	return head;
}

sgx_status_t initE2Esession(char *destUrl, uint32_t *pSessionId, sgx_ec256_public_t *destPubkey, uint8_t *secretKey){
	sgx_status_t ret = SGX_SUCCESS;
	*pSessionId = 0;
	size_t bufLen = 1024;
	size_t curBuffLen = bufLen;
	char sndBuff[1024];
	sgx_ec256_public_t pubKey;
	char sPubKey[2*sizeof(sgx_ec256_public_t)+1];
	ret = dmGetPublicKey(&pubKey);
	if (SGX_SUCCESS != ret)
		return ret;
	bin2hex((uint8_t*)&pubKey, sizeof(sgx_ec256_public_t), (char*)sPubKey);
	sPubKey[2*sizeof(sgx_ec256_public_t)]='\0';

	sgx_ec256_private_t genPrivate;
	size_t keyLen=65;
	uint8_t genPubKey[65];
	char sGenPubKey[131];

	if (SGX_SUCCESS != genK1KeyPair(&genPrivate, genPubKey, &keyLen)){
		ret = (sgx_status_t)-1;
		return ret;
	}

	bin2hex((uint8_t*)&genPubKey, 65, (char*)sGenPubKey);
	sGenPubKey[130]='\0';


	//sgx_ec256_private_t privkey;
	//if (SGX_SUCCESS != dmGetPrivateKey(&privkey)){
	//	ret = (sgx_status_t)-1;
	//	break;
	//}

	char sign[] = "no signature yet";

	snprintf(sndBuff,bufLen-1-sizeof(uint32_t)-sizeof(size_t),
		"{\"session_id\":0,\"pubKey\":\"0x%s\",\"ethPubKey\":\"0x04%s\",\"sign\":\"%s\",\"role\":%d,\"msgType\":%d}",
		sGenPubKey, sPubKey, sign, ROLE_GUEST, SESSION_INIT);

	curBuffLen = strnlen(sndBuff, bufLen-1);

	uint8_t iv[16];

	sgx_status_t oc_ret;
	uint32_t sessionId=0;
	uint8_t *rspPtr=NULL;
	size_t rspLen;
	ret = ocall_http2enclave(&oc_ret, getEnclaveId(), &sessionId, (uint8_t *)sndBuff, curBuffLen, &rspPtr, &rspLen, iv, 16, destUrl);

	if (SGX_SUCCESS != ret || SGX_SUCCESS != oc_ret || NULL == rspPtr || 0 == rspLen){
		if (NULL != rspPtr)
			ocall_get_data(&oc_ret, &rspPtr, NULL, 0); //free(rspPtr);
		return (sgx_status_t)-1;
	}
	uint8_t *rspBuf = (uint8_t *)malloc(rspLen);
	if (NULL == rspBuf)
		return (sgx_status_t)-1;
	ret = ocall_get_data(&oc_ret, &rspPtr, rspBuf, rspLen);
	if (SGX_SUCCESS != ret || SGX_SUCCESS != oc_ret)
		return (sgx_status_t)-1;

	uint8_t serializedDestPubkey[sizeof(sgx_ec256_public_t)+1];
	*serializedDestPubkey = '\4';
	memmove(&(serializedDestPubkey[1]), destPubkey, sizeof(sgx_ec256_public_t));

	ret = gen_ecdh_key(serializedDestPubkey, genPrivate, secretKey);
	if (SGX_SUCCESS != ret){
		free(rspBuf);
		return ret;
	}
	uint8_t dec_iv[16];
	memcpy(dec_iv, iv, 16);
	uint8_t *decMsg = decryptData(rspBuf, rspLen+1, secretKey, dec_iv);
	free(rspBuf);
	if (NULL == decMsg){
		ret = (sgx_status_t)-1;
		return ret;
	}

	*(decMsg+rspLen) = '\0';
	cJSON *respJson = cJSON_Parse((const char *)decMsg);//ssDecryptMessage(*pSessionId, message, len);
	free(decMsg);

	if (NULL == respJson){
		ret = (sgx_status_t)-1;
		return ret;
	}

	uint32_t msgT;
	if (SGX_SUCCESS != getJsonNumber(respJson, "msgType", &msgT)){
		cJSON_Delete(respJson);
		ret = (sgx_status_t)-1;
		return ret;
	}
	uint32_t sId;
	if (SGX_SUCCESS != getJsonNumber(respJson, "session_id", &sId)){
		cJSON_Delete(respJson);
		ret = (sgx_status_t)-1;
		return ret;
	}

	if (SESSION_INIT_RSP != msgT || sId != sessionId){
		if (SESSION_INIT_CREATING_ENCLAVE == msgT){
			if (SGX_SUCCESS == getJsonNumber(respJson, "tidx", &sId)){
				//return SGX_SUCCESS;
			}
		}
		cJSON_Delete(respJson);
		return (sgx_status_t)-1;
	}
	*pSessionId = sId;

	cJSON_Delete(respJson);

	//char msgBuf[128];
	snprintf(sndBuff,127,
		"{\"session_id\":%d,\"msgType\":%d}",
		sessionId, SESSION_CONFIRM);

	curBuffLen = strnlen((char *)sndBuff, 127);
	//*(uint32_t *)ioBuff = sessionId;

	//*(size_t *)(ioBuff+sizeof(uint32_t)) = curBuffLen;

	uint8_t *encBuff = encryptData((uint8_t *)sndBuff, curBuffLen, secretKey, iv, false);

	if (NULL == encBuff){
		ret = (sgx_status_t)-1;
		return ret;
		//return (sgx_status_t)-1;
	}
	memcpy(sndBuff, encBuff, curBuffLen+1);
	free(encBuff);

	//uint32_t rspSessionId = sessionId;
	rspPtr=NULL;
	ret = ocall_http2enclave(&oc_ret, getEnclaveId(), &sessionId, (uint8_t *)sndBuff, curBuffLen, &rspPtr, &rspLen, iv, 16, destUrl);

	sgx_status_t retval;
	if (NULL != rspPtr)
		ocall_get_data(&retval, &rspPtr, NULL, 0); //free(rspPtr);
	if (SGX_SUCCESS != ret || SGX_SUCCESS != oc_ret || 0 == rspLen){
		return (sgx_status_t)-1;
	}
	if (sessionId == *pSessionId)
		return SGX_SUCCESS;
	else
		return (sgx_status_t)-1;
}

sgx_status_t sendE2Emessage(sgx_ec256_public_t *destPubkey, const uint8_t *msgBuf, uint8_t **pRspBuf, size_t *msgLen){
	Cluster *subEncl = NULL;
	sgx_status_t ret=SGX_SUCCESS;
	//uint8_t *ioBuf;
#define MAX_RETRY 2

	if (NULL == enclaveCluster)
		enclaveCluster = loadEnclaveClusterFile();
	if (NULL != enclaveCluster)
		subEncl = enclaveCluster->searchSubEnclave(destPubkey);
	if (NULL == subEncl){
		return (sgx_status_t)-1;
	}

	if (0 == subEncl->sessionId)
		if (SGX_SUCCESS != initE2Esession((char *)subEncl->subEnclave.url, &(subEncl->sessionId),
				destPubkey, subEncl->secretKey))
			return (sgx_status_t)-1;

	uint8_t iv[16];

	int try_cnt=0;
	ret = SGX_SUCCESS;
	while (try_cnt<MAX_RETRY){
		uint8_t *encBuff = encryptData(msgBuf, *msgLen, subEncl->secretKey, iv, false);

		if (NULL == encBuff){
			break;
		}
		sgx_status_t oc_ret;
		uint32_t rspSessionId = subEncl->sessionId;
		uint8_t *rspPtr=NULL;
		size_t rspLen;
		sgx_status_t ret1 = ocall_http2enclave(&oc_ret, getEnclaveId(), &rspSessionId, encBuff, *msgLen, &rspPtr, &rspLen, iv, 16, (char *)subEncl->subEnclave.url);
		free(encBuff);
		if (SGX_SUCCESS != ret1 || SGX_SUCCESS != oc_ret || NULL == rspPtr || 0 == rspLen){
			ret = (sgx_status_t)-1;
		} else {
			uint8_t *rspBuf = (uint8_t *)malloc(rspLen);
			if (NULL == rspBuf){
				ocall_get_data(&oc_ret, &rspPtr, NULL, 0); //free(rspPtr);
				return (sgx_status_t)-1;
			}
			ret1 = ocall_get_data(&oc_ret, &rspPtr, rspBuf, rspLen);
			if (SGX_SUCCESS != ret1 || SGX_SUCCESS != oc_ret){
				ret = (sgx_status_t)-1;
			}
			if (SGX_SUCCESS == ret){
				ret = (sgx_status_t)-1;
				if (0 != rspSessionId){
					uint8_t dec_iv[16];
					memcpy(dec_iv, iv, 16);
					uint8_t *decMsg = decryptData(rspBuf, rspLen+1, subEncl->secretKey, dec_iv);
					if (NULL != decMsg){
						*(decMsg+rspLen) = '\0';
						if ('\{' == *decMsg){ //the response message must be a Json string.
							*pRspBuf = decMsg;
							*msgLen = rspLen;
							ret = SGX_SUCCESS;
						} else
							free(decMsg);
					}
				}
			}
			free(rspBuf);
		}
		if (SGX_SUCCESS != ret) {
			sgx_status_t ret1 = initE2Esession((char *)subEncl->subEnclave.url, &(subEncl->sessionId),
					(sgx_ec256_public_t *)destPubkey, subEncl->secretKey);
			if (SGX_SUCCESS != ret1)
				ret1 = initE2Esession((char *)subEncl->subEnclave.url, &(subEncl->sessionId),
									(sgx_ec256_public_t *)destPubkey, subEncl->secretKey);
			if (SGX_SUCCESS != ret1)
				break;
			else
				ret = SGX_SUCCESS;
		} else
			break;
		try_cnt++;
	}
	if (try_cnt>=MAX_RETRY)
		return (sgx_status_t)-1;
	return ret;
}

sgx_status_t uploadSubCode(char *key, char *title, char *code){
	sgx_ec256_public_t binKey;
	hex2bin(key, (uint8_t *)&binKey, sizeof(sgx_ec256_public_t));

	size_t msgSize=strnlen(code, MAX_CODE_LEN)*3/2+512;
	uint8_t *msgBuf = (uint8_t *)malloc(msgSize);
	if (NULL == msgBuf)
		return (sgx_status_t)-1;
	snprintf((char *)msgBuf, msgSize,
			"{\"session_id\":0,\"msgType\":%d,\"role\":%d,\"title\":\"%s\",\"stId\":1,\"code\":",
					 UPLOAD_MODEL,
					 ROLE_DATA_SCIENTIST,
					 title);
	size_t trLen = strnlen((const char*)msgBuf, 256);
	snprintf((char *)(msgBuf+trLen), msgSize-trLen, "\"%s", code);
	size_t outLen;
	jsonEscape(msgBuf+trLen+1, &outLen, msgSize-trLen-1);
	trLen += strnlen((char *)msgBuf+trLen, msgSize-trLen);
	snprintf((char *)msgBuf+trLen, msgSize-trLen, "\"}");
	trLen += strnlen((char *)msgBuf+trLen, msgSize-trLen);
	oc_print("Send sub code:", (char *)msgBuf);

	uint8_t *rspMsg=NULL;
	sgx_status_t ret = sendE2Emessage(&binKey, msgBuf, &rspMsg, &trLen);

	oc_print("Upload code response: ", (char *)rspMsg);
	if (NULL != rspMsg)
		free(rspMsg);
	return(ret);
}
