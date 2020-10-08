/*
 * cluster.h
 *
 *  Created on: Sep 6, 2019
 *      Author: accuser
 */

#ifndef GT_ENCLAVE_INCLUDE_CLUSTER_H_
#define GT_ENCLAVE_INCLUDE_CLUSTER_H_

#include "sgx_utils.h"
#include "cjson.h"
#include "enclave_def.h"
#include "sgx_tcrypto.h"
#include "string.h"
#include "../gt_enclave_t.h"

typedef struct{
	sgx_ec256_public_t pubKey;
	uint8_t url[URL_BUFF_SIZE];
	uint8_t name[URL_BUFF_SIZE];
} SubEnclave_t;

class Cluster {
public:
	Cluster(){
		next = NULL;
		subEnclave.pubKey.gx[0] = '\0';
		subEnclave.url[0] = '\0';
		subEnclave.name[0] = '\0';
		sessionId = 0;
		tidx=0;
		running=false;
	}
	Cluster(Cluster *c){
		next = NULL;
		memcpy((void *)(&(this->subEnclave)), (const void *)(&(c->subEnclave)), sizeof(SubEnclave_t));
		memcpy(secretKey, c->secretKey, 16);
		sessionId = c->sessionId;
		tidx=c->tidx;
		running=c->running;
	}
	sgx_status_t addSubEnclave(Cluster **head);
	Cluster *searchSubEnclave(sgx_ec256_public_t *pubkey);

	SubEnclave_t subEnclave;
	uint32_t sessionId;
	uint32_t tidx;
	uint8_t secretKey[16];
	bool running;
	Cluster *next;
};

extern Cluster *enclaveCluster;

sgx_status_t deleteSubEnclave(Cluster **head, Cluster *encl);

sgx_status_t saveEnclaveClusterFile(Cluster *head);
Cluster * loadEnclaveClusterFile();

sgx_status_t initE2Esession(char *destUrl, uint32_t *pSessionId, sgx_ec256_public_t *destPubkey, uint8_t *secretKey);
sgx_status_t sendE2Emessage(sgx_ec256_public_t *destPubkey, const uint8_t *msgBuf, uint8_t **pRspBuf, size_t *msgLen);
sgx_status_t uploadSubCode(char *key, char *title, char *code);

#endif /* GT_ENCLAVE_INCLUDE_CLUSTER_H_ */
