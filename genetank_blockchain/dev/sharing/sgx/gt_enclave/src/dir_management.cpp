/*
 * dir_management.cpp
 *
 *  Created on: Feb 5, 2019
 *      Author: accuser
 */
#include "data_management.h"
#include "dir_management.h"
#include "dir_cache.h"
#include "./pyvm/Include/enclave_wrapper/io.h"
#include "gt_enclave.h"
#include "data_share.h"
#include "task_manager_enclave.h"
#include "util.h"
#include "session.h"

static char basePath[MAX_PATH];
static char sandboxPath[MAX_PATH];
static uint32_t sandboxPathSize;
static char modelPath[MAX_PATH];

static void *extFiles[256];

bool isGlobalSandbox(){
	return(!strncmp(sandboxPath, "/data", 10));
}

void dmSetSandbox(const char *sandbox){
	if (NULL == sandbox || '\0' == sandbox[0])
		snprintf(sandboxPath, MAX_PATH, "%s", basePath);
	else
		snprintf(sandboxPath, MAX_PATH, "%s/%s", basePath, sandbox);
	sandboxPathSize = strnlen(sandboxPath, MAX_PATH);
}

/*
 * Input:
 *   sandboxBase: the base path without trailing '/' for the data files, e.g. "/data"
 *   model: e.g. /model/OPAL
 */
void dmInitDirSetting(const char *sandboxBase, const char *model){
	//if ('\0' != sandboxPath[0] || '\0' != modelPath[0])
	//	return (sgx_status_t)-1;
	snprintf(basePath, MAX_PATH, "%s", sandboxBase);
	snprintf(sandboxPath, MAX_PATH, "%s", sandboxBase);
	sandboxPathSize = strnlen(sandboxPath, MAX_PATH);
	size_t len=(NULL == model)?0:(strnlen(model, MAX_PATH-1)+1);
	strncpy(modelPath, model, len);
}

void *addExtFile(void *fp){
	for (int i=0; i<256; i++){
		if (NULL == extFiles[i]){
			extFiles[i] = fp;
			return(extFiles+i);
		}
	}
	return NULL; //add external file failed
}

void delExtFile(void *fp){
	if (isExtFile(fp)){
		fp = NULL;
	}
}

int isReaderLockAcquired = 0;
sgx_thread_mutex_t lockStatus = SGX_THREAD_MUTEX_INITIALIZER;
sgx_thread_mutex_t writerLockDir = SGX_THREAD_MUTEX_INITIALIZER;
sgx_thread_mutex_t readerLockDir = SGX_THREAD_MUTEX_INITIALIZER;
sgx_thread_mutex_t fileRWLockDir = SGX_THREAD_MUTEX_INITIALIZER;

sgx_status_t dmInitDataDir(int stId){
	sgx_status_t ret = SGX_SUCCESS;
	char dirFileName[MAX_PATH];
	char fullDir[MAX_PATH];
	StakeholderRec_t sRec;

  sRec.seqNumber = 0;
  ret = dmReadStake(&sRec);

  if (SGX_SUCCESS != ret || sRec.isDeleted)
  	return (sgx_status_t)(-1);

  snprintf(dirFileName,MAX_PATH,"data%d.dir", stId);

	//snprintf(fullDir,MAX_PATH,"%s/%s",enclaveHome, dirFileName);
	getFullPathFileName(fullDir, MAX_PATH, dirFileName);

	SGX_FILE* file = sgx_fopen(fullDir, "r", &(sRec.encKey));
	if (NULL == file){
	    ret = initDir(dirFileName, &(sRec.encKey));
	    if (SGX_SUCCESS != ret)
	    	return (sgx_status_t)(-1);
	    file = sgx_fopen(fullDir, "r+", &(sRec.encKey));
	    if (NULL == file)
	    	return (sgx_status_t)(-1);
	} else {
		sgx_fclose(file);
		return SGX_SUCCESS;
	}
/*
	DirBlock_t sDirBlock;
	DirRec_t dirRec;
	strncpy((char *)(dirRec.name), "sharedin", 10);
    SET_SUBDIR_ATTR(dirRec.attr);
    SET_SHARED(dirRec.attr); //to be prevent accidentally deletion.
    uint32_t newBlockNumber;
	ret = dmNewSubDir(file, &dirRec, 0, SHAREDIN_INDEX, 0, &newBlockNumber, &sDirBlock);

	if (SGX_SUCCESS != ret){
		sgx_fclose(file);
		return ret;
	}

	strncpy((char *)(dirRec.name), "sharedout", 10);
    SET_SUBDIR_ATTR(dirRec.attr);
    SET_SHARED(dirRec.attr); //to be prevent accidentally deletion.
	ret = dmNewSubDir(file, &dirRec, 0, SHAREDOUT_INDEX, 0, &newBlockNumber, &sDirBlock);
*/
	sgx_fclose(file);
	return ret;
}

sgx_status_t createSymLink(const char *dirType, uint32_t linkFromStId, uint32_t linkToStId, const char *fromName, const char *toName){
	sgx_status_t ret = SGX_SUCCESS;
	DirRec_t dirRec;

	char fromDirName[MAX_PATH];
	snprintf(fromDirName, MAX_PATH, "%s%d.dir", dirType, linkFromStId);
	uint32_t fromKeyId, fromStId;
  int32_t fromFileId;

	ret = dmFindDirEntry(fromDirName, fromName, &fromKeyId, &fromFileId, &dirRec, &fromStId);
    if (SGX_SUCCESS != ret || -1 != fromFileId) { //from file name conflict
            ret = (sgx_status_t)(-2);
            return ret;
    }

	char toDirName[MAX_PATH];
	snprintf(toDirName, MAX_PATH, "%s%d.dir", dirType, linkToStId);
	uint32_t toKeyId, toStId;
	int32_t toFileId;
	ret = dmFindDirEntry(toDirName, toName, &toKeyId, &toFileId, &dirRec, &toStId);
	if (SGX_SUCCESS != ret || -1 == toFileId) { //to file doesn't exist
					ret = (sgx_status_t)(-3);
					return ret;
	}

	double tm;
	sgx_status_t retv;
	ocall_gettime(&retv, &tm);
	SymLinkInfo_t symInfo;
	symInfo.blockNumber = toFileId/BLOCK_DIR_NUMBER;
	symInfo.fileId = toFileId%BLOCK_DIR_NUMBER;
	symInfo.stId = linkToStId;
	symInfo.srcKeyId = toKeyId;
	symInfo.tm = tm;
    ret = dmAddDir(fromDirName, fromName, &symInfo, &fromKeyId, &fromStId, &fromFileId);
    if (SGX_SUCCESS != ret || -1 == fromFileId) {
            ret = (sgx_status_t)(-1);
            return ret;
    }
    return ret;
}

sgx_status_t initDir(const char * dir, sgx_key_128bit_t *pEncKey){
	DirBlock_t sDirBlock;
	char dirFileName[MAX_PATH];

	getFullPathFileName(dirFileName,MAX_PATH,dir);

	SGX_FILE* file = sgx_fopen(dirFileName, "w", pEncKey);
	if (NULL == file)
		return (sgx_status_t)(-1);
	dmInitDirBlock(&sDirBlock);

	DirRec_t *pRec;
  pRec = &(sDirBlock.fileRecs[0]);
  SET_SUBDIR_ATTR(pRec->attr);
  SET_SHARED(pRec->attr); //to be prevent accidentally deletion.
  strncpy((char *)pRec->name, ".", 2);
  pRec = &(sDirBlock.fileRecs[1]);
  SET_SUBDIR_ATTR(pRec->attr);
  SET_SHARED(pRec->attr); // It is a real symbolci link to parent dir. (for root, the parent dir is itself
  strncpy((char *)pRec->name, "..", 3);
	uint32_t stId=0;
  if (!strncmp(dir, "model", 5)){
  	stId = atoi(dir+5);
  	uint32_t stId = atoi(dir+5);
    pRec = &(sDirBlock.fileRecs[2]);
  	if (1 == stId){
  	    pRec = &(sDirBlock.fileRecs[2]);
  	    SET_SUBDIR_ATTR(pRec->attr);
  	    SET_SHARED(pRec->attr); //to be prevent accidentally deletion.
  	    pRec->subDirBlockNumber = -1;
  	    snprintf((char *)pRec->name, 10, "OPAL");
  	} else {
  		SET_SYMLNK(pRec->attr);
  	    SET_SHARED(pRec->attr); //to be prevent accidentally deletion.
  	    pRec->s.symBlockNumber = 0;
  	    pRec->s.symFileIdx=1*BLOCK_DIR_NUMBER+2; //1:The stakeholder ID of the host user,
  	    										 //2:The file index of the /OPAL folder in the block
  	    snprintf((char *)pRec->name, 10, "OPAL");
  	}
  } else if (!strncmp(dir, "data", 4)){
  	stId = atoi(dir+4);
  	for (int i=2;i<5;i++){
  	    pRec = &(sDirBlock.fileRecs[i]);
  	    SET_SUBDIR_ATTR(pRec->attr);
  	    SET_SHARED(pRec->attr); //to be prevent accidentally deletion.
  	    pRec->subDirBlockNumber = -1;
  	    if (2 == i)
  	    	snprintf((char *)pRec->name, 100, "sharedin");
  	    else if ( 3 == i)
  	    	snprintf((char *)pRec->name, 100, "sharedout");
  	    else
  	    	snprintf((char *)pRec->name, 100, "anonymized");
  	}
  }

	if (1 != sgx_fwrite(&sDirBlock, sizeof(DirBlock_t), 1, file)){
		sgx_fclose(file);
		return (sgx_status_t)-1;
	}
	sgx_fclose(file);
/*
	if (!strncmp(dir, "model", 5) && (stId > 1)) {
		getFullPathFileName(dirFileName,MAX_PATH,"model1.dir");
		file = sgx_fopen(dirFileName, "r", pEncKey);
		if (NULL == file) {//model1.dir doesn't exist, create one
			initDir("model1.dir", pEncKey);
		} else
			sgx_fclose(file);
	    createSymLink("model", stId, 1, "OPAL", "OPAL");
	}
*/
	return SGX_SUCCESS;
}

