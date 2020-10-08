/*
 * EnclaveTab.cpp
 *
 *  Created on: Nov 22, 2019
 *      Author: accuser
 */
#include "stdafx.h"
#include "enclave_tab.h"
#include "tools.h"
#include <string.h>
#include <pthread.h>
#include <vector>

EnclaveTab *enclaveTab=NULL;

EnclaveTab::EnclaveTab(){
	for(int i=0; i<ENCLAVE_TABLE_SIZE; i++ )
		pTabNode[i]=NULL;
	lastScanSecond=0;
	scanCycleTime=600;
	enclaveHome = NULL;
}

sgx_status_t EnclaveTab::initEnclave0(){
	uint8_t *message = NULL;
	size_t msgLen=0;
	uint8_t iv[16];
	uint8_t *respBuff=NULL;
	size_t respBuffSize=0;
	size_t respSize;
	if (NULL == newEnclTabNode(HOST_STAKEHOLDER_ID, message, msgLen, iv, respBuff, respBuffSize, &respSize))
		return (sgx_status_t)-1;
	return SGX_SUCCESS;
}

sgx_status_t EnclaveTab::initMutex(){
    if (pthread_mutex_init(&lock, NULL) != 0) {
        std::cout<<"\n mutex init has failed\n";
		return (sgx_status_t)-1;
    }
    if (pthread_mutex_init(&stackLock, NULL) != 0) {
        std::cout<<"\n mutex init has failed\n";
		return (sgx_status_t)-1;
    }
	return SGX_SUCCESS;
}

sgx_status_t EnclaveTab::initEnclaveHome(char *enclHome){
	enclaveHome = (char *)malloc(strnlen(enclHome, MAX_PATH)+1);
	if (NULL != enclaveHome) {
		snprintf(enclaveHome, strnlen(enclHome, MAX_PATH)+1, "%s", enclHome);
		return SGX_SUCCESS;
	} else
		return (sgx_status_t)-1;
}

void EnclaveTab::initFreeEnclaves(){
	EnclaveControl *pEncl;
	for(int i=0; i<SPARE_ENCLAVES; i++){
		pEncl = new EnclaveControl();
		if (NULL != pEncl){
			pEncl->create_enclave(enclaveHome);
			freeEnclaves.push((pEncl));
		}
	}
}

EnclaveTab::~EnclaveTab(){
	for(int i=0; i<ENCLAVE_TABLE_SIZE; i++ ){
		EnclTabNode *node = pTabNode[i];
		while (NULL != node){
			EnclTabNode *n = node->sibling;
			node->sibling=NULL;
			node->deleteNode(node, freeEnclaves);
			node = n;
		}
		pTabNode[i]=NULL;
	}
	if (NULL != enclaveHome)
		free(enclaveHome);
	while (!freeEnclaves.empty()){
		EnclaveControl *pEncl = freeEnclaves.top();
		delete pEncl;
		freeEnclaves.pop();
	}
}

void EnclaveTab::scanTable(uint64_t currentSecond){
	return;
	pthread_mutex_lock(&lock);
	for(int i=0; i<ENCLAVE_TABLE_SIZE; i++ ){
		EnclTabNode *node = pTabNode[i];
		EnclTabNode **pp = &(pTabNode[i]);
		while (NULL != node){
			EnclTabNode *n = node->sibling;
			if ((currentSecond - node->lastActiveSecond)>60*scanCycleTime){
				*pp = n;
				node->deleteNode(node, freeEnclaves);
			} else
				pp = &(node->sibling);
			node = n;
		}
	}
	lastScanSecond = currentSecond;
	pthread_mutex_unlock(&lock);
}
/*
void EnclaveTab::delTabNode(uint32_t stId){
	pthread_mutex_lock(&lock);
	int i = stId % ENCLAVE_TABLE_SIZE;
	EnclTabNode *node = pTabNode[i];
	EnclTabNode **pp = &(pTabNode[i]);
	while (NULL != node){
		EnclTabNode *n = node->sibling;
		if (node->stakeholderId == stId){
			*pp = n;
			node->sibling=NULL;
			delTabNode(node);
			break;
		} else
			pp = &(node->sibling);
		node = n;
	}
	pthread_mutex_unlock(&lock);
}

bool EnclTabNode::findAndDeleteNode(EnclTabNode *node, std::stack<EnclaveControl *> &freeEnclaves){
	if (NULL != sibling){
		if (node == sibling){
			sibling = node->sibling;
			node->sibling = NULL;
			deleteNode(node, freeEnclaves);
			return true;
		} else {
			if (sibling->findAndDeleteNode(node, freeEnclaves))
				return true;
		}
	}
	if (NULL != child){
		if (node == child){
			child = NULL;
			deleteNode(node, freeEnclaves);
			return true;
		} else
			return(child->findAndDeleteNode(node, freeEnclaves));
	}
	return false;
}

void EnclaveTab::delTabNode(EnclTabNode *node){
	if (NULL == node)
		return;
	uint32_t stId = node->stakeholderId;
	int idx = stId%ENCLAVE_TABLE_SIZE;
	EnclTabNode *tabNode = pTabNode[idx];
	if (NULL == tabNode)
		return;
	EnclTabNode *tabNode1 = tabNode;
	while (NULL != tabNode){
		if (tabNode->stakeholderId == stId)
			break;
		EnclTabNode *tabNode1 = tabNode;
		tabNode = tabNode->sibling;
	}
	if (node == tabNode){//node is the root of the tree with same stakeholder IDs
		if (tabNode1 == tabNode){ //node is the first one in the table
			pTabNode[idx] = tabNode->sibling;
		} else
			tabNode1->sibling = tabNode->sibling;
		node->sibling=NULL;
		node->deleteNode(node, freeEnclaves);
	} else
		tabNode->findAndDeleteNode(node, freeEnclaves);
}

*/

