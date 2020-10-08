/*
 * task_manager_enclave.h
 *
 *  Created on: Mar 6, 2019
 *      Author: aayush
 */


#ifndef INCLUDE_TASK_MANAGER_ENCLAVE_H_
#define INCLUDE_TASK_MANAGER_ENCLAVE_H_

#include "../gt_enclave_t.h"
#include "sgx_tcrypto.h"
//#include "string"
#include <stdio.h>
#include <stdlib.h>
#include "sgx_utils.h"
#include "sgx_thread.h"
#include "cjson.h"
#include "session.h"
#include "data_management.h"
#include "vmoutput.h"
#include "../pyvm/Include/enclave_wrapper/io.h"
#include "../pyvm/Include/enclave_wrapper/sys.h"
#include <sgx_tprotected_fs.h>
#include <sys/types.h>

typedef enum _thread_status_t {
	IDLE,
	INITIALIZED,
	ACTIVE,
	INACTIVE,
	SUSPENDED,
	FINISHED,
	ERROR
}thread_status_t;

/**
 * session node class which holds information of each stakeholder
 */
class TaskTableNode{
public:

	uint64_t tid_app;
	uint64_t tid_enclave;

	thread_status_t thread_status;
	uint32_t index;
	uint64_t startTimeInSec;
	char *message;
	uint32_t taskMsgType;
	char *taskOutput;
	SessionNode *sessionNode;
	char *rspBuf;
	size_t maxLen;
	size_t curLen;
	void (*cleanUp)();
	uint8_t* result;
	size_t resultLen;

	VM_Result Result;
	jmp_buf top_env;

	/*
	 * Constructor for the Session Node
	 *     sId : the stakeholderId
	 *     currentSecond : present second on the clock
	 */
	TaskTableNode(uint32_t idx, uint64_t startTime){
		tid_app = 1;
		tid_enclave = 1;
		index = idx;
		thread_status = IDLE;
		startTimeInSec = startTime;
		taskOutput = NULL;
		rspBuf = NULL;
		message = NULL;
		curLen = 1;
		maxLen = 1;
		taskMsgType = 1;
		sessionNode = NULL;
		cleanUp=NULL;
		result=NULL;
		resultLen=0;
	}

	~TaskTableNode(){
		if (NULL != message)
			free(message);
		if (NULL != sessionNode)
			free(sessionNode);
		if (NULL != rspBuf)
			free(rspBuf);
		if (NULL != result)
			free(result);
		if (NULL != taskOutput)
			free(taskOutput);
	}


};

#define TASK_TABLE_SIZE 512
/*
 * Class which acts as a table of session nodes
 * It is a list of session nodes
 */
class TaskTable {
public:
	TaskTable();
	~TaskTable();
	TaskTableNode *newNode(uint32_t idx, uint64_t currentSecond);
	TaskTableNode *getNode(uint32_t idx);
	TaskTableNode *getNodeByTID(uint64_t tid);
	TaskTableNode *getNodeByETID(uint64_t etid);
	TaskTableNode *getFreeNode(uint64_t currentSecond);
	sgx_status_t getTaskStatus(uint64_t tid, cJSON *msgJson, char *rspBuf, size_t maxLen, size_t *pCurLen);
	void deleteNode(uint32_t idx);
	sgx_status_t clearTaskList(cJSON *msgJson, char *rspBuf, size_t maxLen, size_t *pCurLen);
	sgx_status_t killTask(cJSON *msgJson, char *rspBuf, size_t maxLen, size_t *pCurLen);
	uint32_t getLastNode(){
		return lastTaskTableNode;
	}

private:
	TaskTableNode *pTaskTableNode[TASK_TABLE_SIZE];
	uint32_t lastTaskTableNode;
};

extern TaskTable taskTable;

TaskTableNode *create_new_task(SessionNode *sNode, char *message, uint32_t msgType, char *rspBuf, size_t maxLen, size_t *pCurLen, uint64_t currentSecond);
sgx_status_t task_manager_main(SessionNode *sNode, cJSON *msgJson,
		uint32_t msgType, char *rspBuf, size_t maxLen, size_t *pCurLen, uint64_t currentSecond);
sgx_status_t setSubEnclaveOutput(uint8_t *buf, size_t bufLen);
sgx_status_t runSubEnclaveCode(char *projectName);
sgx_status_t runSingleCode(char* encName, char* OPALalgo, char* secAlgo, char* dataPth, char **output);

#endif /* INCLUDE_TASK_TABLE_H_ */