sgx_status_t deleteDirTree(FILE *fp, DirBlock_t *pDirBlock, uint32_t fileIdx, const char *dir){
	DirBlock_t sBlock;
	DirRec_t sRec;

	if (!IS_SUBDIR(pDirBlock->fileRecs[fileIdx].attr))
		return SGX_SUCCESS;
	uint32_t blockNumber = pDirBlock->fileRecs[fileIdx].subDirBlockNumber;
	while (-1 != blockNumber){
        if (SGX_SUCCESS != sgx_fseek(fp, blockNumber*sizeof(DirBlock_t), SEEK_SET))
        	return SGX_SUCCESS;
        if (1 != sgx_fread(&sBlock, sizeof(DirBlock_t), 1, fp))
        	return SGX_SUCCESS;
        for (int i=2; i<BLOCK_DIR_NUMBER; i++){
        	if ('\0' == sBlock.fileRecs[i].name[0])
        		continue;
        	if (IS_SHARED(sBlock.fileRecs[i].attr))
        		return (sgx_status_t)-1;
        	if (IS_SUBDIR(sBlock.fileRecs[i].attr))
        		if (SGX_SUCCESS != deleteDirTree(fp, &sBlock, i, dir))
        			return (sgx_status_t)-1;
        	else if (!IS_SYMLNK(sBlock.fileRecs[i].attr)){
        		char fullName[MAX_PATH];
                getFileNameOnDisk(fullName, MAX_PATH, dir, blockNumber*BLOCK_DIR_NUMBER+i);
        		sgx_remove(fullName);
        		uint32_t len = strnlen(fullName, MAX_PATH);
        		snprintf(fullName+len, MAX_PATH-len, ".info");
        		sgx_remove(fullName);
        	}
        }
        sBlock.fileRecs[0].name[0]='\0';
        if (SGX_SUCCESS != sgx_fseek(fp, blockNumber*sizeof(DirBlock_t), SEEK_SET))
        	return SGX_SUCCESS;
        if (1 != sgx_fwrite(&sBlock, sizeof(DirRec_t), 1, fp))
        	return SGX_SUCCESS;
        blockNumber=sBlock.fileRecs[0].nextBlock;
	}
	return SGX_SUCCESS;
}

/*
 * Input parameters:
 *   dir: The directory file name, such as "model5.dir", "data7.dir"
 *   name: the name of the file including the path after "data", "lib", or "model"
 * Return: if success return SGX_SUCCESS
 *   added entry: *dirId is the entry number
 *   *pKeyId: the stakeholder ID whose key is used for encrypting the file
 */
 sgx_status_t dmAddDir(const char *dir, const char *name, const SymLinkInfo_t *symInfo, uint32_t *pKeyId, uint32_t *stId, int32_t *dirId){
	char dirFileName[MAX_PATH];
	StakeholderRec_t sRec;
	DirRec_t *pDirRec;

	sRec.seqNumber = 0;
  sgx_status_t ret = dmReadStake(&sRec);

  if (SGX_SUCCESS != ret || sRec.isDeleted)
    	return (sgx_status_t)(-1);

  getFullPathFileName(dirFileName, MAX_PATH, dir);
  SGX_FILE* file = sgx_fopen(dirFileName, "r+",&(sRec.encKey));
  if (NULL == file){
    	if (SGX_SUCCESS != initDir(dir, &(sRec.encKey)))
     		return (sgx_status_t)(-1);
    	file = sgx_fopen(dirFileName, "r+",&(sRec.encKey));
     	if (NULL == file)
     		return (sgx_status_t)(-1);
  }

  ret = (sgx_status_t)-1;
  DirBlock_t *pDirBlock;
  uint32_t blockNumber, fileIdx;
	LOCK(&fileRWLockDir);
	if (SGX_SUCCESS == dmGetFreeDirEntry(file, name, &pDirBlock, &blockNumber, &fileIdx)){
        pDirRec = &(pDirBlock->fileRecs[fileIdx]);
        if (!(IS_SUBDIR(pDirRec->attr))){ //if the file exists replace the file, if it is a folder then it can't be replaced
						if (NULL == symInfo) {
            	CLR_SYMLNK(pDirRec->attr);
								sRec.seqNumber = -1;
                if (SGX_SUCCESS != dmReadStake(&sRec) || sRec.isDeleted){
                	UNLOCK(&fileRWLockDir);
                	sgx_fclose(file);
                	return (sgx_status_t)(-1);
                }
            	pDirRec->keySeqNumber = sRec.seqNumber;
						} else {
            	SET_SYMLNK(pDirRec->attr);
            	pDirRec->s.symBlockNumber=symInfo->blockNumber;
            	pDirRec->s.symFileIdx=symInfo->stId*BLOCK_DIR_NUMBER+symInfo->fileId;
            	pDirRec->keySeqNumber = symInfo->srcKeyId;
            	pDirRec->tm = symInfo->tm;
            }
            *pKeyId = pDirRec->keySeqNumber;
            if (SGX_SUCCESS == sgx_fseek(file, blockNumber*sizeof(DirBlock_t)+fileIdx*sizeof(DirRec_t), SEEK_SET)){
								if (1 == sgx_fwrite((const uint8_t *)&(pDirBlock->fileRecs[fileIdx]), sizeof(DirRec_t), 1, file))
                	ret = SGX_SUCCESS;
            }
        }
	}
	UNLOCK(&fileRWLockDir);
	if (SGX_SUCCESS == ret)
		*dirId = blockNumber*BLOCK_DIR_NUMBER+fileIdx;
    sgx_fclose(file);
    return ret;
}

sgx_status_t dmReadBlock(SGX_FILE *file, uint32_t block, DirBlock_t **pDirBlock){
	if (SGX_SUCCESS != getDirCache(pDirBlock, block)){
		if (SGX_SUCCESS != sgx_fseek(file, block*sizeof(DirBlock_t), SEEK_SET))
			return (sgx_status_t)(-1);
		if (1 != sgx_fread(*pDirBlock, sizeof(DirBlock_t), 1, file))
			return (sgx_status_t)(-1);
	}
	return SGX_SUCCESS;
}

sgx_status_t dmReadRecord(SGX_FILE *file, uint32_t block, uint32_t fileId, DirRec_t *pDirRec){
	if (SGX_SUCCESS != sgx_fseek(file, block*sizeof(DirBlock_t)+fileId*sizeof(DirRec_t), SEEK_SET))
		return (sgx_status_t)(-1);
	if (1 != sgx_fread(pDirRec, sizeof(DirRec_t), 1, file))
		return (sgx_status_t)(-1);
	return SGX_SUCCESS;
}

sgx_status_t dmSearchFile(SGX_FILE *fp, const char *fileName, DirBlock_t **pDirBlock, uint32_t *pBlockNumber, uint32_t *pFileIdx){
		while (true){
    	if (SGX_SUCCESS != getDirCache(pDirBlock, *pBlockNumber)){
        	if (SGX_SUCCESS != sgx_fseek(fp, *pBlockNumber*sizeof(DirBlock_t), SEEK_SET)){
        		return (sgx_status_t)(-1);
        	}
        	if (1 != sgx_fread(*pDirBlock, sizeof(DirBlock_t), 1, fp)) {
        		return (sgx_status_t)(-1);
					}
    	}

    	if (NULL == fileName){
    		*pFileIdx = 0;
    		return SGX_SUCCESS;
    	}
    	for (int i=0;i<BLOCK_DIR_NUMBER;i++){
    		if ('\0' != (*pDirBlock)->fileRecs[i].name[0])
    		if (!strncmp(fileName,(const char *)((*pDirBlock)->fileRecs[i].name),DIR_FILE_NAME_SIZE)){
    			*pFileIdx = i;
    			return (SGX_SUCCESS);
    		}
    	}
    	if (0 == (*pDirBlock)->fileRecs[0].nextBlock)
    		return (sgx_status_t)(-2); //no more block in current subdir
    	*pBlockNumber=(*pDirBlock)->fileRecs[0].nextBlock; //The first record is ".", the dirNextBlock is the next block of current directory
    }
    return (sgx_status_t)(-1);
}

