/*
 * data_share.cpp
 *
 *  Created on: Sep 26, 2019
 *      Author: accuser
 */
#include "data_share.h"
#include "data_management.h"
#include "dir_management.h"
#include "conversion.h"
#include "crypto.h"
#include "gt_enclave_t.h"
//#include "sgx_tae_service.h"

bool validShareInfo(char *infoFileName, uint32_t encKeyId, uint32_t destStId, uint64_t tm){
	return true;
	StakeholderRec_t sRec;
	sRec.seqNumber = encKeyId;
	sgx_status_t ret = dmReadStake(&sRec);
	if (SGX_SUCCESS != ret) {
		return false;
	}
	SGX_FILE *file = sgx_fopen(infoFileName, "r", &(sRec.encKey));//file exist?
	if (NULL == file)
		return false;
	ShareInfoFileHeader_t header;
	if (1 != sgx_fread(&header, sizeof(ShareInfoFileHeader_t), 1, file)){
		sgx_fclose(file);
		return NULL;
	}
	if (DATA_SHARE_INFO_VERSION != header.version){ //wrong file version number
		sgx_fclose(file);
		return false;
	}
	ShareInfoRec_t sInfoRec;
	int i=header.shares;
	bool found = false;
	for (i=0; i< header.shares; i++){
		if (1 != sgx_fread(&sInfoRec, sizeof(ShareInfoRec_t), 1, file)){
			sgx_fclose(file);
			return false;
		}
		if (destStId == sInfoRec.destStId){
			found = true;
			break;
		}
	}
	sgx_fclose(file);
	if (!found)
		return false;
	if (tm != sInfoRec.tm)
		return false;
	uint64_t curTime;
	//sgx_time_source_nonce_t nonce;
	//if (SGX_SUCCESS == sgx_get_trusted_time(&curTime, &nonce)){
//		if (curTime>sInfoRec.startTime && curTime<sInfoRec.endTime)
			return true;
	//}
	return false;
}

