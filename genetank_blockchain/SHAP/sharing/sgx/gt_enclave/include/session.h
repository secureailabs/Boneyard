/*
 * session.h
 *
 *  Created on: Nov 19, 2018
 *      Author: accuser
 */


#ifndef INCLUDE_SESSION_H_
#define INCLUDE_SESSION_H_

#include <stdlib.h>

#include "sgx_utils.h"
#include "sgx_thread.h"
#include "sgx_tcrypto.h"
#include "../gt_enclave_t.h"
#include "cluster.h"

#include "cjson.h"

#define RUN_INTERACTIVE_SCRIPT (1<<1)
#define CALL_HTTP (1<<2)
#define RUN_FEDERATED_CYCLE (1<<3)
#define SUBMIT_FEDERATED_DATA (1<<4)
#define RETRIEVE_FEDERATED_DATA (1<<5)
#define CREATE_CHILD_ENCLAVE (1<<6)
#define PRINT_PLOT (1<<7)

/**
 * session node class which holds information of each stakeholder
 */
class SessionNode {
public:
	/*
	 * Constructor for the Session Node
	 *     sId : the stakeholderId
	 *     currentSecond : present time on the clock
	 */
	SessionNode(uint64_t currentSecond){
		lastActiveSecond = currentSecond;
		sessionId = 0;
		confirmed = false;
		role=0;
		//stakeholderId=0;
		threadId = 0;
		ctr_num_bit_size = 32;
		next=NULL;
	}
	~SessionNode(){
	}
	uint32_t sessionId;
//	eth_addr_t ethAddr;
	uint8_t role; //0:runner; 1:host; 2,3: reserved; 4: guest
	//uint32_t stakeholderId;
	sgx_thread_t threadId;
	sgx_key_128bit_t secretKey; // user files secret key
	uint64_t lastActiveSecond;
	bool confirmed;
	uint32_t ctr_num_bit_size;
	SessionNode *next;
};

#define SESSION_TABLE_SIZE 512
/*
 * Class which acts as a table of session nodes
 * It is a list of session nodes
 */
class SessionTable {
public:
	SessionTable();
	~SessionTable();
	sgx_status_t insertNode(SessionNode *node);
	SessionNode * getNode(uint32_t sessionId, uint64_t currentSecond);
	sgx_status_t deleteNode(uint32_t sessionId);
	void scanTable(uint64_t currentSecond);

	SessionNode *pSessions[SESSION_TABLE_SIZE];

private:
	uint32_t lastSessionId;
	uint64_t lastScanSecond;
	uint32_t scanCycleTime;

};

char *oc_http_send(const char *req);

uint32_t getStakeHolderId();
sgx_enclave_id_t getEnclaveId();

sgx_status_t handleCommand(cJSON *msgJson, SessionNode *node, uint32_t msgType, char * rspBuf, size_t maxLen, size_t *pCurLen);
sgx_status_t runnerCmd(cJSON *msgJson, SessionNode *node, uint32_t msgType, char * rspBuf, size_t maxLen, size_t *pCurLen);
sgx_status_t commonCmd(cJSON * msgJson, SessionNode *node, uint32_t msgType, char * rspBuf, size_t maxLen, size_t *pCurLen);
sgx_status_t hostCmd(cJSON * msgJson, SessionNode *node, uint32_t msgType, char * rspBuf, size_t maxLen, size_t *pCurLen);
sgx_status_t guestCmd(cJSON * msgJson, SessionNode *node, uint32_t msgType, char * rspBuf, size_t maxLen, size_t *pCurLen);
sgx_status_t runTrainingSandbox(const char *sandboxPath, const char *modelPath, const char *cmdStr, char **output);
sgx_status_t runOneSubCode(Cluster *subEncl, const char *name, const char *dataPath, const char *secAlgo);

sgx_status_t initSession(uint32_t stId, sgx_enclave_id_t eid, uint32_t apiPermissions, bool initSessionTab, sgx_key_128bit_t connKey);
void resetSession();

#endif /* INCLUDE_SESSION_H_ */
