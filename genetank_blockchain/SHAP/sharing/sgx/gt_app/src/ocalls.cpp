#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include "../gt_enclave_u.h"
#include "task_manager_app.h"
#include "stdafx.h"
#include "network_client.h"
#include "conversion.h"
#include "enclave_tab.h"

using namespace std;
using namespace web;
using namespace utility;
using namespace http;

//----------for lib loading
sgx_status_t ocall_load_base(const char *name, char *buf, size_t len){

    FILE *fp=fopen(name,"r");
    if(fp!=NULL){
    	size_t newLen=fread(buf,sizeof(char),len,fp);
    	if(ferror(fp)!=0){
    		fputs("Error reading file", stderr);
    	}
    	else{
    		buf[newLen++]='\0';
    	}
    }
    else{
    	return sgx_status_t(-1);
    }
    fclose(fp);

    return SGX_SUCCESS;
}

sgx_status_t ocall_send_result(const uint8_t *result, size_t resultSize, void *buff){
	vector <uint8_t> &buffVec = * (vector <uint8_t> *)buff;
	buffVec.assign(result, result+resultSize);
	return SGX_SUCCESS;
}

sgx_status_t ocall_get_data(uint8_t **rspPtr, uint8_t *rspBuf, size_t msgLen){
	if (msgLen > 0)
		memcpy(rspBuf, *rspPtr, msgLen);
	free(*rspPtr);
	return SGX_SUCCESS;
}

//------for timer
sgx_status_t ocall_gettime(double* tm){
	pthread_t tid = pthread_self();
	struct timespec tp;
	int res = timespec_get(&tp, TIME_UTC);
	if(0==res){
		return sgx_status_t(-1);
	}
	else{
		*tm=(double)tp.tv_sec+1e-9*(double)tp.tv_nsec;
		return SGX_SUCCESS;
	}
}

/*
 * tid: external thread id, if not zero then kill the thread
 */
sgx_status_t ocall_task_manager(uint32_t index,uint64_t tid, uint32_t stId){
	return task_manager_app(index, tid, stId);
}

sgx_status_t ocall_sleep(uint32_t seconds){
	sleep(seconds);
	return SGX_SUCCESS;
}

sgx_status_t ocall_http_send(const char *req, size_t reqSize, uint8_t **rspPtr, size_t *rspSize){
	sgx_status_t ret;
	json::value reqObj, rspObj;

	reqObj = json::value::parse(req);
	ret = rest_request(reqObj, rspObj);
	string_t rspStr = rspObj.serialize();
	*rspSize = rspStr.size();
	uint8_t *saveData = (uint8_t *)malloc(*rspSize);
	memcpy((void *)saveData, (const void *)rspStr.c_str(), *rspSize);
	*rspPtr = saveData;
	return SGX_SUCCESS;
}

sgx_status_t ocall_http2enclave(sgx_enclave_id_t eid, uint32_t *pSession, uint8_t * buff, size_t msgLen, uint8_t **rspPtr, size_t *rspLen,
		uint8_t *iv, size_t iv_len, char *url){
	sgx_status_t retval;

	//ucout << U("Buffer: ") << buff << endl;
	json::value objTmp1;// = json::value::parse(buff);

	uint32_t sessionId = *pSession; //*(uint32_t *)buff;
	if (0 == sessionId){
		objTmp1 = json::value::parse((char *)buff);
	} else {
		objTmp1[U("session_id")] = json::value::number(sessionId);

		uint8_t iv_hex[33];
		iv_hex[32]=0;
		bin2hex((const unsigned char *)iv, 16, (char *)iv_hex);
		string_t iv_str = (char *)iv_hex;
		objTmp1[U("aes_iv")] = json::value::string(iv_str);

		utility::string_t content;
		if (sessionId == 0)
			content = (const char *)buff;
		else{
			vector <unsigned char> vcontent(buff, buff+msgLen);
			content = utility::conversions::to_base64(vcontent);
		}
		objTmp1[U("content")] = json::value::string(content);
	}

	utility::string_t eurl((const char *)url);
	retval = http_send_2enclave(eurl, (json::value &) objTmp1);

	// copy json to the buff
	if (SGX_SUCCESS == retval){
		*pSession = objTmp1[U("session_id")].as_integer();
		string_t iv_string = objTmp1[U("aes_iv")].as_string();
		hex2bin(iv_string.c_str(), iv, 16);
		size_t rspOrigLen = objTmp1[U("contentLen")].as_integer();
		string_t rspContent = objTmp1[U("content")].as_string();
		uint8_t *rspBuf = base64_decode(rspContent.c_str(), rspOrigLen, rspLen);
		*rspPtr = rspBuf;
	}

	return retval;
}

sgx_status_t ocall_print(const char *str, size_t len){
	//printf(str);
	ucout << str ;
	return SGX_SUCCESS;
}

void *create_enclave(void *stId){
	uint8_t *reqBuff = NULL;
	size_t reqLen=0;
	uint8_t iv[16];
	uint8_t *rspBuf=NULL;
	size_t bufSize=0;
	size_t rspSize;
	enclaveTab->newEnclTabNode(*(uint32_t *)stId, reqBuff, reqLen, iv, rspBuf, bufSize, &rspSize);
	free(stId);
}