uint8_t *readShareDataInfo(char *infoFileName, sgx_key_128bit_t *encKey, uint32_t symLinkStId, size_t *pLen){
	SGX_FILE *file = sgx_fopen(infoFileName, "r", encKey);//file exist?
	if (NULL == file)
		return NULL;
	ShareInfoFileHeader_t header;
	if (1 != sgx_fread(&header, sizeof(ShareInfoFileHeader_t), 1, file)){
		sgx_fclose(file);
		return NULL;
	}
	if (DATA_SHARE_INFO_VERSION != header.version){ //wrong file version number
		sgx_fclose(file);
		return NULL;
	}
	ShareInfoRec_t sInfoRec;
	ShareInfoRec_t *pInfoRec=NULL;
	int i=header.shares;
	bool found = false;
	char *shareInfo;
	if (0 != symLinkStId){
		for (i=0; i< header.shares; i++){
			if (1 != sgx_fread(&sInfoRec, sizeof(ShareInfoRec_t), 1, file)){
				sgx_fclose(file);
				return NULL;
			}
			if (symLinkStId == sInfoRec.destStId){
				found = true;
				break;
			}
		}
	} else {
		pInfoRec = (ShareInfoRec_t *)malloc(header.shares*sizeof(ShareInfoRec_t));
		if (NULL == pInfoRec){
			sgx_fclose(file);
			return NULL;
		}
		if (header.shares != sgx_fread(pInfoRec, sizeof(ShareInfoRec_t), header.shares, file)){
			sgx_fclose(file);
			return NULL;
		}
	}
	if (SGX_SUCCESS != sgx_fseek(file,0,SEEK_END)){
		sgx_fclose(file);
		return NULL;
	}
	size_t dataInfoLen = sgx_ftell(file) - sizeof(ShareInfoFileHeader_t)-header.shares*sizeof(ShareInfoRec_t);
	size_t infoLen;
	if (0 != symLinkStId)
		infoLen = 128;
	else
		infoLen = header.shares*256+128;
	uint8_t *buff = (uint8_t *)malloc(dataInfoLen + infoLen);
	if (NULL == buff){
		if (NULL != pInfoRec)
			free(pInfoRec);
		sgx_fclose(file);
		return NULL;
	}
	sgx_fseek(file, sizeof(ShareInfoFileHeader_t)+header.shares*sizeof(ShareInfoRec_t), SEEK_SET);
	if (1 != sgx_fread(buff, dataInfoLen, 1, file)){
		free(buff);
		if (NULL != pInfoRec)
			free(pInfoRec);
		sgx_fclose(file);
		return NULL;
	}
	uint8_t *ptr = buff+dataInfoLen-1;
	while (*ptr != '}' && ptr > buff)
		ptr--;
	if (ptr>buff){
		*ptr++ = ',';
	} else {
		free(buff);
		if (NULL != pInfoRec)
			free(pInfoRec);
		sgx_fclose(file);
		return NULL;
	}
	size_t curLen=0;
	if (0 != symLinkStId){
		snprintf((char *)(ptr), 128, "\"startTime\":%lu, \"endTime\":%lu}", sInfoRec.startTime, sInfoRec.endTime);
		curLen += strnlen((char *)(ptr+curLen), 255);
	} else {
		snprintf((char *)ptr, 128, "\"shareInfo\":[");
		curLen = strnlen((char *)ptr, 128);
		eth_addr_t ethAddr;
		char addrHex[41];
		addrHex[40]='\0';
		for (int i=0; i<header.shares; i++){
			if (SGX_SUCCESS != dmReadEthAddr((pInfoRec+i)->destStId, &ethAddr))
				break;
			bin2hex((uint8_t *)&ethAddr, 20, addrHex);
			snprintf((char *)(ptr+curLen), 255, "{\"ethAddr\":\"%s\",\"startTime\":%lu,\"endTime\":%lu},",
					addrHex, (pInfoRec+i)->startTime, (pInfoRec+i)->endTime);
			curLen += strnlen((char *)(ptr+curLen), 255);
		}
		if (*(ptr+curLen-1) == ',')
			curLen--;
		snprintf((char *)(ptr+curLen), 128, "]}");
		curLen += 2;
	}
	*pLen = dataInfoLen + curLen;
	if (NULL != pInfoRec)
		free(pInfoRec);
	sgx_fclose(file);
	return buff;
}

