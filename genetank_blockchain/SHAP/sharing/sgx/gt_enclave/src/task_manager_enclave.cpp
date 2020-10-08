
#include "task_manager_enclave.h"
#include "session.h"
#include "data_management.h"
#include "gt_enclave.h"
#include "projects.h"
#include "util.h"
#include "conversion.h"
#include "msg_def.h"

sgx_thread_mutex_t tmTableLock = SGX_THREAD_MUTEX_INITIALIZER;

TaskTable taskTable;

typedef struct _cleanUpFunc{
	uint32_t messageType;
	void (*func)();
}cleanUnFunc_t;

cleanUnFunc_t cleanUpFuncTable[] = {
		{RUN_FEDERATED_MODEL, cleanUpHubOnKill},
		{0, NULL}
};

void (*getFuncByType(uint32_t msgType))(void){
	uint32_t i=0;
	while (0 != cleanUpFuncTable[i].messageType){
		if (msgType == cleanUpFuncTable[i].messageType)
			return cleanUpFuncTable[i].func;
		i++;
	}
	return NULL;
}

/**
 * Constructor for the node table
 */
TaskTable::TaskTable(){
	for(uint32_t i=0; i<TASK_TABLE_SIZE; i++ ){
		pTaskTableNode[i]=NULL;
	}
	lastTaskTableNode=1;
}

/*
 * Destructor for the node table
 */
TaskTable::~TaskTable(){
	for(uint32_t i=0; i<TASK_TABLE_SIZE; i++ ){
		TaskTableNode *node = pTaskTableNode[i];
		if(node != NULL){
			delete node;
		}
	}
}

void TaskTable::deleteNode(uint32_t idx){
	TaskTableNode *node = pTaskTableNode[idx];
		if(node != NULL){
			delete(node);
		}
	pTaskTableNode[idx] = NULL;
}

/**
 * Insert a node into the table
 *     node : the node to be inserted
 */
TaskTableNode* TaskTable::getFreeNode(uint64_t currentSecond){//need a lock
	sgx_thread_mutex_lock(&tmTableLock);
	for(uint32_t i = 1; i < TASK_TABLE_SIZE; i++){
		TaskTableNode *node = pTaskTableNode[i];
		if(node == NULL){
			node = newNode(i,currentSecond);
			sgx_thread_mutex_unlock(&tmTableLock);
			return node;
		}
	}
	sgx_thread_mutex_unlock(&tmTableLock);
	return NULL;
}

/**
 * Gets a node with the given index value from the table
 *     idx : the index at which the node is located
 */
TaskTableNode* TaskTable::getNode(uint32_t idx){//need a lock
    sgx_thread_mutex_lock(&tmTableLock);
	for(uint32_t i = 0; i < TASK_TABLE_SIZE; i++){
		TaskTableNode *node = pTaskTableNode[i];
            if(node != NULL && node->index == idx){
	        sgx_thread_mutex_unlock(&tmTableLock);
			return node;
		}
	}
    sgx_thread_mutex_unlock(&tmTableLock);
	return NULL;
	//return(pTaskTableNode[idx]);
}

/**
 * Gets a node with the given thread ID value from the table
 *     tid : the thread ID outside of the enclave.
 */
TaskTableNode* TaskTable::getNodeByTID(uint64_t tid){
	TaskTableNode *node;
	TaskTableNode *outNode = NULL;
    sgx_thread_mutex_lock(&tmTableLock);
	for(uint32_t i = 0; i < TASK_TABLE_SIZE; i++){
		node = pTaskTableNode[i];
        if(node != NULL && node->tid_app == tid){
        	outNode = node;
	        break;
		}
	}
    sgx_thread_mutex_unlock(&tmTableLock);
	return outNode;
}

/**
 * Gets a node with the given thread ID value from the table
 *     etid : the thread ID inside of the enclave.
 */
TaskTableNode* TaskTable::getNodeByETID(uint64_t etid){
	TaskTableNode *node;
	TaskTableNode *outNode = NULL;
    sgx_thread_mutex_lock(&tmTableLock);
	for(uint32_t i = 0; i < TASK_TABLE_SIZE; i++){
		node = pTaskTableNode[i];
        if(node != NULL && node->tid_enclave == etid){
        	outNode = node;
	        break;
		}
	}
    sgx_thread_mutex_unlock(&tmTableLock);
	return outNode;
}

/*
 * Create a new node at index i and store its address in pTaskTableNode array
 */
TaskTableNode* TaskTable::newNode(uint32_t i, uint64_t currentSecond){
  	TaskTableNode *nNode = new TaskTableNode(i,currentSecond);
	pTaskTableNode[i] = nNode;
	return nNode;
}	

