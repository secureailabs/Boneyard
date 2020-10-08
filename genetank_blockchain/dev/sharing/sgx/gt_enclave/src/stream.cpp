/*
 * stream.cpp
 *
 *  Created on: Oct 3, 2019
 *      Author: accuser
 */
#include "gt_enclave_t.h"
#include "cjson.h"
#include "stream.h"
#include "sgx_tprotected_fs.h"
#include "sgx_thread.h"
//#include "sgx_tae_service.h"
#include "util.h"
#include "enclave_def.h"
#include "session.h"
#include "string.h"
#include "dir_management.h"
#include "conversion.h"

StreamTable pageStreams;

//sgx_thread_mutex_t PaginationStream::streamLock = SGX_THREAD_MUTEX_INITIALIZER;

void StreamTable::freeStream(uint32_t psId){
	//LOCK(&PaginationStream::streamLock);
	if (NULL !=streamTable[psId]){
		delete streamTable[psId];
		streamTable[psId] = NULL;
	}
	//UNLOCK(&PaginationStream::streamLock);
}


uint32_t StreamTable::getFreeStreamId(PaginationStream *stream){
	uint32_t i=0;
	//LOCK(&PaginationStream::streamLock);
	for(i=0;i<MAX_STREAM;i++){
		if (NULL == streamTable[i]){
			streamTable[i] = stream;
			stream->setId(i);
			break;
		}
	}
	//UNLOCK(&PaginationStream::streamLock);
	return i;
}

void StreamTable::scanStream(uint64_t currentTm){
	if (currentTm - recentScan > SCAN_INTERVAL){
		//LOCK(&PaginationStream::streamLock);
		for (int i=0;i<MAX_STREAM;i++){
			if (NULL != streamTable[i]){
				uint64_t tm = streamTable[i]->getTimestamp();
				if (currentTm - tm > 2*SCAN_INTERVAL)
					freeStream(i);
			}
		}
		//UNLOCK(&PaginationStream::streamLock);
	}
}

void PaginationStream::updateTimestamp(uint64_t currentTm){
	recentActive = currentTm;
}

uint64_t PaginationStream::getTimestamp(){
	return recentActive;
}

sgx_status_t PaginationStream::initStream(SGX_FILE *fp, void (*func)()){//including allocation of buffers
	buff = (uint8_t *)malloc(pageSize*windSize);
	if (NULL == buff)
		return (sgx_status_t)-1;
	int tagLen = windSize/2+1;
	pageTags = (uint8_t *)malloc(tagLen);
	for (int i=0; i<tagLen; i++)
		pageTags[i]=0;
	if (NULL == pageTags){
		free(buff);
		buff = NULL;
		return (sgx_status_t)-1;
	}
	this->fp = fp;
	this->closeFunc = func;
	return SGX_SUCCESS;
}

uint8_t PaginationStream::getPageTag(uint32_t buffOffset){
	if (NULL == pageTags)
		return 0xff;
	uint8_t pageTag = pageTags[buffOffset/2];
	if (buffOffset & 1)
		pageTag >> 4;
	return (pageTag && 0x0f);
}

void PaginationStream::setPageTag(uint8_t tag, uint32_t buffOffset){
	if (NULL == pageTags)
		return;
	uint8_t pageTag = pageTags[buffOffset/2];
	if (buffOffset & 1){
		tag = tag << 4;
		pageTag &= 0x0f;
	} else {
		pageTag &= 0xf0;
		tag &= 0x0f;
	}
	pageTags[buffOffset/2] = pageTag | tag;
}

uint8_t *PaginationStream::getPageBuff(uint32_t pageNo){
	uint8_t *ptr=NULL;
	LOCK(&pageStreams.streamLock);
	if (pageNo>=windFirstPage && pageNo<windFirstPage+windSize){
		uint32_t buffOffset = pageNo%windSize;
		uint8_t pageTag= getPageTag(buffOffset);
		if (!(pageTag & (PAGE_RECEIVED || PAGE_RECEIVING))){
			setPageTag(PAGE_RECEIVING, buffOffset);
			if (NULL != buff)
				ptr = buff+buffOffset*pageSize;
		}// else if (pageTag & PAGE_RECEIVED)
		//	receivePage(pageNo, recentActive);
	}
	UNLOCK(&pageStreams.streamLock);
	return ptr;
}