sgx_status_t ocall_new_session(uint32_t stId, const char *message, size_t msgLen, uint32_t *pSessionId, uint8_t *iv, size_t ivLen,
		uint8_t *outBuf, size_t bufLen, size_t *pResultLen){
	if (NULL == enclaveTab)
		return (sgx_status_t)-1;

	EnclTabNode *tabNode = enclaveTab->getNode(stId);
	std::vector <uint8_t> rspMsg(1024);
	sgx_status_t ret = (sgx_status_t)-1;
	*pResultLen = 0;
	if (NULL != tabNode) {
		if (NULL == tabNode->pEncl){
			tabNode = enclaveTab->getNodeLock(stId);
		}
		if (NULL != tabNode->pEncl){
			ret = tabNode->pEncl->new_session(pSessionId, message, (void *)&rspMsg, iv);
			if (SGX_SUCCESS == ret){
				*pResultLen = rspMsg.size();
				size_t len = bufLen>*pResultLen?*pResultLen:bufLen;
				memcpy(outBuf, &(rspMsg[0]), len);
			} else {
				*outBuf = '\0';
				*pResultLen = 0;
			}
			return SGX_SUCCESS;
		}
	}
	pthread_t tid;
	uint32_t *pStId = (uint32_t*)malloc(sizeof(uint32_t));
	*pStId = stId;
	if(pthread_create(&tid, NULL, create_enclave, (void*)pStId) != 0){
		return (sgx_status_t) -1;
	}
	return (sgx_status_t) -1;
}

sgx_status_t ocall_create_enclave(uint32_t srcEID, uint32_t stId, const uint8_t *reqBuff, size_t reqLen,
		uint8_t *rspBuf, size_t bufSize, size_t *rspSize, uint8_t *iv, size_t ivLen){
	if (NULL == enclaveTab)
		return (sgx_status_t)-1;

	EnclTabNode *tabNode = enclaveTab->getNode(stId);
	if (NULL == tabNode){
		return (sgx_status_t)-1;
	}
	EnclTabNode *srcNode = tabNode->getNodeByEID(srcEID);
	if (NULL != srcNode){
		EnclTabNode *childNode = new EnclTabNode(stId, currentSecond());
		if (NULL != childNode){
			EnclaveControl *pEncl = enclaveTab->createEnclave();
			if (NULL != pEncl && SGX_SUCCESS == pEncl->init_enclave(stId, reqBuff, reqLen, iv, rspBuf, bufSize, rspSize)){
				childNode->pEncl = pEncl;
				childNode->sibling = srcNode->child;
				srcNode->child = childNode;
				return SGX_SUCCESS;
			}
			delete childNode;
			if (NULL != pEncl)
				delete pEncl;
		}
	}
	return (sgx_status_t)-1;
}

sgx_status_t ocall_run_command(sgx_enclave_id_t srcEID, sgx_enclave_id_t destEID, uint32_t stId, uint8_t *encBuff, size_t bufLen,
			uint8_t **rspBuf, size_t *rspSize, uint8_t *iv, size_t ivLen){
	if (NULL == enclaveTab)
		return (sgx_status_t)-1;
	EnclTabNode *tabNode = enclaveTab->getNode(stId);
	if (NULL == tabNode)
		return (sgx_status_t)-1;
	EnclTabNode *srcNode = tabNode->getNodeByEID(srcEID);
	if (NULL == srcNode)
		return (sgx_status_t)-1;
	EnclTabNode *destNode = srcNode->child;
	while (NULL != destNode){
		if (NULL != destNode->pEncl && destEID == destNode->pEncl->getEnclaveId())
			break;
		destNode = destNode->sibling;
	}
	if (NULL == destNode || NULL == destNode->pEncl)
		return (sgx_status_t)-1;
	std::vector <unsigned char> rspMsg(1024);
	sgx_status_t ret = destNode->pEncl->run_command(encBuff, bufLen, (void *)&rspMsg, iv);
	if (SGX_SUCCESS != ret)
		return ret;
	size_t size = rspMsg.size();
	uint8_t *buf = (uint8_t *)malloc(size);
	*rspBuf = buf;
	*rspSize = size;
	if (NULL == buf){
		*rspSize = 0;
		return (sgx_status_t)-1;
	}
	memcpy(buf, &(rspMsg[0]), size);
	return SGX_SUCCESS;
	//ret = srcNode->pEncl->send_in_data(&(rspMsg[0]), rspMsg.size(), rspBuf);
}

sgx_status_t ocall_destroy_enclave(sgx_enclave_id_t srcEID, sgx_enclave_id_t destEID, uint32_t stId){
	if (NULL == enclaveTab)
		return (sgx_status_t)-1;
	EnclTabNode *tabNode = enclaveTab->getNode(stId);
	if (NULL == tabNode)
		return (sgx_status_t)-1;
	EnclTabNode *srcNode = tabNode->getNodeByEID(srcEID);
	if (NULL == srcNode)
		return (sgx_status_t)-1;
	EnclTabNode *destNode = srcNode->child;
	EnclTabNode *dNode;
	dNode = destNode;
	while (NULL != destNode){
		if (NULL != destNode->pEncl && destEID == destNode->pEncl->getEnclaveId())
			break;
		dNode = destNode;
		destNode = destNode->sibling;
	}
	if (NULL == destNode)
		return (sgx_status_t)-1;
	if (dNode == destNode){
		srcNode->child = destNode->sibling;
	} else
		dNode->sibling = destNode->sibling;
	destNode->sibling = NULL;
	destNode->deleteNode(destNode, enclaveTab->freeEnclaves);
}