/*
 * Copy quoted string
 */
void copyQuotedStr(char *dstBuf, size_t maxLen, char *srcBuf, size_t srcLen){
	size_t cpLen=0;
	size_t srcCnt=0;
	while (cpLen<maxLen && srcCnt<srcLen){
		if ('\"' == *(srcBuf+srcCnt)){
			*(dstBuf+cpLen) = '\\';
			cpLen++;
		}
		*(dstBuf+cpLen)=*(srcBuf+srcCnt);
		cpLen++;
		srcCnt++;
	}
	if (cpLen<maxLen)
		*(dstBuf+cpLen) = '\0';
}

void jsonEscapeNoQuote(uint8_t *buf, size_t *pInSize, size_t maxSize);

/**
 * Fill in the rsp buffer with the status from the tasktable
 * idx : index of the tasktableNode
 */
sgx_status_t generateStatusOutput(uint32_t i, char *rspBuf, size_t maxLen, size_t *pCurLen, TaskTableNode *tmNode){

	size_t curLen = *pCurLen;

	const char *statusType[] = {"IDLE","INITIALIZED","ACTIVE","INACTIVE","SUSPENDED","FINISHED","ERROR"};

	snprintf((rspBuf+curLen), maxLen-curLen-1, "{\"tid\":%lu", (tmNode->tid_app));
		curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);

	snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"taskType\":%d", (tmNode->taskMsgType));
	curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);

	if(tmNode->rspBuf != NULL){
		snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"output\":\"");
		curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		size_t rspSize = (maxLen-curLen-1)>tmNode->curLen?tmNode->curLen:(maxLen-curLen-1);
		memcpy((rspBuf+curLen), tmNode->rspBuf, rspSize);
		*(rspBuf+curLen+rspSize) = '\0';
		//size_t escSize;
		jsonEscape((uint8_t*)(rspBuf+curLen), &rspSize, maxLen-curLen-1);
		curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
	}

	snprintf((rspBuf+curLen), maxLen-curLen-1, "\",\"startTime\":%lu", tmNode->startTimeInSec);
	curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);

	snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"taskStatus\":\"%s\"},", statusType[tmNode->thread_status]);
	curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);

	*pCurLen = curLen;

	return SGX_SUCCESS;
}

/**
 * populates the rsp buffer with the task details of the first task table node found10j-
 */
sgx_status_t TaskTable::getTaskStatus(uint64_t tidx, cJSON *msgJson, char *rspBuf, size_t maxLen, size_t *pCurLen){
	sgx_status_t ret=SGX_SUCCESS;

	size_t curLen = *pCurLen;

	snprintf((rspBuf+curLen), maxLen-curLen-1, ", \"taskDetails\" : [ ");
	curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);

	*pCurLen = curLen;

	if (tidx<TASK_TABLE_SIZE && pTaskTableNode[tidx] != NULL){
		ret = generateStatusOutput(tidx, rspBuf, maxLen, pCurLen, pTaskTableNode[tidx]);
		if (NULL != pTaskTableNode[tidx]->result){
			curLen = *pCurLen -2;
			snprintf((rspBuf+curLen), maxLen-curLen-1, ", \"federatedData\":\"%s\",\"dataSize\":%lu},",
					pTaskTableNode[tidx]->result, pTaskTableNode[tidx]->resultLen);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
			*pCurLen = curLen;
		}
	}
	if (tidx > TASK_TABLE_SIZE) {
		for(uint32_t i = 0; i < TASK_TABLE_SIZE; i++){
			if(pTaskTableNode[i] != NULL){
				ret = generateStatusOutput(i, rspBuf, maxLen, pCurLen,pTaskTableNode[i]);
			}
		}
	}
	curLen = *pCurLen;
	snprintf((rspBuf+curLen-1), maxLen-curLen-1, "], \"msgType\":%d", GET_TASK_STATUS_SUCCESS);
	curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);

	*pCurLen = curLen;

	return ret;
}