sgx_status_t PaginationStream::receivePage(uint32_t pageNo, uint32_t dataSize, uint64_t tm){
	sgx_status_t ret=(sgx_status_t)-1;
	if (pageNo>=totalPages)
		return ret;
	if (pageNo<totalPages-1)
		if (dataSize != pageSize)
			return ret;
	LOCK(&pageStreams.streamLock);
	if (pageNo>=windFirstPage && pageNo<windFirstPage+windSize){
		uint32_t buffOffset = pageNo%windSize;
		setPageTag(PAGE_RECEIVED, buffOffset);
		recentActive = tm;
		if (NULL != fp){
			uint32_t lastPage = windFirstPage+windSize;
			if (lastPage>totalPages)
				lastPage = totalPages;
			dataSize=pageSize;
			for (uint32_t i=windFirstPage; i<lastPage; i++){
				uint32_t offset = i%windSize;
				uint8_t tag = getPageTag(offset);
				if (tag & PAGE_RECEIVED){
					if (i==totalPages-1)
						dataSize = totalSize%pageSize;
					if (1 == sgx_fwrite(buff+offset*pageSize, dataSize, 1, fp)){
						setPageTag(0, offset);
						windFirstPage++;
					} else
					  break;
				} else
					break;
			}
		}
		ret = SGX_SUCCESS;
	}
	if (windFirstPage >= totalPages){
		if (NULL != fp) {
			sgx_fclose(fp);
			fp = NULL;
		}
		if (NULL != closeFunc)
			closeFunc();
	}
	UNLOCK(&pageStreams.streamLock);
	return ret;
}

sgx_status_t PaginationStream::getPageStatus(uint32_t *firstPage, uint8_t *pgTags, uint32_t tagSize){
	if (NULL == pageTags)
		return (sgx_status_t)-1;
	LOCK(&pageStreams.streamLock);
	*firstPage = windFirstPage;
	int j=0;
	uint8_t mask = 1;
	for (uint32_t i=windFirstPage; i<windFirstPage+windSize; i++){
		uint32_t offset = i%windSize;
		uint8_t pageTag = pageTags[offset/2];
		if (offset&1)
			pageTag >> 4;
		if (!mask){
			j++;
			mask=1;
		}
		if (j<tagSize){
			if (1 == mask)
				pgTags[j] = 0;
			if (pageTag & 1){
				pgTags[j] |= mask;
			}
			mask<<1;
		}
	}
	UNLOCK(&pageStreams.streamLock);
	return SGX_SUCCESS;
}
size_t PaginationStream::genStartStreamRspJSON(char *rspBuf, size_t maxLen){
	size_t curLen=0;
	snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"streamReady\":{\"pageSize\":%d", pageSize);
	curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
	snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"windSize\":%d", windSize);
	curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
	snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"streamId\":%d}", id);
	curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
	return curLen;
}

PaginationStream *newDataStream(uint32_t stId, cJSON *startStreamJson, const char *dirType,
		const char *dataName, const char *dataInfo){
	sgx_status_t ret=(sgx_status_t)-1;
	uint64_t fileSize=0;
	if(SGX_SUCCESS != getJsonNumberDouble(startStreamJson, "totalSize", &fileSize)){
		return NULL;
	}
	uint32_t pgSize;
	if(SGX_SUCCESS != getJsonNumber(startStreamJson, "maxPageSize", &pgSize)){
		return NULL;
	}
	uint32_t wSize;
	//if(SGX_SUCCESS != getJsonNumber(startStreamJson, "maxWindSize", &wSize)){
	//	return (sgx_status_t)-1;
	//}

	pgSize = (pgSize>DEFAULT_PAGE_SIZE)?DEFAULT_PAGE_SIZE:pgSize;
	wSize = (DEFAULT_WIND_BUFF_SIZE)/pgSize;

	PaginationStream *pStream = new PaginationStream(pgSize, wSize, fileSize);
	if (NULL != pStream){
		uint32_t psId = pageStreams.getFreeStreamId(pStream);
		if (psId < MAX_STREAM){
			SGX_FILE *file;
			char dir[MAX_PATH];
			snprintf(dir, MAX_PATH-1, "/%s/%s", dirType, dataName);
			removeDoubleSlash(dir);
			ret = dmOpenFileWithStId(dir, "w", &file, stId);
			if (SGX_SUCCESS == ret && NULL != file){
				ret = pStream->initStream(file, NULL);
			}
			if (NULL != dataInfo){
				snprintf(dir, MAX_PATH-1, "%s%d.dir", dirType, stId);
				dmWriteFileInfo(dir, dataName, dataInfo);
			}
		}
	}
	if (SGX_SUCCESS != ret){
		if (NULL != pStream)
			delete pStream;
		pStream = NULL;
	}
	return pStream;
}
