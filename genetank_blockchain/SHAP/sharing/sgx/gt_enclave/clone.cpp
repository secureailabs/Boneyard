/*
 * clone.cpp
 *
 *  Created on: Aug 20, 2018
 *      Author: accuser
 */

#include "gt_enclave_t.h"

#include <stddef.h>
#include "sgx_trts.h"
#include "sgx_error.h"
#include "sgx_tcrypto.h"
//extern "C"{
#include "./include/data_management.h"
#include "./include/dir_management.h"
//}
#include "msg_def.h"

sgx_status_t ec_cloneRecieveModel(const char *fileName, size_t nameSize, const char *data, size_t dataLen,
		eth_addr_t *pEthAddr){
	sgx_status_t ret;
	StakeholderRec_t sRec;

	memcpy(sRec.ethAddr, pEthAddr, sizeof(eth_addr_t));
	if (SGX_SUCCESS != sgx_sha256_msg((const uint8_t *)data, dataLen, &(sRec.dataHash)))
		return (sgx_status_t)(-1);

	ret = sgx_read_rand((unsigned char *)&(sRec.encKey), sizeof(sgx_key_128bit_t));
	if (SGX_SUCCESS != ret)
		return (sgx_status_t)(-1);

	sRec.seqNumber = (uint32_t)-1;
	ret = dmAppendStake(&sRec);
	if (SGX_SUCCESS != ret)
		return (sgx_status_t)(-1);

	ret = dmWriteModelFile(fileName, (const uint8_t *)data, dataLen);
	if (SGX_SUCCESS != ret)
		return (sgx_status_t)(-1);

	return SGX_SUCCESS;
}

sgx_status_t ec_getModelList(char *buf, size_t bufSize, size_t *pFileLen){
	size_t fileLen = bufSize;
	sgx_status_t ret =  dmGetDirList(modelDir, NULL, buf, &fileLen);
	*pFileLen = fileLen;
	return ret;
}

sgx_status_t ec_getModel(const char *fileName, size_t nameSize,	char *buf, size_t maxBufLen, size_t *pRealLen,
		eth_addr_t *pEthAddr){
	size_t realLen = maxBufLen;
	sgx_status_t ret = dmReadModelFile(fileName, (uint8_t *)buf, &realLen);
	return ret;
}