sgx_status_t updateShareInfo(char *srcDirName, uint32_t srcFileId, uint32_t srcKeyId, ShareInfoRec_t *pInfoRec, char *dataInfo){
	sgx_status_t ret=SGX_SUCCESS;
	StakeholderRec_t sRec;
	sRec.seqNumber = srcKeyId;
	ret = dmReadStake(&sRec);
	if (SGX_SUCCESS != ret) {
		return ret;
	}
	char fullFileName[MAX_PATH];
	//getFileName(srcDirName, srcFileId, prefix, MAX_PATH);
	getInfoFileNameOnDisk(fullFileName, MAX_PATH, srcDirName, srcFileId);
	//snprintf(fullFileName, MAX_PATH, "%s/%s/%s.%d.info", enclaveHome, ENCLACE_FILE_DIR, srcDirName, srcFileId);
	//uint32_t nameLen = strnlen(prefix, MAX_PATH);
	//if ((nameLen+10)>=MAX_PATH)
	//	return (sgx_status_t)-1;
	ShareInfoFileHeader_t header;
	header.version = DATA_SHARE_INFO_VERSION;
	header.shares = 0;
	//snprintf(prefix+nameLen, 10, ".info");
	SGX_FILE *file = sgx_fopen(fullFileName, "r", &(sRec.encKey));//file exist?
	if (NULL == file){//file not exist
		file = sgx_fopen(fullFileName, "w", &(sRec.encKey)); //create a new file
		if (1 != sgx_fwrite(&header, sizeof(ShareInfoFileHeader_t), 1, file)){
			sgx_fclose(file);
			return (sgx_status_t)-1;
		}
	}
	sgx_fclose(file);
	file = sgx_fopen(fullFileName, "r+", &(sRec.encKey));
	if (NULL == file){
		return (sgx_status_t)-1;
	}
	if (1 != sgx_fread(&header, sizeof(ShareInfoFileHeader_t), 1, file)){
		sgx_fclose(file);
		return (sgx_status_t)-1;
	} else {
		if (DATA_SHARE_INFO_VERSION != header.version){
			sgx_fclose(file);
			return (sgx_status_t)-1;
		}
	}
	if (NULL == pInfoRec){
		sgx_fseek(file, sizeof(ShareInfoFileHeader_t)+header.shares*sizeof(ShareInfoRec_t), SEEK_SET);
		if (1 != sgx_fwrite(dataInfo, strnlen(dataInfo, 1024*1024), 1, file)){
			ret = (sgx_status_t)-1;
		}
		sgx_fclose(file);
		return ret;
	}
	ShareInfoRec_t sInfoRec;
	int cnt;
	for (cnt=0; cnt<header.shares; cnt++){
		if (1 != sgx_fread(&sInfoRec, sizeof(ShareInfoRec_t), 1, file)){
			sgx_fclose(file);
			return (sgx_status_t)-1;
		}
		if (sInfoRec.destStId == pInfoRec->destStId){
			sgx_fseek(file, sizeof(ShareInfoFileHeader_t)+cnt*sizeof(ShareInfoRec_t), SEEK_SET);
			if (1 != sgx_fwrite(pInfoRec, sizeof(ShareInfoRec_t), 1, file)){
				sgx_fclose(file);
				return (sgx_status_t)-1;
			}
			break;
		}
	}
	if (cnt<header.shares && NULL == dataInfo){//new record updated and no new share info
		sgx_fclose(file);
		return SGX_SUCCESS;
	}
	char *buff=NULL;
	uint64_t bufLen;
	if (NULL == dataInfo){ //insert a share record, backup the data info
		if (SGX_SUCCESS != sgx_fseek(file,0,SEEK_END)){
			sgx_fclose(file);
			return (sgx_status_t)(-1);
		}
		bufLen = sgx_ftell(file) - sizeof(ShareInfoFileHeader_t)-header.shares*sizeof(ShareInfoRec_t);
		buff = (char *)malloc(bufLen);
		if (NULL == buff){
			sgx_fclose(file);
			return (sgx_status_t)(-1);
		}
		sgx_fseek(file, sizeof(ShareInfoFileHeader_t)+header.shares*sizeof(ShareInfoRec_t), SEEK_SET);
		if (1 != sgx_fread(buff, bufLen,1,file)){
			free(buff);
			sgx_fclose(file);
			return (sgx_status_t)(-1);
		}
	}
	if (cnt==header.shares){
		sgx_fseek(file, 0, SEEK_SET);
		header.shares++;
		if (1 != sgx_fwrite(&header, sizeof(ShareInfoFileHeader_t), 1, file)){
			free(buff);
			sgx_fclose(file);
			return (sgx_status_t)-1;
		}
		sgx_fseek(file, sizeof(ShareInfoFileHeader_t)+(header.shares-1)*sizeof(ShareInfoRec_t), SEEK_SET);
		if (1 != sgx_fwrite(pInfoRec, sizeof(ShareInfoRec_t), 1, file)){
			free(buff);
			sgx_fclose(file);
			return (sgx_status_t)-1;
		}
	}
	if (NULL != buff){
		dataInfo = buff;
	} else
		bufLen = strnlen(dataInfo, 1024*1024);
	sgx_fseek(file, sizeof(ShareInfoFileHeader_t)+header.shares*sizeof(ShareInfoRec_t), SEEK_SET);
	if (1 != sgx_fwrite(dataInfo, bufLen, 1, file)){
		if (NULL != buff)
			free(buff);
		sgx_fclose(file);
		return (sgx_status_t)-1;
	}
	if (NULL != buff)
		free(buff);
	sgx_fclose(file);
	return SGX_SUCCESS;
}