EnclTabNode *EnclTabNode::getNodeByEID(sgx_enclave_id_t eid){
	if (NULL != pEncl){
		if (eid == pEncl->getEnclaveId())
			return this;
	}
	EnclTabNode *p;
	if (NULL != sibling){
		p = sibling->getNodeByEID(eid);
		if (NULL != p)
			return p;
	}
	if (NULL != child){
		p = child->getNodeByEID(eid);
		if (NULL != p)
			return p;
	}
	return NULL;
}

void EnclTabNode::deleteNode(EnclTabNode *node, std::stack<EnclaveControl *> &freeEnclaves){
	if (NULL != node->sibling)
		deleteNode(node->sibling, freeEnclaves);
	if (NULL != node->child)
		deleteNode(node->child, freeEnclaves);
	if (NULL != node->pEncl){
		if (freeEnclaves.size()<2*SPARE_ENCLAVES){
			node->pEncl->reset_enclave();
			freeEnclaves.push(node->pEncl);
		} else
			delete node->pEncl;
	}
	delete node;
}

EnclTabNode *EnclaveTab::getNode(uint32_t stId){
	int idx = stId%ENCLAVE_TABLE_SIZE;
	EnclTabNode *tabNode = pTabNode[idx];
	while (NULL != tabNode){
		if (tabNode->stakeholderId == stId)
			break;
		tabNode = tabNode->sibling;
	}
	return tabNode;
}

EnclTabNode *EnclaveTab::getNodeLock(uint32_t stId){
	int idx = stId%ENCLAVE_TABLE_SIZE;
	pthread_mutex_lock(&lock);
	EnclTabNode *tabNode = pTabNode[idx];
	while (NULL != tabNode){
		if (tabNode->stakeholderId == stId)
			break;
		tabNode = tabNode->sibling;
	}
	pthread_mutex_unlock(&lock);
	return tabNode;
}

EnclaveControl *EnclaveTab::createEnclave(){
	EnclaveControl *pEncl=NULL;
	pthread_mutex_lock(&stackLock);
	if (!freeEnclaves.empty()){
		pEncl = freeEnclaves.top();
		freeEnclaves.pop();
	}
	if (NULL == pEncl){
		sgx_status_t ret=(sgx_status_t)-1;
		pEncl = new EnclaveControl();
		if (NULL != pEncl)
			ret = pEncl->create_enclave(enclaveHome);
		if (SGX_SUCCESS != ret){
			delete pEncl;
			pEncl = NULL;
		}
	}
	pthread_mutex_unlock(&stackLock);
	return pEncl;
}

EnclTabNode *EnclaveTab::newEnclTabNode(uint32_t stId, const uint8_t *message, size_t msgLen, uint8_t *iv,
		uint8_t *respBuff, size_t respBuffSize, size_t *respSize){
	pthread_mutex_lock(&lock);
	EnclTabNode *tabNode = getNode(stId);
	if (NULL == tabNode){ //if not exist, create a new one.
		tabNode = new EnclTabNode(stId, currentSecond());
		if (NULL != tabNode){
			uint32_t idx = stId%ENCLAVE_TABLE_SIZE;
			tabNode->sibling=enclaveTab->pTabNode[idx];
			enclaveTab->pTabNode[idx] = tabNode;
			EnclaveControl *pEncl = createEnclave();
			if (NULL != pEncl && SGX_SUCCESS == pEncl->init_enclave(stId, message, msgLen, iv, respBuff, respBuffSize, respSize))
				tabNode->pEncl = pEncl;
			else {
				enclaveTab->pTabNode[idx] = enclaveTab->pTabNode[idx]->sibling;
				free(tabNode);
				tabNode=NULL;
				if (NULL != pEncl)
					delete pEncl;
			}
		} else
			*respSize = 0;
	} else
		*respSize = 0;
	pthread_mutex_unlock(&lock);
	return tabNode;
}

sgx_status_t EnclaveTab::newSession(const char *ethPubkey, uint32_t *session_id, const char *message,
		void *resultVecPtr, uint8_t *iv){
	if (NULL == enclaveTab || NULL == enclaveTab->pTabNode[HOST_STAKEHOLDER_ID] ||
			NULL == enclaveTab->pTabNode[HOST_STAKEHOLDER_ID]->pEncl)
		return (sgx_status_t)-1;
	EnclaveControl *pEncl = enclaveTab->pTabNode[HOST_STAKEHOLDER_ID]->pEncl;
	return(pEncl->new_session(session_id, message, resultVecPtr, iv));
}

sgx_status_t EnclaveTab::session(sgx_status_t *retval, uint32_t session_id, const uint8_t *message, size_t mLen,
		void *resultVecPtr, uint8_t *iv){
	uint32_t stId = session_id >> 16;
	EnclTabNode *tabNode = getNode(stId);
	if (NULL == tabNode || NULL == tabNode->pEncl)
		return (sgx_status_t)-1;
	return(tabNode->pEncl->session(retval, session_id, message, mLen, resultVecPtr, iv));

}