sgx_status_t TaskTable::clearTaskList(cJSON *msgJson, char *rspBuf, size_t maxLen, size_t *pCurLen){

	size_t curLen = *pCurLen;

	for(uint32_t i = 0; i < TASK_TABLE_SIZE; i++){
		if(pTaskTableNode[i] != NULL){
			//if ((0 == stId) || stId == pTaskTableNode[i]->sessionNode->stakeholderId) {
				if (FINISHED != pTaskTableNode[i]->thread_status){
					sgx_status_t retval, ret;
					if (NULL != pTaskTableNode[i]->cleanUp)
						(pTaskTableNode[i]->cleanUp)();
					ret = ocall_task_manager(&retval, -1, pTaskTableNode[i]->tid_app, 0);
					if (!(SGX_SUCCESS == ret && SGX_SUCCESS == retval)){
						snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"killFail\":%lu", pTaskTableNode[i]->tid_app);
						curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
						*pCurLen = curLen;
						continue;
					}
				}
				TaskTableNode *node = pTaskTableNode[i];
				delete(node);
				pTaskTableNode[i] = NULL;
			//}
		}
	}
	return SGX_SUCCESS;
}

sgx_status_t TaskTable::killTask(cJSON *msgJson, char *rspBuf, size_t maxLen, size_t *pCurLen){
	uint64_t tid;
	size_t curLen = *pCurLen;
	if (SGX_SUCCESS != getJsonNumberDouble(msgJson, "tid", &tid)){
		snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", KILL_TASK_FAIL);
		curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		*pCurLen = curLen;
		return (sgx_status_t) -1;
	}

	for(uint32_t i = 0; i < TASK_TABLE_SIZE; i++){
		if(pTaskTableNode[i] != NULL){
			if (tid == pTaskTableNode[i]->tid_app){
				//if ((0 == stId) || stId == pTaskTableNode[i]->sessionNode->stakeholderId) {
					if (NULL != pTaskTableNode[i]->cleanUp)
						(pTaskTableNode[i]->cleanUp)();
					TaskTableNode *node = pTaskTableNode[i];
					delete(node);
					pTaskTableNode[i] = NULL;
					sgx_status_t retval, ret;
					ret = ocall_task_manager(&retval, -1,tid, 0);
					if (SGX_SUCCESS == ret && SGX_SUCCESS == retval){
						snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", KILL_TASK_SUCCESS);
						curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
						*pCurLen = curLen;
						return SGX_SUCCESS;
					} else {
						snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", KILL_TASK_FAIL);
						curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
						*pCurLen = curLen;
						return (sgx_status_t) -1;
					}
				//}
			}
		}
	}
	return SGX_SUCCESS;
}

sgx_status_t runSingleCode(char* encName, char* OPALalgo, char* secAlgo, char* dataset, char **output){
	sgx_status_t ret = (sgx_status_t)-1;
	if (NULL != encName && '\0' != encName[0]){
		if (NULL == enclaveCluster)
			enclaveCluster = loadEnclaveClusterFile();
		Cluster *encl = enclaveCluster;
		while (NULL != encl){
			if (!strncmp(encName, (const char *)(encl->subEnclave.name), URL_BUFF_SIZE))
				break;
			encl = encl->next;
		}
		if (NULL != encl)
			ret = runOneSubCode(encl, OPALalgo, dataset, secAlgo);
		*output = NULL;
	} else {
		char cmdStr[256];
		snprintf(cmdStr, 255, "{\"session_id\":0,\"msgType\":%d,\"name\":\"%s\",\"dataPath\":\"%s\"}",
			RUN_MODEL_FILE, secAlgo, dataset);
		ret = runTrainingSandbox(dataset, "", cmdStr, output);
	}
    return ret;
}

sgx_status_t runSubEnclaveCode(char *projectName){
	SessionNode *node = new SessionNode(0);
	if (NULL == node)
		return (sgx_status_t)-1;
	node->confirmed = true;
	node->role=1;
	node->sessionId = 8080;
	uint32_t msgType=HOST_RUN_SUB_CODE_CYCLE;
	char rspBuf[256];
	char *reqBuf = (char *)malloc(256);
	if (NULL == reqBuf)
		return (sgx_status_t)-1;
	size_t maxLen=256;
	snprintf((char *)reqBuf, 255,"{\"name\":\"%s\"}", projectName);

	subResultQueue.startQueue();
	snprintf(rspBuf,maxLen-1,"{\"session_id\":%d",node->sessionId);
	size_t curLen = strnlen(rspBuf, maxLen);
	TaskTableNode *tNode = create_new_task(node, reqBuf, msgType,
			rspBuf, maxLen, &curLen, 0);
	delete node;

	if (NULL == tNode)
		return (sgx_status_t)-1;
	return SGX_SUCCESS;
}