/*
 * Search the path of the file
 */
sgx_status_t dmSearchPath(SGX_FILE **fp, const char *path, DirBlock_t **pDirBlock, uint32_t *pBlockNumber,
		uint32_t *pFileIdx, const char *dirType, uint32_t *stId){
    if (NULL == path) //bad data path
        return (sgx_status_t)-1;
    if (-1 == *pBlockNumber) //empty folder
    	return (sgx_status_t)-1;
    char pathBuf[MAX_PATH];
    strncpy(pathBuf, path, MAX_PATH);
    char *pathName = strtok(pathBuf,"/");
    sgx_status_t ret = dmSearchFile(*fp, pathName, pDirBlock, pBlockNumber, pFileIdx);
		if (SGX_SUCCESS == ret){
	        if (IS_SYMLNK((*pDirBlock)->fileRecs[*pFileIdx].attr)){
	        	while (IS_SYMLNK((*pDirBlock)->fileRecs[*pFileIdx].attr)){
	            	DirRec_t *pDirRec =&((*pDirBlock)->fileRecs[*pFileIdx]);
	    			if (pDirRec->tm>0){
	    				//Todo: get trusted time and check whether the sharing expired
	    			}
	    			uint32_t symSrcFileIdx = pDirRec->s.symFileIdx;
	    			uint32_t symSrcfIdx = symSrcFileIdx % BLOCK_DIR_NUMBER;
	    			uint32_t symSrcStId = symSrcFileIdx / BLOCK_DIR_NUMBER;
	    			uint32_t symSrcBlock = pDirRec->s.symBlockNumber;
	    			*pBlockNumber = symSrcBlock;
	    			*pFileIdx = symSrcfIdx;
	    			if (symSrcStId != *stId){
	    		    	*stId = symSrcStId;
	        			char dir[MAX_PATH], dirFileName[MAX_PATH];
	        			snprintf(dir, MAX_PATH, "%s%d.dir", dirType, symSrcStId);
	        			StakeholderRec_t sRec;
      	        sRec.seqNumber = 0;
								if (SGX_SUCCESS != dmReadStake(&sRec) || sRec.isDeleted){
      	        	ret = (sgx_status_t)(-1);
      	        	break;
      	        }
      	        getFullPathFileName(dirFileName, MAX_PATH, dir);
      	        SGX_FILE *file = sgx_fopen(dirFileName, "r", &(sRec.encKey));
      	        if (NULL == file) {
      	        	ret = (sgx_status_t)(-1);
      	        	break;
      	        }
      	        sgx_fclose(*fp);
      	        *fp = file;
	    			}
	    			if (SGX_SUCCESS == ret && SGX_SUCCESS != dmReadBlock(*fp, symSrcBlock, pDirBlock)){
        	        	ret = (sgx_status_t)(-1);
        	        	break;
	    			}
	        	}
	        }
	        if (SGX_SUCCESS == ret && NULL != pathName){
	            char *newPath = strchr(path+1,'/');
	            if (NULL != newPath && IS_SUBDIR((*pDirBlock)->fileRecs[*pFileIdx].attr)){
	                *pBlockNumber = (*pDirBlock)->fileRecs[*pFileIdx].subDirBlockNumber;
	            	ret = dmSearchPath(fp, newPath, pDirBlock, pBlockNumber, pFileIdx, dirType, stId);
	            }
	        }
		}
        return ret;
}

uint32_t parseDir(const char *dir, char *dirType, size_t maxLen){
	int i;
	uint32_t stId=0;
	for (i=0;i<maxLen;i++){
		if ('.' == dir[i] || ('\0' == dir[i]) || (dir[i]>='0' && dir[i]<='9')){
			stId = atoi(&(dir[i]));
			break;
		}
	}
	strncpy(dirType, dir, i);
	dirType[i]='\0';
	return stId;
}

/*
 * Find the entry for a given data/model name.
 * If found return SGX_SUCCESS and the directory sequence number in dirId, and the encryption key Id in keyId
 * If not found at end of directory file, return SGX_SUCCESS and dirId == -1
 * If error, return -1;
 */
sgx_status_t dmFindDirEntry(const char *dir, const char *name, uint32_t *keyId, int32_t *dirId,
		DirRec_t *dirRec, uint32_t *newStId){
    char dirFileName[MAX_PATH];
    StakeholderRec_t sRec;
    sRec.seqNumber = 0;
    if (SGX_SUCCESS != dmReadStake(&sRec))
      return (sgx_status_t)(-1);
		if(sRec.isDeleted) {
			return (sgx_status_t)(-1);
		}
    getFullPathFileName(dirFileName, MAX_PATH, dir);
    SGX_FILE* file = sgx_fopen(dirFileName, "r", &(sRec.encKey));
    if (NULL == file)
    	return (sgx_status_t)(-1);

    uint32_t blockNumber=0;
    uint32_t fileIdx; //=0; //"." in root directory is the root itself
    DirBlock_t *pDirBlock;
    char dirType[MAX_PATH];
		*newStId = parseDir(dir, dirType, MAX_PATH);

		LOCK(&fileRWLockDir);
		if ('l' == dir[0] && 'i' == dir[1] && 'b' == dir[2] && '.' == dir[3])
			initDirCache(dir, false);
		else
			initDirCache(NULL, false);
    if (SGX_SUCCESS != dmSearchPath(&file, name, &pDirBlock, &blockNumber, &fileIdx, dirType, newStId)){
			*dirId = -1;
    } else {
            *dirId=fileIdx+blockNumber*BLOCK_DIR_NUMBER;
            *keyId = pDirBlock->fileRecs[fileIdx].keySeqNumber;
            if (NULL != dirRec)
            	memmove(dirRec, &(pDirBlock->fileRecs[fileIdx]), sizeof(DirRec_t));
    }
    UNLOCK(&fileRWLockDir);
   	sgx_fclose(file);
    return SGX_SUCCESS;
}

sgx_status_t dmGetFileNameArray(const char *dir, const char *path, char *rspBuf, size_t maxLen, size_t *pCurLen){
        char dirFileName[MAX_PATH];
        StakeholderRec_t sRec;

        uint32_t keyId, stId;
        int32_t dirId;

        if (SGX_SUCCESS != dmFindDirEntry(dir, path, &keyId, &dirId, NULL, &stId))
                return (sgx_status_t)-1;

        size_t curLen = *pCurLen;

        snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"filenames\":[");
        curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);

        sRec.seqNumber = 0;
        sgx_status_t ret = dmReadStake(&sRec);

				if (SGX_SUCCESS != ret || sRec.isDeleted)
                return (sgx_status_t)(-1);

        //snprintf(dirFileName,MAX_PATH,"%s/%s",enclaveHome,dir);
        getFullPathFileName(dirFileName,MAX_PATH,dir);

        SGX_FILE* file = sgx_fopen(dirFileName, "r", &(sRec.encKey));
        if (NULL == file)
                return (sgx_status_t)(-1);

        int offset=0;
        bool firstfile=true;
        uint32_t blockNumber = dirId/BLOCK_DIR_NUMBER;
        while (true){
                DirBlock_t sDirBlock;

                sgx_fseek(file, blockNumber*sizeof(DirBlock_t), SEEK_SET);
                if (1 != sgx_fread(&sDirBlock, sizeof(DirBlock_t), 1, file)){
                        if (sgx_feof(file)){ // encounter end of file
                                break;
                        }
                        sgx_fclose(file);
                        return (sgx_status_t)(-1);
                }

                for (int i=0;i<BLOCK_DIR_NUMBER;i++){
                        if ('\0' != sDirBlock.fileRecs[i].name[0] && '.' != sDirBlock.fileRecs[i].name[0]){//"." means a hidden file
                                if (firstfile){
                                        snprintf((rspBuf+curLen), maxLen-curLen-1, "\"%s\"", sDirBlock.fileRecs[i].name);
                                        curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
                                        firstfile = false;
                                } else {
                                        snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"%s\"", sDirBlock.fileRecs[i].name);
                                        curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
                                }
                        }
                }
                blockNumber = sDirBlock.fileRecs[0].nextBlock;
                if (0 == blockNumber)
                        break;
        }
        sgx_fclose(file);
        snprintf((rspBuf+curLen), maxLen-curLen-1, "]");
        curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
        *pCurLen = curLen;
        return SGX_SUCCESS;
}


