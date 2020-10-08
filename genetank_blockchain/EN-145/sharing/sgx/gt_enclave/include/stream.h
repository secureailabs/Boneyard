/*
 * stream.h
 *
 *  Created on: Oct 3, 2019
 *      Author: accuser
 */

#ifndef GT_ENCLAVE_INCLUDE_STREAM_H_
#define GT_ENCLAVE_INCLUDE_STREAM_H_

#include "../gt_enclave_t.h"
#include "./cjson.h"
#include "sgx_tprotected_fs.h"
//#include "sgx_tae_service.h"
#include "sgx_thread.h"
#include "./session.h"

#define MAX_STREAM 128
#define SCAN_INTERVAL 5*60*1000
#define PAGE_RECEIVED 1
#define PAGE_RECEIVING 2

#define DEFAULT_PAGE_SIZE (100*1024)
#define DEFAULT_WIND_BUFF_SIZE 1024*1024

class PaginationStream{
public:
	PaginationStream(){
		id = 0;
		pageSize=0;
		windSize=0;
		buff=NULL;
		windFirstPage=0;
		pageTags=NULL;
		fp = NULL;
		recentActive=0;
		totalSize=0;
		closeFunc = NULL;
		totalPages=0;
	}
	PaginationStream(uint32_t pgSize, uint32_t wSize, size_t totalSize){
		id=0;
		pageSize=pgSize;
		windSize=wSize;
		buff=NULL;
		windFirstPage=0;
		pageTags=NULL;
		fp = NULL;
		recentActive=0;
		this->totalSize = totalSize;
		totalPages = totalSize/pageSize;
		if (totalSize%pageSize)
			totalPages++;
		closeFunc = NULL;
	}
	~PaginationStream(){
		if (NULL != buff)
			free(buff);
		if (NULL != pageTags)
			free(pageTags);
		if (NULL != fp)
			sgx_fclose(fp);
	}

	sgx_status_t initStream(SGX_FILE *fp, void (*func)()); //including allocation of buffers
	uint8_t *getPageBuff(uint32_t pageNo);
	sgx_status_t receivePage(uint32_t pageNo, uint32_t dataSize, uint64_t tm);
	//sgx_status_t storePages();
	sgx_status_t getPageStatus(uint32_t *firstPage, uint8_t *pgTags, uint32_t tagSize);
	uint8_t getPageTag(uint32_t buffOffset);
	void setPageTag(uint8_t tag, uint32_t buffOffset);
	uint32_t getPageSize(){
		return pageSize;
	}
	void setId(uint32_t id){
		this->id = id;
	}
	uint32_t getFirstPage(){
		return windFirstPage;
	}

	void updateTimestamp(uint64_t tm);
	uint64_t getTimestamp();
	size_t genStartStreamRspJSON(char *rspBuf, size_t maxLen);

private:
	uint32_t id;
	uint32_t pageSize;
	uint32_t windSize; //number of pages
	uint8_t *buff;
	uint32_t windFirstPage;
	uint8_t *pageTags;
	size_t totalSize;
	uint32_t totalPages;
	SGX_FILE *fp;
	uint64_t recentActive;
	void (*closeFunc)();
};

class StreamTable{
public:
	sgx_thread_mutex_t streamLock;

	StreamTable(){
		double tm;
		sgx_status_t ret, retv;
		for (int i=0; i<MAX_STREAM; i++)
			streamTable[i]=NULL;
		ret=ocall_gettime(&retv, &tm);
		if (SGX_SUCCESS == ret && SGX_SUCCESS == retv)
			recentScan = tm;
		streamLock = SGX_THREAD_MUTEX_INITIALIZER;
	}

	uint32_t getFreeStreamId(PaginationStream *stream);
	PaginationStream *getStreamById(uint32_t psId){
		if (psId>MAX_STREAM)
			return NULL;
		return streamTable[psId];
	}
	//static sgx_status_t setStream(PaginationStream *stream);
	void freeStream(uint32_t psId);
	void scanStream(uint64_t currentTm);
private:
	PaginationStream *streamTable[MAX_STREAM];
	uint64_t recentScan;
};

extern StreamTable pageStreams;

PaginationStream *newDataStream(uint32_t stId, cJSON *startStreamJson, const char *dirType,	const char *dataName, const char *dataInfo);


#endif /* GT_ENCLAVE_INCLUDE_STREAM_H_ */
