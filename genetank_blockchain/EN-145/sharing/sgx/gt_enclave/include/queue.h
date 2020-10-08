/*
 * queue.h
 *
 *  Created on: Sep 21, 2019
 *      Author: accuser
 */

#ifndef GT_ENCLAVE_INCLUDE_QUEUE_H_
#define GT_ENCLAVE_INCLUDE_QUEUE_H_

#include <stdint.h>
//#include "string"
#include <stdlib.h>
#include "sgx_utils.h"
#include "sgx_thread.h"
#include "../../gt_enclave/gt_enclave_t.h"

typedef struct _queue_node_t{
	uint8_t *data;
	size_t dataLength;
	uint32_t dataType;
	_queue_node_t *next;
}queue_node_t;

class Queue{
public:
	Queue(){
		header = NULL;
		tail = NULL;
		length = 0;
		task_stopped = false;
		qLock = SGX_THREAD_MUTEX_INITIALIZER;
	}
	sgx_status_t enQueue(uint8_t *data, size_t dataLen, uint32_t dataType){
		queue_node_t *qNode = (queue_node_t *)malloc(sizeof(queue_node_t));
		if (NULL == qNode)
			return (sgx_status_t)-1;
		qNode->data = data;
		qNode->dataLength = dataLen;
		qNode->dataType = dataType;
		qNode->next = NULL;
		sgx_thread_mutex_lock(&qLock);
		if (NULL != tail){
			tail->next = qNode;
		} else {
			header = qNode;
		}
		tail = qNode;
		length++;
		sgx_thread_mutex_unlock(&qLock);
		return SGX_SUCCESS;
	}

	uint8_t *deQueue(size_t *pLen, uint32_t *dataType){
		sgx_status_t retv;
		while (0 == length) {
			if (task_stopped) {
				if (length)
					break;
				*pLen = 0;
				return NULL;
			}
			ocall_sleep(&retv, 1);
		}
		queue_node_t *pNode;
		pNode = header;
		sgx_thread_mutex_lock(&qLock);
		header = header->next;
		if (NULL == header)
			tail = NULL;
		length--;
		sgx_thread_mutex_unlock(&qLock);
		*pLen = pNode->dataLength;
		if (dataType) {
			*dataType = pNode->dataType;
		}
		uint8_t *pData = pNode->data;
		free(pNode);
		return (pData);
	}
	void stopQueue(){
		task_stopped = true;
	}
	void startQueue(){
		task_stopped = false;
	}

private:
	queue_node_t *header;
	queue_node_t *tail;
	size_t length;
	bool task_stopped;
	sgx_thread_mutex_t qLock;
};

extern Queue subResultQueue;
extern Queue subSimFlLSHQueue;

#endif /* GT_ENCLAVE_INCLUDE_QUEUE_H_ */