sgx_status_t dmGetDirList(const char *dir, const char *path, //reserve for future
                char *result, size_t *pLen){
	char dirFileName[MAX_PATH];
	StakeholderRec_t sRec;

	sRec.seqNumber = 0;
	sgx_status_t ret = dmReadStake(&sRec);

	if (SGX_SUCCESS != ret || sRec.isDeleted)
		return (sgx_status_t)(-1);

	//snprintf(dirFileName,MAX_PATH,"%s/%s",enclaveHome,dir);
	getFullPathFileName(dirFileName,MAX_PATH,dir);

	SGX_FILE* file = sgx_fopen(dirFileName, "r", &(sRec.encKey));
	if (NULL == file)
		return (sgx_status_t)(-1);

	int offset=0;
	while (true){
		DirBlock_t sDirBlock;

		if (1 != sgx_fread(&sDirBlock, sizeof(DirBlock_t), 1, file)){
			if (sgx_feof(file)){ // encounter end of file
				break;
			}
			sgx_fclose(file);
			return (sgx_status_t)(-1);
		}

		for (int i=0;i<BLOCK_DIR_NUMBER;i++){
			if ('\0' != sDirBlock.fileRecs[i].name[0] && '.' != sDirBlock.fileRecs[i].name[0]){//"." means a hidden file
				snprintf(result+offset, *pLen-offset, "%s", sDirBlock.fileRecs[i].name);
				offset += strnlen(result+offset,*pLen-offset)+1;
				if (offset>=*pLen){
					offset = *pLen-1;
					break;
				}
				*(result+offset-1) = '\n';
			}
		}
	}
	*(result+offset)='\0';
	*pLen = offset+1;
	sgx_fclose(file);
	return SGX_SUCCESS;
}


void dmInitDirBlock(DirBlock_t *pBlock){
        DirRec_t *pRec;
        for (int i=0; i<BLOCK_DIR_NUMBER; i++){
                pRec = &(pBlock->fileRecs[i]);
                pRec->attr = 0;
                pRec->subDirBlockNumber = 0;
                //pRec->s.symBlockNumber=0;
                pRec->size=0;

                pRec->tm = 0;
                pRec->name[0]='\0';
        }
        pBlock->fileRecs[0].name[0]='.';
        pBlock->fileRecs[0].name[1]='\0';
}

sgx_status_t dmGetFreeDirBlock(SGX_FILE *fp, uint32_t *pBlockNumber){
	uint32_t blockNumber = 0;
	DirBlock_t sBlock;
	DirRec_t sRec;
	while(true){
		sgx_fseek(fp, blockNumber*sizeof(DirBlock_t), SEEK_SET);
		if (1 != sgx_fread(&sRec, sizeof(DirRec_t), 1, fp)){
			if (sgx_feof(fp)){
				*pBlockNumber = blockNumber;
				return SGX_SUCCESS;
			}
			else
				return (sgx_status_t)-1;
		}
		if ('\0' == sRec.name[0]){
			*pBlockNumber = blockNumber;
			return SGX_SUCCESS;
		}
		blockNumber++;
	}
	return SGX_SUCCESS;
}

/*
 * Inputs:
 *     pBlock: the first block in the directory
 *     pBlockNumber: current block number
 * Outputs:
 *     pBlock: the block contains the free entry
 *     pBlockNumber: the number of the block containing the free entry
 *     pFileIdx: the free entry number in the block
 */
sgx_status_t dmGetFreeEntryInCurrentDir(SGX_FILE *fp, DirBlock_t *pBlock, uint32_t *pBlockNumber, uint32_t *pFileIdx){
    //uint32_t block=0;
	//uint8_t found=0;
	uint32_t idx=0;
	uint32_t newBlockNumber;
	while (true){
		sgx_fseek(fp, *pBlockNumber*sizeof(DirBlock_t), SEEK_SET);
		if (1 != sgx_fread(pBlock, sizeof(DirBlock_t), 1, fp))
			return (sgx_status_t)-1;
		for (int i=1; i<BLOCK_DIR_NUMBER; i++){
			if ('\0' == pBlock->fileRecs[i].name[0]) {
				idx = i;
				break;
			}
		}
		if (0 != idx)
			break;
		newBlockNumber = pBlock->fileRecs[0].nextBlock;
		if (0 == newBlockNumber){
			if (SGX_SUCCESS != dmGetFreeDirBlock(fp, &newBlockNumber))
				return (sgx_status_t)-1;
			pBlock->fileRecs[0].nextBlock = newBlockNumber;
			sgx_fseek(fp, *pBlockNumber*sizeof(DirBlock_t), SEEK_SET);
			if (1 != sgx_fwrite(pBlock, sizeof(DirRec_t), 1, fp))
				return (sgx_status_t)-1;
			dmInitDirBlock(pBlock);
			pBlock->fileRecs[0].prevBlock = *pBlockNumber;
			sgx_fseek(fp, newBlockNumber*sizeof(DirBlock_t), SEEK_SET);
			if (1 != sgx_fwrite(pBlock, sizeof(DirBlock_t), 1, fp))
				return (sgx_status_t)-1;
			idx = 1;
			*pBlockNumber = newBlockNumber;
			break;
		} else {
			*pBlockNumber = newBlockNumber;
		}
	}
	*pFileIdx = idx;
	return SGX_SUCCESS;
}

sgx_status_t dmNewSubDir(SGX_FILE *fp, DirRec_t *pRec, uint32_t currBlkNum, uint32_t currFileIdx, uint32_t firstBlkOfCurrDir,
		uint32_t *pNewBlockNumber, DirBlock_t *pBlockBuff){
	if (SGX_SUCCESS != dmGetFreeDirBlock(fp, pNewBlockNumber))
		return (sgx_status_t)-1;
	SET_SUBDIR_ATTR(pRec->attr);
	pRec->subDirBlockNumber = *pNewBlockNumber;
	sgx_fseek(fp, currBlkNum*sizeof(DirBlock_t)+currFileIdx*sizeof(DirRec_t), SEEK_SET);
	if (1 != sgx_fwrite(pRec, sizeof(DirRec_t), 1, fp))
		return (sgx_status_t)-1;
	dmInitDirBlock(pBlockBuff);
	pBlockBuff->fileRecs[0].prevBlock = *pNewBlockNumber;
	pBlockBuff->fileRecs[1].prevBlock = firstBlkOfCurrDir;
	pBlockBuff->fileRecs[1].s.symBlockNumber = currBlkNum;
	pBlockBuff->fileRecs[1].s.symFileIdx = currFileIdx;
	pBlockBuff->fileRecs[1].name[0]='.';
	pBlockBuff->fileRecs[1].name[1]='.';
	pBlockBuff->fileRecs[1].name[2]='\0';
	sgx_fseek(fp, *pNewBlockNumber*sizeof(DirBlock_t), SEEK_SET);
	if (1 != sgx_fwrite(pBlockBuff, sizeof(DirBlock_t), 1, fp))
	      return (sgx_status_t)-1;
	return SGX_SUCCESS;
}

/*
 * Get a free directory entry under the given path
 * Inputs:
 *   name: the path to the file (the file name is included)
 * Outputs: when return SGX_SUCCESS
 *   *pBlock: the directory block that the free entry is in it
 *   *pBlockNumber: the block number that the entry is in
 *   *pFileIdx: the entry number in the block
 */
sgx_status_t dmGetFreeDirEntry(SGX_FILE *fp, const char *name, DirBlock_t **pBlock,
		uint32_t *pBlockNumber, uint32_t *pFileIdx){
    uint32_t newBlockNumber, fileIdx;
    uint32_t firstBlockNumber;
    sgx_status_t ret;
    if (NULL == name)
            return (sgx_status_t)-1;
    char pathBuf[MAX_PATH];
    strncpy(pathBuf, name, MAX_PATH);
    char *pathName = strtok(pathBuf,"/");
    if (NULL == pathName)
    	return (sgx_status_t)(-1); //empty path
    *pBlockNumber = 0;
    while (true) {
    	initDirCache(NULL, true);
    	firstBlockNumber = *pBlockNumber;
        ret = dmSearchFile(fp, pathName, pBlock, pBlockNumber, pFileIdx);
        if ((sgx_status_t)(-1) == ret)
            return ret; //something error
        else if (SGX_SUCCESS != ret)
        	break; //Not found
        pathName = strtok(NULL, "/");
        if (NULL == pathName)
        	return SGX_SUCCESS; //file is found, can be either a file or a sub directory
        else {
            if (IS_SUBDIR((*pBlock)->fileRecs[*pFileIdx].attr)){
            	DirRec_t *pRec = &((*pBlock)->fileRecs[*pFileIdx]);
            	uint32_t blkNum = pRec->subDirBlockNumber;
            	uint32_t newBlockNumber;
                if (-1 == blkNum){ //empty folder
                	ret = dmNewSubDir(fp, pRec, *pBlockNumber, *pFileIdx, firstBlockNumber, &newBlockNumber, *pBlock);
                	if (SGX_SUCCESS != ret)
                		return ret;
                	*pBlockNumber = newBlockNumber;
                } else
                	*pBlockNumber = blkNum;
            } else
            	return (sgx_status_t)(-1); //existing file conflicts with path, can't add a new file.
        }
    }
    *pBlockNumber = firstBlockNumber;
    ret = dmGetFreeEntryInCurrentDir(fp, *pBlock, pBlockNumber, &fileIdx);
    if (SGX_SUCCESS != ret)
    	return (sgx_status_t)-1;
    while (true){//create directories in the path
    	DirRec_t *pRec = &((*pBlock)->fileRecs[fileIdx]);
    	strncpy((char *)pRec->name, pathName, DIR_FILE_NAME_SIZE);
    	pathName = strtok(NULL, "/");
    	if (NULL == pathName){
        	 SET_FILE_ATTR(pRec->attr);
    		break;
    	}

    	ret = dmNewSubDir(fp, pRec, *pBlockNumber, fileIdx, firstBlockNumber, &newBlockNumber, *pBlock);
    	if (SGX_SUCCESS != ret)
    		break;
    	*pBlockNumber = newBlockNumber;
    	firstBlockNumber = newBlockNumber;
    	fileIdx = 2; //0: ".", 1:"..", 2:the first file or subdir
    }
    *pFileIdx = fileIdx;
    return SGX_SUCCESS;
}

