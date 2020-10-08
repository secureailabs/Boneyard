/*
 * enclaveList.h
 *
 *  Created on: Nov 22, 2019
 *      Author: accuser
 */

#ifndef GT_APP_INCLUDE_ENCLAVE_TAB_H_
#define GT_APP_INCLUDE_ENCLAVE_TAB_H_

#include <stdlib.h>
#include <pthread.h>
#include "sgx_utils.h"
#include "enclave_control.h"
#include <stack>

#define SPARE_ENCLAVES 0

class EnclTabNode {
public:
	/*
	 * Constructor for the Session Node
	 *     sId : the stakeholderId
	 *     currentSecond : present time on the clock
	 */
	EnclTabNode(uint32_t stId, uint64_t currentSecond){
		lastActiveSecond = currentSecond;
		pEncl = NULL;
		stakeholderId=stId;
		sibling=child=NULL;
	}
	~EnclTabNode(){
	}
	void deleteNode(EnclTabNode *node, std::stack<EnclaveControl *> &freeEnclaves);
	EnclTabNode *getNodeByEID(sgx_enclave_id_t eid);
	//bool findAndDeleteNode(EnclTabNode *node, std::stack<EnclaveControl *> &freeEnclaves);
	EnclaveControl *pEncl;
	uint32_t stakeholderId;
	uint64_t lastActiveSecond;
	EnclTabNode *sibling;
	EnclTabNode *child;
};

#define ENCLAVE_TABLE_SIZE 64
/*
 * Class which acts as a table of session nodes
 * It is a list of session nodes
 */
class EnclaveTab {
public:
	EnclaveTab();
	sgx_status_t initEnclave0();
	sgx_status_t initMutex();
	sgx_status_t initEnclaveHome(char *enclHome);
	~EnclaveTab();
	EnclTabNode * newEnclTabNode(uint32_t stId, const uint8_t *message, size_t msgLen, uint8_t *iv,
			uint8_t *respBuff, size_t respBuffSize, size_t *respSize);
	EnclaveControl *createEnclave();
	EnclTabNode * getNode(uint32_t stakeholderId);
	EnclTabNode * getNodeLock(uint32_t stakeholderId);
	//void delTabNode(uint32_t stakeholderId);
	//void delTabNode(EnclTabNode *node);
	void scanTable(uint64_t currentSecond);
	sgx_status_t newSession(const char *ethPubkey, uint32_t *session_id, const char *message, void *resultVecPtr, uint8_t *iv);
	sgx_status_t session(sgx_status_t *retval, uint32_t session_id, const uint8_t *message, size_t mLen,
			void *resultVecPtr, uint8_t *iv);
	void initFreeEnclaves();
	std::stack<EnclaveControl *> freeEnclaves;

private:
	EnclTabNode *pTabNode[ENCLAVE_TABLE_SIZE];
	uint64_t lastScanSecond;
	uint32_t scanCycleTime;
	pthread_mutex_t lock, stackLock;
	char *enclaveHome;

};

extern EnclaveTab *enclaveTab;


#endif /* GT_APP_INCLUDE_ENCLAVE_TAB_H_ */