TaskTableNode *create_new_task(SessionNode *sNode, char *message, uint32_t msgType, char *rspBuf, size_t maxLen, size_t *pCurLen, uint64_t currentSecond){
	TaskTableNode *node = taskTable.getFreeNode(currentSecond);
	if (NULL == node)
		return NULL;
	//node->isBackground = true;
	//store information in the node
	node->message = message;
	node->taskMsgType = msgType;
	node->cleanUp = getFuncByType(msgType);
	node->maxLen = maxLen;
	node->rspBuf = (char *)malloc(maxLen);
	if (node->rspBuf == NULL){
		node->thread_status = ERROR;
		return NULL;
	}
	strncpy(node->rspBuf, rspBuf, *pCurLen);
	node->curLen = *pCurLen;
	SessionNode *sNodeCopy = (SessionNode*)malloc(sizeof(SessionNode));
	if (NULL == sNodeCopy){
		node->thread_status = ERROR;
		free(node->rspBuf);
		node->rspBuf = NULL;
		return NULL;
	}
	memcpy(sNodeCopy,sNode,sizeof(SessionNode));
	node->sessionNode = sNodeCopy;
	//node->sessionId = sNode->sessionId;
	node->thread_status = INITIALIZED;
	sgx_status_t retval;
	if (SGX_SUCCESS == ocall_task_manager(&retval, node->index, 0, getStakeHolderId())){
		if (SGX_SUCCESS == retval)
			return node;
	}
	return NULL;
}

sgx_status_t task_manager_main(SessionNode *sNode, cJSON *msgJson,
		uint32_t msgType, char *rspBuf, size_t maxLen, size_t *pCurLen,//uint8_t *iv, size_t ivLen,
		uint64_t currentSecond){
	sgx_status_t retval=SGX_SUCCESS;

	if(msgType == GET_TASK_STATUS){
		uint32_t tidx;
		if (SGX_SUCCESS != getJsonNumber(msgJson, "tidx", &tidx)){
			tidx = TASK_TABLE_SIZE+1;
		}
		return taskTable.getTaskStatus(tidx, msgJson, rspBuf, maxLen,pCurLen);
	} else if(msgType == CLEAR_TASK_LIST) {
		size_t curLen = *pCurLen;
		retval = taskTable.clearTaskList(msgJson,rspBuf,maxLen,pCurLen);
		snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"msgType\":%d", CLEAR_TASK_LIST_SUCCESS);
		curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		*pCurLen = curLen;

	} else if (msgType == KILL_TASK){
		return taskTable.killTask(msgJson, rspBuf, maxLen,pCurLen);
	} else {
		char *bgfg = getJsonStringPtr(msgJson, "bgfg");
		if(bgfg==NULL){ //Foreground task
			sgx_status_t ret = handleCommand(msgJson, sNode, msgType, rspBuf, maxLen, pCurLen);
			return ret;
		}

		char *message = cJSON_PrintUnformatted(msgJson);
		TaskTableNode *node = create_new_task(sNode, message, msgType, rspBuf, maxLen, pCurLen, currentSecond);

		size_t curLen = *pCurLen;
		if(NULL != node){
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"result\":\"Task started in the background\",\"msgType\":%d,\"tidx\":%d",
					START_TASK_SUCCESS, node->index);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
			*pCurLen = curLen;
			return SGX_SUCCESS;
		} else {
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"result\":\"Error starting task in the background\",\"msgType\":%d", START_TASK_FAIL);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
			*pCurLen = curLen;
			return (sgx_status_t) -1;
		}

	}
	if(retval != SGX_SUCCESS){
		return (sgx_status_t) -1;
	}
	return SGX_SUCCESS;
}

sgx_status_t ec_task_manager(uint64_t tid_app, uint32_t index){
	//read information from the task table
	TaskTableNode *node = taskTable.getNode(index);

	//only start the process when status is INITIALIZED
	if(node->thread_status != INITIALIZED){
		return (sgx_status_t)-1;
	}
	node->tid_app = tid_app;
	node->tid_enclave = sgx_thread_self();
	//char *rspBuf;

	cJSON *msgJson = cJSON_Parse(node->message);
	if (NULL == msgJson){
		node->thread_status = ERROR;
		return (sgx_status_t)-1;
	}

	node->thread_status = ACTIVE;

	sgx_status_t ret=SGX_SUCCESS;
	ret = handleCommand(msgJson, node->sessionNode, node->taskMsgType, node->rspBuf, node->maxLen,
			&(node->curLen));

	snprintf((node->rspBuf+node->curLen), node->maxLen-node->curLen-1, "}");
	node->curLen += strnlen((char *)(node->rspBuf+node->curLen), node->maxLen-node->curLen);

	node->tid_enclave = (uint64_t)-1;
	node->thread_status = FINISHED;

	cJSON_Delete(msgJson);

	return ret;
}