sgx_status_t dmSetFileAttr(const char *dirName, uint32_t fileId, uint32_t attr){
	StakeholderRec_t sRec;

	sRec.seqNumber = 0;
	sgx_status_t ret = dmReadStake(&sRec);

	if (SGX_SUCCESS != ret || sRec.isDeleted)
		return (sgx_status_t)(-1);

	char dirFileName[MAX_PATH];
	//snprintf(dirFileName,MAX_PATH,"%s/%s",enclaveHome,dirName);
	getFullPathFileName(dirFileName,MAX_PATH,dirName);

	SGX_FILE* file = sgx_fopen(dirFileName, "r+", &(sRec.encKey));
	if (NULL == file)
		return (sgx_status_t)(-1);

	//uint32_t block_size = BLOCK_DIR_NUMBER*sizeof(DirRec_t); //sgx_calc_sealed_data_size(0,BLOCK_DIR_NUMBER*sizeof(DirRec_t));
	uint32_t block = fileId / BLOCK_DIR_NUMBER;
	uint32_t idx = fileId % BLOCK_DIR_NUMBER;

	if (SGX_SUCCESS != sgx_fseek(file, block*sizeof(DirBlock_t)+idx*sizeof(DirRec_t), SEEK_SET)){
		sgx_fclose(file);
		return (sgx_status_t)(-1);
	}

	DirRec_t sDirRec;
	if (1 != sgx_fread(&sDirRec, sizeof(DirRec_t), 1, file)){
		sgx_fclose(file);
		return (sgx_status_t)(-1);
	}

	sDirRec.attr = attr;

	if (SGX_SUCCESS != sgx_fseek(file, block*sizeof(DirBlock_t)+idx*sizeof(DirRec_t), SEEK_SET)){
		sgx_fclose(file);
		return (sgx_status_t)(-1);
	}
	if (1 != sgx_fwrite(&sDirRec, sizeof(DirRec_t), 1, file)){
		sgx_fclose(file);
		return (sgx_status_t)(-1);
	}

	sgx_fclose(file);
	return SGX_SUCCESS;
}

sgx_status_t dmUpdateFileSize(const char *dirName, uint32_t fileId, size_t size){
	StakeholderRec_t sRec;

	sRec.seqNumber = 0;
	sgx_status_t ret = dmReadStake(&sRec);

	if (SGX_SUCCESS != ret || sRec.isDeleted)
		return (sgx_status_t)(-1);

	char dirFileName[MAX_PATH];
	//snprintf(dirFileName,MAX_PATH,"%s/%s",enclaveHome,dirName);
	getFullPathFileName(dirFileName,MAX_PATH,dirName);

	SGX_FILE* file = sgx_fopen(dirFileName, "r+", &(sRec.encKey));
	if (NULL == file)
		return (sgx_status_t)(-1);

	//uint32_t block_size = BLOCK_DIR_NUMBER*sizeof(DirRec_t); //sgx_calc_sealed_data_size(0,BLOCK_DIR_NUMBER*sizeof(DirRec_t));
	uint32_t block = fileId / BLOCK_DIR_NUMBER;
	uint32_t idx = fileId % BLOCK_DIR_NUMBER;

	if (SGX_SUCCESS != sgx_fseek(file, block*sizeof(DirBlock_t)+idx*sizeof(DirRec_t), SEEK_SET)){
		sgx_fclose(file);
		return (sgx_status_t)(-1);
	}

	DirRec_t sDirRec;
	if (1 != sgx_fread(&sDirRec, sizeof(DirRec_t), 1, file)){
		sgx_fclose(file);
		return (sgx_status_t)(-1);
	}

	sDirRec.size = size;

	if (SGX_SUCCESS != sgx_fseek(file, block*sizeof(DirBlock_t)+idx*sizeof(DirRec_t), SEEK_SET)){
		sgx_fclose(file);
		return (sgx_status_t)(-1);
	}
	if (1 != sgx_fwrite(&sDirRec, sizeof(DirRec_t), 1, file)){
		sgx_fclose(file);
		return (sgx_status_t)(-1);
	}

	sgx_fclose(file);
	return SGX_SUCCESS;
}

void freeBlock(FILE *fp, DirBlock_t *pBlock, uint32_t blockNumber){
	if (0 == blockNumber)
		return;
	uint32_t preBlock = pBlock->fileRecs[0].prevBlock;
	uint32_t nextBlock = pBlock->fileRecs[0].nextBlock;
	if (preBlock == blockNumber) { //The first block in the subdirectory
		return;
	}
	DirRec_t sRec;

	if (SGX_SUCCESS != sgx_fseek(fp, blockNumber*sizeof(DirBlock_t), SEEK_SET))
		return;
	pBlock->fileRecs[0].name[0] = '\0';
	if (1 != sgx_fread(pBlock, sizeof(DirRec_t), 1, fp))
		return;

	if (SGX_SUCCESS != sgx_fseek(fp, preBlock*sizeof(DirBlock_t), SEEK_SET))
		return;
	if (1 != sgx_fread(&sRec, sizeof(DirRec_t), 1, fp))
		return;
	sRec.nextBlock = nextBlock;
	if (SGX_SUCCESS != sgx_fseek(fp, preBlock*sizeof(DirBlock_t), SEEK_SET))
		return;
	if (1 != sgx_fwrite(&sRec, sizeof(DirRec_t), 1, fp))
		return;

	if (0 != nextBlock){
		if (SGX_SUCCESS != sgx_fseek(fp, nextBlock*sizeof(DirBlock_t), SEEK_SET))
			return;
		if (1 != sgx_fread(&sRec, sizeof(DirRec_t), 1, fp))
			return;
		sRec.prevBlock = preBlock;
		if (SGX_SUCCESS != sgx_fseek(fp, nextBlock*sizeof(DirBlock_t), SEEK_SET))
			return;
		if (1 != sgx_fwrite(&sRec, sizeof(DirRec_t), 1, fp))
			return;
	}
}

/*
 * Parameters:
 *   dir: The directory file name, can be "model.dir", "data.dir" or subdir name
 *   *dirId -- delete a directory entry with seq# == *dirId
 *   pRec->name: the name of the data/model, file with name start with '.' is not allowed to be deleted
 *
 * Return: if success return SGX_SUCCESS
 *
 */
