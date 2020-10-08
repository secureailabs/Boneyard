#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include "sgx_urts.h"
#include "sgx_tcrypto.h"
#include "ecalls.h"
#include "task_manager_app.h"


sgx_status_t task_manager_kill_thread(pthread_t tid){
	int ret = pthread_cancel(tid);
	if(0 != ret){
		return (sgx_status_t) -1;
	}
	return SGX_SUCCESS;
}

void *task_manager_make_ecall(void *pIndex){
	sgx_status_t retval;
	uint32_t *pIdx = (uint32_t *)pIndex;
	uint32_t idx = *pIdx;
	uint32_t stId = *(uint32_t*)(pIdx+1);
	pthread_t tid_app = pthread_self();
	sgx_status_t ret = call_ec_task_manager(&retval, tid_app, idx, stId);
	free(pIndex);
	if(ret == SGX_SUCCESS){
		return NULL;//(void*)&retval;
	}
	return NULL;//(void*)&ret;
}

/**
 * Creates new threads outside the enclave
 * pIndex : Pointer to the index of the tasktable
 */
sgx_status_t task_manager_new_threads(uint32_t index, uint32_t stId){
	pthread_t tid;
	uint32_t *idx = (uint32_t*)malloc(2*sizeof(uint32_t));
	*idx = index;
	*(idx+1) = stId;
	if(pthread_create(&tid, NULL, task_manager_make_ecall, (void*) idx) != 0){
		return (sgx_status_t) -1;
	}
	return SGX_SUCCESS;
}

sgx_status_t task_manager_app(uint32_t index,uint64_t tid, uint32_t stId){
	if(tid == 0){
		if(SGX_SUCCESS != task_manager_new_threads(index, stId)){
			return (sgx_status_t) -1;
		}
	} else if (tid != 0){
		if (SGX_SUCCESS != task_manager_kill_thread(tid)){
			return (sgx_status_t) -1;
		}
	} else {
		return (sgx_status_t) -1;
	}
	return SGX_SUCCESS;
}