sgx_status_t saveDataInfo(char *srcPath, char *dataName, char *dataInfo){
	sgx_status_t ret = SGX_SUCCESS;
	DirRec_t dirRec;

	char fileName[MAX_PATH];
	snprintf(fileName, MAX_PATH, "%s/%s", srcPath, dataName);
	removeDoubleSlash(fileName);

	char srcDirName[]="data1.dir";
	//snprintf(srcDirName,MAX_PATH-1,"data%d.dir", 1);
    uint32_t srcKeyId, stId;
    int32_t srcFileId;
	ret = dmFindDirEntry(srcDirName, fileName, &srcKeyId, &srcFileId, &dirRec, &stId);
    if (SGX_SUCCESS != ret || -1 == srcFileId) {
            ret = (sgx_status_t)(-1);
            return ret;
    }
	if (SGX_SUCCESS == ret){
		ret = updateShareInfo(srcDirName, srcFileId, srcKeyId, NULL, dataInfo);
	}
	return ret;
}

sgx_status_t shareData2Guest(char *srcPath, char *dstPath,	 char *dataName, char *guestPubKey,
		uint64_t startTime, uint64_t endTime, char *dataInfo, bool isAnonymized){
	sgx_status_t ret = SGX_SUCCESS;
	DirRec_t dirRec;

	char fileName[MAX_PATH];
	if (isAnonymized)
		snprintf(fileName, MAX_PATH, "/anonymized/%s/%s", srcPath, dataName);
	else
		snprintf(fileName, MAX_PATH, "%s/%s", srcPath, dataName);
	removeDoubleSlash(fileName);

	char srcDirName[]="data1.dir";
	//snprintf(srcDirName,MAX_PATH-1,"data%d.dir", 1);
    uint32_t srcKeyId, stId;
    int32_t srcFileId;
	ret = dmFindDirEntry(srcDirName, fileName, &srcKeyId, &srcFileId, &dirRec, &stId);
    if (SGX_SUCCESS != ret || -1 == srcFileId) {
            ret = (sgx_status_t)(-1);
            return ret;
    }

	uint8_t ethAddr[20];
	sgx_ec256_public_t pubKey;
	hex2bin(guestPubKey, (uint8_t *)&pubKey, 64);
	ret = pubKey2EthAddr((uint8_t *)&pubKey, ethAddr);
	uint32_t role;
	uint32_t dstStId=-1;
	ret = dmSearchRole(ethAddr, &dstStId);
	if (SGX_SUCCESS != ret || dstStId < 1){
		ret = (sgx_status_t)-1;
		return ret;
	}

	char dstDir[MAX_PATH];
	snprintf(dstDir,MAX_PATH-1,"data%d.dir",dstStId);
	char symName[MAX_PATH];
	if (isAnonymized)
		snprintf(symName, MAX_PATH-1, "/anonymized/%s/%s", dstPath, dataName);
	else
		snprintf(symName, MAX_PATH-1, "/sharedin/%s/%s", dstPath, dataName);
	removeDoubleSlash(symName);

	double tm;
	sgx_status_t retv;
	ocall_gettime(&retv, &tm);
	SymLinkInfo_t symInfo;
	symInfo.blockNumber = srcFileId/BLOCK_DIR_NUMBER;
	symInfo.fileId = srcFileId%BLOCK_DIR_NUMBER;
	symInfo.stId = 1;
	symInfo.srcKeyId = srcKeyId;
	symInfo.tm = tm;
	int32_t dstFileId;
	uint32_t dstKeyId;
    ret = dmAddDir(dstDir, symName, &symInfo, &dstKeyId, &dstFileId);
    if (SGX_SUCCESS != ret || -1 == dstFileId) {
            ret = (sgx_status_t)(-1);
            return ret;
    }

    ShareInfoRec_t infoRec;
    infoRec.destStId = dstStId;
    infoRec.startTime = startTime;
    infoRec.endTime = endTime;
    infoRec.tm = symInfo.tm;

	if (SGX_SUCCESS == ret){
		ret = updateShareInfo(srcDirName, srcFileId, srcKeyId, &infoRec, dataInfo);
	}

	return ret;
}