sgx_status_t dmDelDirByDirId(const char *dir, int32_t *dirId, const char *name){
	char dirFileName[MAX_PATH];
	char fileNameOnDisk[MAX_PATH];
	StakeholderRec_t sRec;
	if ('.' == *name &&
			('\0' == *(name+1) || ('.' == *(name+1) && '\0' == *(name+2)) ))
		return (sgx_status_t)-1; //".", ".." can't be deleted
	sRec.seqNumber = 0;
	sgx_status_t ret = dmReadStake(&sRec);

	if (SGX_SUCCESS != ret || sRec.isDeleted)
		return (sgx_status_t)(-1);

	//snprintf(dirFileName,MAX_PATH,"%s/%s",enclaveHome,dir);
	getFullPathFileName(dirFileName,MAX_PATH,dir);
	SGX_FILE* file = sgx_fopen(dirFileName, "r+", &(sRec.encKey));
	if (NULL == file)
		return (sgx_status_t)(-1);
	uint32_t block = *dirId / BLOCK_DIR_NUMBER;

	if (SGX_SUCCESS != sgx_fseek(file, block*sizeof(DirBlock_t), SEEK_SET)){
		sgx_fclose(file);
		return (sgx_status_t)(-1);
	}
	DirBlock_t sDirBlock;
	if (1 != sgx_fread(&sDirBlock, sizeof(DirBlock_t), 1, file)){
		sgx_fclose(file);
		return (sgx_status_t)(-1);
	}
	uint32_t idx = *dirId % BLOCK_DIR_NUMBER;

	if (IS_SHARED(sDirBlock.fileRecs[idx].attr)){
		sgx_fclose(file);
		return (sgx_status_t)-1;
	}
	if (IS_SUBDIR(sDirBlock.fileRecs[idx].attr))
		deleteDirTree(file, &sDirBlock, idx, dir);
	getFileNameOnDisk(fileNameOnDisk, MAX_PATH, dir, *dirId);
	sgx_remove(fileNameOnDisk);
	uint32_t nameLen = strnlen(fileNameOnDisk, MAX_PATH);
	if ((nameLen+10)<MAX_PATH){
		snprintf(fileNameOnDisk+nameLen, 10, ".info");
		sgx_remove(fileNameOnDisk);
	}


	sDirBlock.fileRecs[idx].name[0] = '\0';
	if (SGX_SUCCESS != sgx_fseek(file, block*sizeof(DirBlock_t)+idx*sizeof(DirRec_t), SEEK_SET)){
		sgx_fclose(file);
		return (sgx_status_t)(-1);
	}
	if (1 != sgx_fwrite(&(sDirBlock.fileRecs[idx]), sizeof(DirRec_t), 1, file)){
		sgx_fclose(file);
		return (sgx_status_t)(-1);
	}
	int i;
	for (i=2; i<BLOCK_DIR_NUMBER; i++)
		if ('\0' != sDirBlock.fileRecs[idx].name[i])
			break;
	if (BLOCK_DIR_NUMBER <= i) {
		freeBlock(file, &sDirBlock, block);
	}

	sgx_fclose(file);
	return SGX_SUCCESS;
}

sgx_status_t getDirName(const char *name, char *dir, size_t dirSize, uint32_t stkHldrSeqNum, uint32_t *offset){

    strncpy(dir, name, dirSize);

    char *path = strtok(dir, "/");
    if (!strncmp(path, "lib", dirSize)){
        strncpy(dir,"lib.dir", dirSize);
        *offset = 4;
    } else if (!strncmp(path, "tmp", dirSize)) {
    	//ret = oc_open_file(file, name+4, mode);
		strncpy(dir, "tmp", dirSize);
    	// *dir = '\0';
    	*offset = 4;
    } else if (!strncmp(path, "data", dirSize)){
    	snprintf(dir,dirSize-1,"data%d.dir",stkHldrSeqNum);
    	*offset = 5;
    } else if (!strncmp(path, "model", dirSize)){
    	snprintf(dir,dirSize-1,"model%d.dir",stkHldrSeqNum);
    	*offset = 6;
    }
    else {
   		return (sgx_status_t)-1;
    }
    return SGX_SUCCESS;
}

/*
 * Delete the entry for a given data/model name.
 * If found return SGX_SUCCESS
 * If error, return -1;
 */
sgx_status_t dmDelDirEntryByName(const char *dir, const char *name){
	sgx_status_t ret;
	uint32_t keyId, stId;
	int32_t dirId;
	DirRec_t sdRec;
  char dirType[MAX_PATH];
	uint32_t oldStId = 	parseDir(dir, dirType, MAX_PATH);
	//LOCK(&writerLockDir);
	ret = dmFindDirEntry(dir, name, &keyId, &dirId, &sdRec, &stId);
	if (SGX_SUCCESS == ret && -1 != dirId && (oldStId == stId)) {
		LOCK(&fileRWLockDir);
		ret = dmDelDirByDirId(dir, &dirId, name);
		UNLOCK(&fileRWLockDir);
	}
	//UNLOCK(&writerLockDir);
	return(ret);
}

int dmRemove(const char *filename){
    uint32_t stkHldrSeqNum = getStakeHolderId();

    char dir[MAX_PATH];   //name of the file to be opened. eg. data2.dir
    char dataFileName[MAX_PATH];
    uint32_t offset;
    if (SGX_SUCCESS != getDirName(filename, dir, MAX_PATH, stkHldrSeqNum, &offset))
    	return -1;
	filename = filename + offset;
    if ('\0' == *dir){
    	char tmpFileName[MAX_PATH];
    	snprintf(tmpFileName, MAX_PATH, "tmp%s", filename);
    	getFullPathFileName(dataFileName, MAX_PATH-1, tmpFileName);
    	return(sgx_remove(dataFileName));
    }
    if (SGX_SUCCESS != dmDelDirEntryByName(dir, filename))
    	return -1;
    return 0;
}

/*
* Remove's stakeholder's record and data
* 1- Update the stakeholder record and set isDelete to true
* 2- Remove the data and model files and directories
*/
sgx_status_t dmRemoveStake(StakeholderRec_t *pRec) {
	sgx_status_t ret;
	char dir[MAX_PATH];
	char dirFullPath[MAX_PATH];
	uint32_t seqNumber = 1 + (3 * (pRec->stId - 1));
	pRec->isDeleted = true;
	for(int role = 1; role <= 3; role++) {
		pRec->role = role;
		ret = dmUpdateStake(pRec, true); // update records for all three roles
		if(SGX_SUCCESS != ret) {
			return (sgx_status_t)(-1);
		}
	}

	// delete user's data dir and data files
	snprintf(dir, MAX_PATH, "data%d.dir", seqNumber);
	dmDelDir(dir);	// delete data files
	getFullPathFileName(dirFullPath, MAX_PATH, dir);
	if(SGX_SUCCESS != sgx_remove(dirFullPath)) {
		return (sgx_status_t)(-1);
	}

	// delete user's model.dir and model files
	snprintf(dir, MAX_PATH, "model%d.dir", seqNumber);
	dmDelDir(dir);	// delete model files
	getFullPathFileName(dirFullPath, MAX_PATH, dir);
	sgx_remove(dirFullPath);
	return SGX_SUCCESS;
}

/*
* Deletes directory files
*/
sgx_status_t dmDelDir(const char *dir) {
	char dirFileName[MAX_PATH];
	char fileList[MAX_PATH];
	StakeholderRec_t sRec;
	sRec.seqNumber = 0;
	sgx_status_t ret = dmReadStake(&sRec);
	if (SGX_SUCCESS != ret || sRec.isDeleted) {
		return (sgx_status_t)(-1);
	}

	getFullPathFileName(dirFileName,MAX_PATH,dir);
	SGX_FILE* file = sgx_fopen(dirFileName, "r", &(sRec.encKey));
	if (NULL == file) {
		return (sgx_status_t)(-1);
	}
	while (true){
		DirBlock_t sDirBlock;
		if (1 != sgx_fread(&sDirBlock, sizeof(DirBlock_t), 1, file)){
			if (sgx_feof(file)){ // encounter end of file
				sgx_fclose(file);
				return SGX_SUCCESS;
			}
			sgx_fclose(file);
			return (sgx_status_t)(-1);
		}
		for (int i=0;i<BLOCK_DIR_NUMBER;i++){
			if ('\0' != sDirBlock.fileRecs[i].name[0] &&
					'.' != sDirBlock.fileRecs[i].name[0]) {	//"." means a hidden file
				snprintf(fileList, MAX_PATH, "%s", sDirBlock.fileRecs[i].name);
				if (SGX_SUCCESS != dmDelDirEntryByName(dir, fileList)) {
					return (sgx_status_t)(-1);
				}
			}
		}
	}
}

sgx_status_t dmListDir(const char *dir) {
	char dirFileName[MAX_PATH];
	char fileList[MAX_PATH], outputString[MAX_PATH];
	StakeholderRec_t sRec;
	sgx_status_t p1;
	sRec.seqNumber = 0;
	sgx_status_t ret = dmReadStake(&sRec);
	if (SGX_SUCCESS != ret || sRec.isDeleted) {
		return (sgx_status_t)(-1);
	}

	getFullPathFileName(dirFileName,MAX_PATH,dir);
	SGX_FILE* file = sgx_fopen(dirFileName, "r", &(sRec.encKey));
	if (NULL == file) {
		return (sgx_status_t)(-1);
	}

	snprintf(outputString, MAX_PATH, "\nFile list in %s:\n", dir);
	ocall_print(&p1, (const char*)outputString, MAX_PATH);
	while (true){
		DirBlock_t sDirBlock;
		if (1 != sgx_fread(&sDirBlock, sizeof(DirBlock_t), 1, file)){
			if (sgx_feof(file)){ // encounter end of file
				sgx_fclose(file);
				return SGX_SUCCESS;
			}
			sgx_fclose(file);
			return (sgx_status_t)(-1);
		}
		for (int i=0;i<BLOCK_DIR_NUMBER;i++){
			if ('\0' != sDirBlock.fileRecs[i].name[0]) {	//"." means a hidden file
				snprintf(fileList, MAX_PATH, "%s, %d, %d", sDirBlock.fileRecs[i].name, sDirBlock.fileRecs[i].keySeqNumber, sDirBlock.fileRecs[i].s.symFileIdx);
				ocall_print(&p1, (const char*)fileList, MAX_PATH);
				ocall_print(&p1, "\n", 10);
			}
		}
	}
}

