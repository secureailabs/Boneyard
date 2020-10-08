/*
 * dir_cache.cpp
 *
 *  Created on: Oct 23, 2019
 *      Author: accuser
 */

//#include "sgx_trts.h"
#include "dir_management.h"
#include "dir_cache.h"
#include "enclave_def.h"

char dirName[MAX_PATH]={'\0'};
static bool enabled;
#define BLOCK_FOR_DISABLE DIR_CACHE_SIZE-1
DirBlock_t blockCache[DIR_CACHE_SIZE];
uint8_t useCnt[DIR_CACHE_SIZE];
uint32_t blockNos[DIR_CACHE_SIZE];

void initDirCache(const char *dir, bool force){
	enabled = true;
	if (NULL == dir){
		enabled = false;
		blockNos[BLOCK_FOR_DISABLE] = -1;
	}
	else if (strncmp(dirName, dir, MAX_PATH) || force){
		for (int i=0; i<DIR_CACHE_SIZE; i++){
			blockNos[i]=(uint32_t)-1;
			useCnt[i] = 0;
		}
		strncpy(dirName, dir, MAX_PATH);
	}
}

sgx_status_t getDirCache(DirBlock_t **ppDirBlock, uint32_t blockNo){
	if (!enabled){
		*ppDirBlock = &(blockCache[BLOCK_FOR_DISABLE]);
		return (sgx_status_t)-1;
	}
	for (int i=0; i<DIR_CACHE_SIZE; i++){
		if (blockNos[i] == blockNo){
			*ppDirBlock = &(blockCache[i]);
			useCnt[i]++;
			if (useCnt[i]==255)
				useCnt[i]=254;
			return SGX_SUCCESS;
		}
	}

	//No existing cache entry for blockNo
	uint32_t lessUseCache=0;
	uint8_t cnt=255;
	for (int i=0; i<DIR_CACHE_SIZE; i++){
		if (useCnt[i]<cnt){
			lessUseCache = i;
			cnt = useCnt[i];
			if (0 == cnt)
				break;
		}
	}
	if (cnt>0){ //aging use counter by 1 when there is not idle cache
		for (int i=0; i<DIR_CACHE_SIZE; i++){
			if (useCnt[i]>0)
				useCnt[i]-=cnt;
		}
	}
	blockNos[lessUseCache]=blockNo;
	useCnt[lessUseCache]=1;
	*ppDirBlock = &(blockCache[lessUseCache]);
	return (sgx_status_t)-1;
}