sgx_status_t dmWriteFileInfo(const char *dirFileName, const char *fileName, const char *infoStr){
    sgx_status_t ret;
    uint32_t keyId, stId;
    int32_t dirId;
    StakeholderRec_t sRec;
    DirRec_t dirRec;

    ret = dmFindDirEntry(dirFileName, fileName, &keyId, &dirId, &dirRec, &stId);
    if (SGX_SUCCESS != ret || -1 == dirId) {
            return (sgx_status_t)(-1);
    }
		sRec.seqNumber = keyId;
    ret = dmReadStake(&sRec);
		if (SGX_SUCCESS != ret || sRec.isDeleted)
      return (sgx_status_t)(-1);

    char prefix[MAX_PATH];

    getInfoFileName(dirFileName, dirId, prefix, MAX_PATH);

		ShareInfoFileHeader_t header;
		header.version = DATA_SHARE_INFO_VERSION;
		header.shares = 0;
		ret = dmWriteFile(prefix, &(sRec.encKey), (uint8_t *)&header, sizeof(ShareInfoFileHeader_t));
		ret = dmAppendFile(prefix, &(sRec.encKey),(uint8_t *)infoStr,strnlen(infoStr, 1<<20));

		return ret;
}
/* Write data into a file under either /data or /model directory depends on the value of "dir" parameter
 * Input:
 *   dir: the directory file name, such as data12.dir, or model15.dir
 *   name: the full path and data file name without the prefix of /data or /model
 *   buf: data to be written to a file
 *   len: the size of the data to be written
 *   isLarge: indicate whether the data has multiple slices
 *   offset: the offset of the current piece
 *   info: the data information. If it is not null, it will be written in the a file with .info post-fix.
 */
sgx_status_t dmWriteDataOrModel(const char *dir, const char *name, const uint8_t *buf, size_t len,
		uint32_t isLarge, uint32_t offset){
	sgx_status_t ret;
	uint32_t keyId, stId;
	int32_t dirId;
	StakeholderRec_t sRec;
	if (SGX_SUCCESS != dmAddDir(dir, name, NULL, &keyId, &stId, &dirId)) {
		return (sgx_status_t)(-1);
	}
	sRec.seqNumber = keyId;
	ret = dmReadStake(&sRec);
	if (SGX_SUCCESS != ret || sRec.isDeleted) {
		return (sgx_status_t)(-1);
	}
	char prefix[DIR_FILE_NAME_SIZE];
	getFileName(dir, dirId, prefix, DIR_FILE_NAME_SIZE);
	//snprintf(prefix, DIR_FILE_NAME_SIZE, "%s.%d", dir, dirId);
 	ret = dmWriteFile(prefix, &(sRec.encKey), buf, len);
 	dmUpdateFileSize(dir, dirId, len);
	return ret;
}

/*
 * Inputs: dir: the directory file name, can be either "model.dir" or "data.dir"
 *   name: name of the data/model to be read
 *   buf: the buffer for the data to be red
 *   len: the max buffer size
 * Outputs:
 *   len: size of data read if return SGX_SUCCESS
 */
sgx_status_t dmReadDataOrModel(const char *dir, const char *name, uint8_t *buf, size_t *len){
	sgx_status_t ret;
	uint32_t keyId, stId;
	int32_t dirId;
	StakeholderRec_t sRec;

	char dirType[MAX_PATH];
	uint32_t oldStId = 	parseDir(dir, dirType, MAX_PATH);
	ret = dmFindDirEntry(dir, name, &keyId, &dirId, NULL, &stId);
	if (SGX_SUCCESS != ret || -1 == dirId) {
	        return (sgx_status_t)(-1);
	}
	sRec.seqNumber = keyId;
	ret = dmReadStake(&sRec);
	if (SGX_SUCCESS != ret || sRec.isDeleted) {
	  return (sgx_status_t)(-1);
	}
	char prefix[DIR_FILE_NAME_SIZE];
	const char *newDir = dir;
	char newDirBuf[MAX_PATH];
	if (oldStId != stId){
		snprintf(newDirBuf, MAX_PATH, "%s%d.dir", dirType, stId);
		newDir = newDirBuf;
	}
	getFileName(newDir, dirId, prefix, DIR_FILE_NAME_SIZE);
	ret = dmReadFile(prefix, &(sRec.encKey), buf, len);
	return ret;
}

sgx_status_t dmOpenFileWithStId(const char *name, const char *mode, FILE **file, uint32_t stkHldrSeqNum){
	sgx_status_t ret;
	char dir[MAX_PATH]; 	 //name of the file to be opened. eg. data2.dir
	char dataFileName[MAX_PATH];
	uint32_t offset;

	ret = getDirName(name, dir, MAX_PATH, stkHldrSeqNum, &offset);
	if (SGX_SUCCESS != ret) {
		return ret;
	}

	oc_print("Opening file:", name);
	name = name + offset;
	if ('\0' == *dir || !strncmp(dir, "tmp", 3)) {
		void *fp;
		getTmpFileNameOnDisk(dataFileName, MAX_PATH, name);
		if (memchr(mode, 'r', strnlen(mode, 3)) && ('\0' == *dir)) {
			return (sgx_status_t)-1;
		}
		ret = oc_fopen(&fp, dataFileName, mode);
		if (SGX_SUCCESS == ret && NULL != fp) {
			*file = addExtFile(fp);
			return SGX_SUCCESS;
		}
		return (sgx_status_t)-1;
	}

	char dirType[MAX_PATH];
	uint32_t oldStId = parseDir(dir, dirType, MAX_PATH);
	uint32_t keyId, stId = oldStId;
	int32_t dirId;
	StakeholderRec_t sRec;

	if (NULL != memchr(mode,'w', strnlen(mode, 3))){
		ret = dmAddDir(dir, name, NULL, &keyId, &stId, &dirId); //Create a directory entry for the file if the file is not exist
	} else {
		DirRec_t sdRec;
		ret = dmFindDirEntry(dir, name, &keyId, &dirId, &sdRec, &stId);
	}
	if (SGX_SUCCESS != ret || -1 == dirId)
		return (sgx_status_t)-1;

	sRec.seqNumber = keyId;
	ret = dmReadStake(&sRec);
	if (SGX_SUCCESS != ret || sRec.isDeleted)
		return (sgx_status_t)(-1);

	char prefix[MAX_PATH];
	char *newDir = dir;
	char newDirBuf[MAX_PATH];
	if (oldStId != stId){
		snprintf(newDirBuf, MAX_PATH, "%s%d.dir", dirType, stId);
		newDir = newDirBuf;
	}

	getFileNameOnDisk(dataFileName, MAX_PATH, newDir, dirId);

	*file = sgx_fopen(dataFileName, mode, &(sRec.encKey));
	if (*file == NULL){
		return (sgx_status_t)(-1);
	}
	return ret;
}

/*
 * SGX Wrapper function for fopen. It will open the files located outside of enclave
 *
 * Paramters:
 *
 *          name : name of the file inside enclave as the table entry in the file data.dir
 *          mode : mode in which file is needed to be opened eg. "w","r", "wb"
 *          file : empty pointer which will hold address to the file opened
 *
 */
sgx_status_t dmOpenFile(const char *name, const char *mode, FILE **file){           // const char *dir){ //TODO : what goes in the dir?
        sgx_status_t ret;
        uint32_t stkHldrSeqNum = getStakeHolderId();
/*
    	sgx_thread_t threadId = sgx_thread_self();
    	TaskTableNode *tNode = taskTable.getNodeByETID(threadId);
    	if (NULL != tNode)
    		stkHldrSeqNum = tNode->sessionNode->stakeholderId;
    	else {
    		stkHldrSeqNum = getStakeHolderIdByThreadId(threadId);
    		if ((uint32_t)-1 == stkHldrSeqNum)
        		return (sgx_status_t)-1;
        }
*/
    	ret = dmOpenFileWithStId(name, mode, file, stkHldrSeqNum);

        return ret;
}


/*
 * fopen which in turn calls dmOpenFile to open a file
 * name : name of the file inside the enclave
 * mode : mode in which the file needs to be opened
 */
SGX_FILE *fopen(const char *name, const char *mode){

	FILE *file;
	sgx_status_t ret;
    uint32_t stId = getStakeHolderId();

    if (!strncmp(name, "/data/", 6)){
    	char realName[MAX_PATH];
    	strncpy(realName, sandboxPath, MAX_PATH);
    	strncpy(realName+sandboxPathSize, name+5, MAX_PATH-sandboxPathSize);
    	ret = dmOpenFileWithStId(realName,mode,&file,stId);
    } else
    	ret = dmOpenFileWithStId(name,mode,&file,stId);
	if(SGX_SUCCESS != ret){
		file = NULL;
	}

	return file;
}

/**
 * Close the file
 */
int32_t fclose(FILE *file){
	if (isExtFile(file)){
		delExtFile(file);
		int32_t rval;
		if (SGX_SUCCESS == oc_fclose(&rval, *(void **)file))
			return rval;
		return (-1);
	}
	return sgx_fclose(file);
}

/*
 * fread
 */

size_t fread(void *ptr, size_t size, size_t nmemb, FILE *file){
	if (isExtFile(file)){
		size_t rval;
		if (SGX_SUCCESS == oc_fread(&rval, ptr, size*nmemb, size, nmemb, *(void **)file))
			return rval;
		return 0;
	}
	size_t ret=sgx_fread(ptr,size,nmemb,file);
	return ret;
}

/*
 * fwrite
 */

//size_t fwrite(const void *ptr, size_t size, size_t nmemb, SGX_FILE *file){
//	return sgx_fwrite(ptr,size,nmemb,file);
//}

/*
 * FEOF
 */
int32_t feof(FILE *file){
	if (isExtFile(file)){
		int32_t rval;
		if (SGX_SUCCESS == oc_feof(&rval, *(void **)file))
			return rval;
		return (-1);
	}
	return sgx_feof(file);
}

/*
 * fflush
 */

int32_t fflush(FILE* file){
	if(file==stdout){
		outbuf.cx=0;
		return 0;
	}
	else if(file==stdin){
		inbuf.cx=0;
		return 0;
	}
	else if(file==stderr){
		errsbuf.cx=0;
		return 0;
	}
	else if (isExtFile(file)){
		int32_t rval;
		if (SGX_SUCCESS == oc_fflush(&rval, *(void **)file))
			return(rval);
		return (-1);
	} else
	    return sgx_fflush(file);
}

/*
 * ftell
 */
int64_t ftell(FILE* file){
	if (isExtFile(file)){
		int64_t rval;
		if (SGX_SUCCESS == oc_ftell(&rval, *(void **)file))
			return rval;
		return -1;
	}
	return sgx_ftell(file);
}

/*
 * ferror
 */
int32_t ferror(FILE *file){
	if (isExtFile(file)){
		int32_t rval;
		if (SGX_SUCCESS == oc_ferror(&rval, *(void **)file))
			return rval;
		return -1;
	}
	return sgx_ferror(file);
}

/*
 * clearerr
 */

void clearerr(FILE* file){
	if (isExtFile(file)){
		oc_clearerr(*(void **)file);
	} else
		sgx_clearerr(file);
}

/*
 * fseek
 */

int fseek(FILE* file, int64_t offset, int origin){
	if (isExtFile(file)){
		int rval;
		if (SGX_SUCCESS == oc_fseek(&rval, *(void **)file, offset, origin))
			return rval;
		return -1;
	}
	return sgx_fseek(file,offset,origin);
}

int remove(const char* filename){
	return(dmRemove(filename));
}

/*
 * fputs
 */
//int fputs(const char *str,SGX_FILE *fp){
//        int str_len = 0;
//        int i = 0;
//        for(i = 0; str[i] != '\0'; ++i){
//        }
//        str_len = i;
//        sgx_fwrite(str,sizeof(char),str_len,fp);
//}


/*
 * fgetc reads one character at a time.

int fgetc (SGX_FILE* file){
	char buf[1];
	sgx_fread(buf,1,1,file);
	return buf[0];
}
 */
int fgetc(FILE *fp){
	if (isExtFile(fp)){
		int rval;
		if (SGX_SUCCESS == oc_fgetc(&rval, *(void **)fp))
			return rval;
		return -1;
	}
        char buffer[1];
        size_t numRead = sgx_fread(buffer, 1,1, fp);
        if ( 1 == numRead)
        	return (int)buffer[0];
        else
        	return (-1);
}


/*
 * fputc prints one character into the file
 */

int fputc(int chr, FILE *file){
	if (isExtFile(file)){
		int rval;
		if (SGX_SUCCESS == oc_fputc(&rval, chr, *(void **)file))
			return rval;
		return -1;
	}
	if(file==stdout){
		if(outbuf.cx+1<MSG_BUF_LEN){
		    outbuf.buf[outbuf.cx]=chr;
		    outbuf.cx++;
		    outbuf.buf[outbuf.cx]='\0';
		    return int(chr);
		}
		return -1;
	}
	else if(file==stderr){
		if(errsbuf.cx+1<MSG_BUF_LEN){
		    errsbuf.buf[errsbuf.cx]=chr;
		    errsbuf.cx++;
		    errsbuf.buf[errsbuf.cx]='\0';
		    return int(chr);
		}
		return -1;
	}
	return sgx_fwrite(&chr, 1, 1, file);
}


/*
 * fgets
 */
char *fgets (char *str, int num, FILE* fp){
        int i = 0;
        for(i = 0; i < num - 1; i++){
                char c = fgetc(fp);
                if(c == '\n'){
                        str[i] = c;
                        str[i+1] = '\0';
                        return str;
                }
                str[i] = c;
        }
        str[i] = '\0';
}

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream){
	//dmWriteDataFile(stream, (const uint8_t *)ptr, size*nmemb);
	size_t len1 = strlen((const char*) ptr);
	if (isExtFile(stream)){
		size_t rval;
		if (SGX_SUCCESS == oc_fwrite(&rval, ptr, size*nmemb, size, nmemb, *(void **)stream))
			return rval;
		return(0);
	}
	if(stream==stdout){
		size_t len2 = MSG_BUF_LEN-outbuf.cx-1;
		size_t len = len2>len1?len1:len2;
		strncpy(outbuf.buf+outbuf.cx, (const char*) ptr, len);
        outbuf.cx+=len;
        return len;
	}
	else if(stream==stdin){
	    size_t len2 = MSG_BUF_LEN-inbuf.cx-1;
		size_t len = len2>len1?len1:len2;
		strncpy(inbuf.buf+inbuf.cx, (const char*) ptr, len);
		inbuf.cx+=len;
		return len;
	}
	else if(stream==stderr){
	    size_t len2 = MSG_BUF_LEN-errsbuf.cx-1;
		size_t len = len2>len1?len1:len2;
		strncpy(errsbuf.buf+errsbuf.cx, (const char*) ptr, len);
		errsbuf.cx+=len;
		return len;
	}
	else{
	    return sgx_fwrite(ptr,size, nmemb, stream);
	}
}

int isdir(char *path){
        sgx_status_t ret;

        char dir[DIR_FILE_NAME_SIZE];   //name of the file to be opened. eg. data2.dir
        char dataFileName[MAX_PATH];
        uint32_t stkHldrSeqNum = getStakeHolderId();

        uint32_t offset;
        ret = getDirName(path, dir, DIR_FILE_NAME_SIZE, stkHldrSeqNum, &offset);
        if (SGX_SUCCESS != ret)
        	return ret;
        path = path + offset;
        if ('\0' == *dir){
        	int isDirType;
        	if (SGX_SUCCESS == oc_isdir(&isDirType, path))
        		return isDirType;
        	return 0;
        }

        char dirFileName[MAX_PATH];
        StakeholderRec_t sRec;

        sRec.seqNumber = 0;
        ret = dmReadStake(&sRec);

				if (SGX_SUCCESS != ret || sRec.isDeleted)
          return 0;

        getFullPathFileName(dirFileName,MAX_PATH,dir);

        SGX_FILE* file = sgx_fopen(dirFileName, "r", &(sRec.encKey));
        if (NULL == file)
                return 0;

        uint32_t blockNumber=0;
        uint32_t fileIdx; //=0; //"." in root directory is the root itself
        char dirType[MAX_PATH];
		uint32_t stId=parseDir(dir, dirType, MAX_PATH);
        DirBlock_t *pDirBlock;

    	LOCK(&fileRWLockDir); //preventing the block data in the cache being evicted.
		if ('l' == dir[0] && 'i' == dir[1] && 'b' == dir[2] && '.' == dir[3])
			initDirCache(dir, false);
		else
			initDirCache(NULL, false);
		bool isSubDir=true;
		if (strncmp(path, "/",2)){
			ret = dmSearchPath(&file, path, &pDirBlock, &blockNumber, &fileIdx, dirType, &stId);
			if (SGX_SUCCESS == ret)
				isSubDir = IS_SUBDIR(pDirBlock->fileRecs[fileIdx].attr);
		}
        UNLOCK(&fileRWLockDir);
   		sgx_fclose(file);

        if (SGX_SUCCESS != ret)
        	return 0;

        if (isSubDir)
            return 1;
        return 0;
}
