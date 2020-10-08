/*
 * enclave_view.cpp
 *
 * To generate an enclave information tree JSON with the element of:
 * {
 *   "module":module name string,
 *   "children:[children elements]
 * }
 *
 *  Created on: Feb 23, 2019
 *      Author: accuser
 */

#include "./include/enclave_view.h"
#include "./include/data_management.h"
#include "./include/data_share.h"
#include "./include/dir_management.h"
#include "./include/session.h"
#include "./include/dir_cache.h"
#include "conversion.h"

TreeElement *getDirTree(char *dir, const char *virDir, const char *path, int depth, int stId);

TreeElement *findDotNode(TreeElement *tree){
	while (NULL != tree){
		if (!strncmp(tree->getModule(), ".", 2)){
			return tree;
		}
		tree = tree->getSibling();
	}
	return NULL;
}

long getFileListJson(TreeElement *dirTree, char * rspBuf, size_t maxLen){
	size_t len = 0;
	if (NULL != dirTree){
		if (NULL != dirTree->getChild()){
			TreeElement *tnode = dirTree->getChild();
			while (NULL != tnode){
				bool isFolder = (NULL != tnode->getChild());
				snprintf((rspBuf+len), maxLen-len,
					"{\"name\":\"%s\",\"size\":%lu, \"modified\":%lu, \"folder\":%d},",
					tnode->getModule(), tnode->size*!isFolder, tnode->tm, isFolder);
				len += strnlen((char *)(rspBuf+len), maxLen-len);
				tnode = tnode->getSibling();
			}
		}
		if (',' == *(rspBuf+len-1))
			len--;
	}
	return len;
}

long getRootInfo(int stId, char * rspBuf, size_t maxLen, const char *virDir){
	size_t len = 0;
	char fullDirName[MAX_PATH];
	char dir[MAX_PATH];
	if (!strncmp(virDir, "lib", 4))
		snprintf(dir,MAX_PATH,"lib.dir");
	else
		snprintf(dir,MAX_PATH,"%s%d.dir", virDir, stId);
	getFullPathFileName(fullDirName, MAX_PATH, dir);
	TreeElement *dirTree = getDirTree(fullDirName, virDir, "/", 1, stId);
	if (NULL != dirTree){
		TreeElement *tNode = dirTree->getChild();
		if (NULL != tNode){
			snprintf((rspBuf+len), maxLen-len,
			"{\"name\":\"%s\",\"size\":0, \"modified\":%lu, \"folder\":1},", virDir, tNode->tm);
			len += strnlen((char *)(rspBuf+len), maxLen-len);
		}
		delete dirTree;
	}
	return len;
}

long getFileList(int stId, char * rspBuf, size_t maxLen, const char *path){
	long len=0, maxSize = maxLen;
	sgx_ec256_public_t pubKey;
	char fullDirName[MAX_PATH];

	snprintf((rspBuf+len), maxLen-len, ",\"result\":[");
	len += strnlen((char *)(rspBuf+len), maxLen-len);

	TreeElement *dirTree;
	if (!strncmp(path, "/", 2)){//root path
		len += getRootInfo(stId, rspBuf+len, maxLen, "data");
		len += getRootInfo(stId, rspBuf+len, maxLen, "lib");
		len += getRootInfo(stId, rspBuf+len, maxLen, "model");
		if (',' == *(rspBuf+len-1))
			len--;
	}else {
		char dir[MAX_PATH];
		if (!strncmp(path, "/data", 5)){
			snprintf(dir,MAX_PATH,"data%d.dir", stId);
			getFullPathFileName(fullDirName, MAX_PATH, dir);
			dirTree = getDirTree(fullDirName, "data", path+5, 1, stId);
		} else if (!strncmp(path, "/lib", 4)){
			snprintf(dir,MAX_PATH,"lib.dir");
			getFullPathFileName(fullDirName, MAX_PATH, dir);
			dirTree = getDirTree(fullDirName, "lib", path+4, 1, stId);
		} else if (!strncmp(path, "/model", 6)){
			snprintf(dir,MAX_PATH,"model%d.dir", stId);
			getFullPathFileName(fullDirName, MAX_PATH, dir);
			dirTree = getDirTree(fullDirName, "model", path+6, 1, stId);
		} else
			dirTree = NULL;
		if (NULL == dirTree)
			return 0;
		len += getFileListJson(dirTree, rspBuf+len, maxLen);
		if (NULL != dirTree)
			delete dirTree;
	}
	snprintf((rspBuf+len), maxLen-len, "]");
	len += strnlen((char *)(rspBuf+len), maxLen-len);
	return len;
}

void getDirBlockTree(SGX_FILE *file, const char *dirType, uint32_t block, TreeElement& tree, int depth, int stId){
   	DirBlock_t sDirBlock;

		if (0 == depth) {
			TreeElement *child = new TreeElement("+", -1, 0, 0, 0, 0);
			if (NULL != child) { tree.addChild(*child); }
			return;
   	}

   	while (true) {
   		if (SGX_SUCCESS != sgx_fseek(file, block*sizeof(DirBlock_t), SEEK_SET))
   			return;
 	   	if (1 != sgx_fread(&sDirBlock, sizeof(DirBlock_t), 1, file)){
   			return;
   		}

   	   	for (int i=1; i<BLOCK_DIR_NUMBER; i++){
   			if ('\0' != sDirBlock.fileRecs[i].name[0] && '.' != sDirBlock.fileRecs[i].name[0]){
   				TreeElement *child=new TreeElement((char *)(sDirBlock.fileRecs[i].name), block*BLOCK_DIR_NUMBER+i,
					sDirBlock.fileRecs[i].keySeqNumber, stId, sDirBlock.fileRecs[i].tm, sDirBlock.fileRecs[i].size);
					if (NULL == child)
	   				break;
   				if (IS_SUBDIR(sDirBlock.fileRecs[i].attr)){
   	   			tree.addChild(*child);
	   				getDirBlockTree(file, dirType, sDirBlock.fileRecs[i].subDirBlockNumber, *child, depth-1, stId);
   				}
   				else if (IS_SYMLNK(sDirBlock.fileRecs[i].attr)){
   	   			SGX_FILE *symFile;
   					uint32_t symSrcFileIdx = sDirBlock.fileRecs[i].s.symFileIdx;
   					uint32_t symSrcfIdx = symSrcFileIdx % BLOCK_DIR_NUMBER;
   					uint32_t symSrcStId = symSrcFileIdx / BLOCK_DIR_NUMBER;
   					uint32_t symSrcBlock = sDirBlock.fileRecs[i].s.symBlockNumber;
   					child->stId = symSrcStId;
   					child->dirId = symSrcBlock*BLOCK_DIR_NUMBER+symSrcfIdx;
   					child->symLinkStId = stId;
   	   	   			if (symSrcStId != stId){
   	   	   	   			char dir[MAX_PATH];
   	   	   	   			char fullDirName[MAX_PATH];
   	   	   				snprintf(dir,MAX_PATH,"%s%d.dir", dirType, symSrcStId);
   	   	   				getFullPathFileName(fullDirName, MAX_PATH, dir);
   	   	   				StakeholderRec_t sRec;

   	   	   				sRec.seqNumber = 0;
   	   	   				sgx_status_t ret = dmReadStake(&sRec);
									if (SGX_SUCCESS != ret || sRec.isDeleted)
   	   	   					break;
   	   	   				symFile = sgx_fopen(fullDirName, "r", &(sRec.encKey));
   	   	   				if (NULL == symFile){
   	   	   					delete child;
   	   	   					continue;
   	   	   				}
   	   	   			} else
   	   	   				symFile = file;
   	   	   			bool validShare = false;
   	   				if (SGX_SUCCESS == sgx_fseek(symFile, symSrcBlock*sizeof(DirBlock_t), SEEK_SET)){
   	   					DirBlock_t sSrcDirBlock;
   	   	   				if (1 == sgx_fread(&sSrcDirBlock, sizeof(DirBlock_t), 1, symFile)){
   	   	   					if ('\0' != sSrcDirBlock.fileRecs[symSrcfIdx].name[0]){
   	   	    					child->keyId = sSrcDirBlock.fileRecs[symSrcfIdx].keySeqNumber;
   	   	    					char dir[MAX_PATH];
   	   	    					snprintf(dir, MAX_PATH, "%s%d.dir", dirType, symSrcStId);
   	   	    					char infoFileName[MAX_PATH];
   	   	    					getInfoFileNameOnDisk(infoFileName, MAX_PATH, dir, child->dirId);
   	   	    					if (validShareInfo(infoFileName, sSrcDirBlock.fileRecs[symSrcfIdx].keySeqNumber, stId,
   	   	    							sDirBlock.fileRecs[i].tm)){
   	   	   	    	   				tree.addChild(*child);
   	   	   	    	   				validShare = true;
   	   	   	    	   				if (IS_SUBDIR(sSrcDirBlock.fileRecs[symSrcfIdx].attr))
   	   	   	    						getDirBlockTree(symFile, dirType, sSrcDirBlock.fileRecs[symSrcfIdx].subDirBlockNumber, *child, depth-1, symSrcStId);
   	   	    					}
   	   	   					}
   	   	   				}
   	   				}
   	   				if (!validShare)
   	   					delete child;
   	   				if (symFile != file){
   	   					sgx_fclose(symFile);
   	   				}
   				} else
   	   				tree.addChild(*child);
   			}
   		}
   		block = sDirBlock.fileRecs[0].nextBlock;
   		if (0 == block)
   			return;
   	}
}

TreeElement *getDirTree(char *dir, const char *dirType, const char *path, int depth, int stId){
    uint32_t block=0;
    StakeholderRec_t sRec;

    sRec.seqNumber = 0;
    sgx_status_t ret = dmReadStake(&sRec);

		if (SGX_SUCCESS != ret || sRec.isDeleted)
    	return NULL;

    SGX_FILE* file = sgx_fopen(dir, "r", &(sRec.encKey));
    if (NULL == file)
            return NULL;

    TreeElement *tree=NULL;

    if ('\0' == path[0] || !strncmp(path, "/",2)) {
        tree = new TreeElement(dirType, -1, 0, 0, 0, 0);
    	if(NULL != tree){
    		getDirBlockTree(file, dirType, block, *tree, depth, stId);
    	}
    } else {
    	bool isSubDir=false;
    	DirBlock_t *pDirBlock;
    	uint32_t fileIdx;
      char dirType[MAX_PATH];
			uint32_t stId=parseDir(strrchr(dir,'/')+1, dirType, MAX_PATH);
			LOCK(&fileRWLockDir);
	    initDirCache(NULL, false);//disable block cache
	    if (SGX_SUCCESS == dmSearchPath(&file, path, &pDirBlock, &block, &fileIdx, dirType, &stId)){
	    	tree = new TreeElement((char *)(pDirBlock->fileRecs[fileIdx].name), block*BLOCK_DIR_NUMBER+fileIdx,
	    			pDirBlock->fileRecs[fileIdx].keySeqNumber, stId, pDirBlock->fileRecs[fileIdx].tm,
					pDirBlock->fileRecs[fileIdx].size);
	    	block = pDirBlock->fileRecs[fileIdx].subDirBlockNumber;

	    	isSubDir = IS_SUBDIR(pDirBlock->fileRecs[fileIdx].attr);
	    }
			UNLOCK(&fileRWLockDir);
			if (isSubDir){
				if(NULL != tree){
					getDirBlockTree(file, dirType, block, *tree, depth, stId);
				}
			}
    }

	sgx_fclose(file);
	return tree;
}

long tree2JsonString(TreeElement& tree, char *buf, long bufSize, uint32_t maxLayer, bool readDataInfo){
	long len=0;
	snprintf(buf+len, bufSize - len, "{\"module\":\"%s\",", tree.getModule());
	len += strnlen(buf+len, bufSize - len);
	TreeElement *child;
	child = tree.getChild();
	if (NULL != child){
		if (bufSize < 2000 || maxLayer<=0){
			snprintf(buf+len, bufSize - len, "\"children\":[{\"module\":\"+\",\"unloaded\":true}]");
			len += strnlen(buf+len, bufSize - len);
		} else {
			snprintf(buf+len, bufSize - len, "\"children\":[");

			len += strnlen(buf+len, bufSize - len);
			maxLayer--;
			while (NULL != child){//more sibling?
				long ln;
				ln= tree2JsonString(*child, buf+len, bufSize-len-500, maxLayer, readDataInfo);
				child = child->getSibling();
				if (ln>0){
					len += ln;
					snprintf(buf+len, bufSize - len, ",");
					len += strnlen(buf+len, bufSize - len);
				}else
					break;
			}
			if (',' == *(buf+len-1))
				len--;

			snprintf(buf+len, bufSize - len, "]");
			len += strnlen(buf+len, bufSize - len);
		}
	} else {
		snprintf(buf+len, bufSize - len, "\"leaf\":true");
		len += strnlen(buf+len, bufSize - len);
	}

	if (readDataInfo && (tree.stId>0)){ //dirName is not null means this call is for data which could has .info file.
		size_t infoSize = 1024*10-1;
        StakeholderRec_t sRec;
        char dir[MAX_PATH];
		char dataFileName[MAX_PATH];

		snprintf(dir, MAX_PATH, "%s%d.dir", "data", tree.stId);
		getInfoFileNameOnDisk(dataFileName, MAX_PATH, dir, tree.dirId);
    sRec.seqNumber = tree.getKeyId();
    if (SGX_SUCCESS == dmReadStake(&sRec)){
			if(sRec.isDeleted) {
				return (sgx_status_t)(-1);
			}
    	size_t infoLen;
    	uint8_t *infoBuf = readShareDataInfo(dataFileName, &(sRec.encKey), tree.symLinkStId, &infoLen);
    	if (NULL != infoBuf){
    		snprintf(buf+len, bufSize - len, ",\"info\":%s", infoBuf);
    		len += strnlen(buf+len, bufSize - len);
	        free(infoBuf);
    	}
    }
	}
	snprintf(buf+len, bufSize - len, "}");
	len += strnlen(buf+len, bufSize - len);
	if (bufSize - len < 10)
		len = 0;
	return len;
}

size_t getDirInfoJson(int stId, const char *dirType, char * rspBuf, size_t maxLen, const char *path, int depth){
	size_t len = 0;
	char fullDirName[MAX_PATH];
	char dir[MAX_PATH];
	bool readDataInfo=false;
	if (stId>=0) {
		snprintf(dir, MAX_PATH, "%s%d.dir", dirType, stId);
		//snprintf(fullDirName,MAX_PATH,"%s/%s%d.dir",enclaveHome, virDir, stId);
		if (!strncmp(dirType, "data", 5)){
			readDataInfo=true;
		}
	} else
		snprintf(dir, MAX_PATH, "%s.dir", dirType);
		//snprintf(fullDirName,MAX_PATH,"%s/%s.dir",enclaveHome, virDir);
	getFullPathFileName(fullDirName, MAX_PATH, dir);
	TreeElement *dirTree = getDirTree(fullDirName, dirType, path, depth, stId);
	if (NULL != dirTree){
		//len += strnlen((char *)(rspBuf+len), maxLen-len);
		len += tree2JsonString(*dirTree, rspBuf+len, maxLen-len, depth, readDataInfo);
		delete dirTree;
		//if (len >= maxLen-10)
		//	return 0;
		return len;
	}
	return 0;
}

size_t getStakeholderInfo(char * buf, size_t bufSize){
	StakeholderRec_t sRec;
	uint32_t stId = 4;
	size_t len = 0;
	sRec.seqNumber = 0;
	if (SGX_SUCCESS != dmReadStake(&sRec))
		return len;
	if(sRec.isDeleted) {
		return len;
	}
	snprintf(buf+len, bufSize - len, "{\"module\":\"stakeholders\",\"children\":[{\"module\":\"Runner\",\"children\":[");
	len += strnlen(buf+len, bufSize - len);
	char ethBuf[41]; ethBuf[40]='\0';
	bin2hex(sRec.ethAddr, 20, ethBuf);
	snprintf(buf+len, bufSize - len, "{\"module\":\"0x%s\",\"leaf\":true}]}", ethBuf);
	len += strnlen(buf+len, bufSize - len);
	sRec.seqNumber = 1;
	if (SGX_SUCCESS != dmReadStake(&sRec) || sRec.isDeleted){
		snprintf(buf+len, bufSize - len, "]}");
		len += strnlen(buf+len, bufSize - len);
		return len;
	}
	snprintf(buf+len, bufSize - len, ",{\"module\":\"Trainer\",\"children\":[");
	len += strnlen(buf+len, bufSize - len);
	bin2hex(sRec.ethAddr, 20, ethBuf);
	snprintf(buf+len, bufSize - len, "{\"module\":\"0x%s\",\"leaf\":true}]}", ethBuf);
	len += strnlen(buf+len, bufSize - len);
	snprintf(buf+len, bufSize - len, ",{\"module\":\"Data donors\",\"children\":[");
	len += strnlen(buf+len, bufSize - len);
	while (true){
		sRec.seqNumber=stId;
		if (SGX_SUCCESS != dmReadStake(&sRec))
			break;
		if(sRec.isDeleted) {
			continue;
		}
		bin2hex(sRec.ethAddr, 20, ethBuf);
		snprintf(buf+len, bufSize - len, "{\"module\":\"0x%s\",\"leaf\":true},", ethBuf);
		len += strnlen(buf+len, bufSize - len);
		stId++;
	}
	if (',' == *(buf+len-1)){
		*(buf+len-1) = '\0';
		len--;
	}
	snprintf(buf+len, bufSize - len, "]}]}");
	len += strnlen(buf+len, bufSize - len);
	if (len >= bufSize-10)
		return 0;
	return len;
}

enum ShareType{
	SHARED2HOST,
	SHARED2GUEST
};

long getElementInfoJson(uint32_t stId, char * buf, long bufSize, const char *path){
	long retLen;
	char dir[MAX_PATH], virDir[MAX_PATH];
	if (!strncmp(path, "/stakeholders", strnlen("/stakeholders",20))){
		if (stId != 0)
			return 0;
		if (strnlen(path, 20) != strnlen("/stakeholders",20))
			return 0;
		retLen = getStakeholderInfo(buf, bufSize);
	} else if (!strncmp(path, "/data", 5)){
		retLen = getDirInfoJson(stId, "data", buf, bufSize, path+5, 2);
	} else if (!strncmp(path, "/lib", 4)){
		retLen = getDirInfoJson(-1, "lib", buf, bufSize, path+4, 1);
	} else if (!strncmp(path, "/model", 6)){
		retLen = getDirInfoJson(stId, "model", buf, bufSize, path+6, 2);
	}
	if (retLen >= bufSize-10)
		return 0;
	return retLen;
}

#define min(a,b) (a>b?b:a)

long getEnclaveInfoJson(uint32_t stId, char * buf, long bufSize, const char *path){
	long len=0, maxSize = bufSize;
	sgx_ec256_public_t pubKey;

	snprintf((buf+len), bufSize-len, ",\"result\":");
	len += strnlen((char *)(buf+len), bufSize-len);

	if (NULL != path && strncmp(path, "/", 2)){//with a path but it is not "/" (root path)
		long retLen = getElementInfoJson(stId, buf+len, bufSize-len, path);
		if (0 == retLen){
			snprintf((buf+len), bufSize-len, "{}");
			len += strnlen((char *)(buf+len), bufSize-len);
		}
		return len+retLen;
	}

	if (SGX_SUCCESS != dmGetPublicKey(&pubKey))
		return 0;
	char keyBuf[129];keyBuf[128]='\0';
	bin2hex((const uint8_t *)&pubKey, 64, keyBuf);
	switch (stId){
	case 0: snprintf(buf+len, bufSize - len, "{\"module\":\"Administrator\",");	break;
	case 1: snprintf(buf+len, bufSize - len, "{\"module\":\"Enclave Host\",");	break;
	default: snprintf(buf+len, bufSize - len, "{\"module\":\"Enclave Guest\",");	break;
	}
	len += strnlen(buf+len, bufSize - len);

	snprintf(buf+len, bufSize - len, "\"children\":[");
	len += strnlen(buf+len, bufSize - len);
	size_t retLen;
	if (0 == stId){
		retLen = getStakeholderInfo(buf+len, bufSize-len);
		if (retLen>0){
			len += retLen;
			snprintf(buf+len, bufSize - len, ",");
			len += strnlen(buf+len, bufSize - len);
		}
	}
	//snprintf(dir,MAX_PATH,"%s/%s.dir",enclaveHome, "lib");
	retLen = getDirInfoJson(-1, "lib", buf+len, min(bufSize-len, maxSize/4), "/", 1);
	if (retLen>0){
		len += retLen;
		snprintf(buf+len, bufSize - len, ",");
		len += strnlen(buf+len, bufSize - len);
	}

	retLen = getDirInfoJson(stId, "data", buf+len, min(bufSize-len, maxSize/4), "/", 2);
	if (retLen>0){
		len += retLen;
		snprintf(buf+len, bufSize - len, ",");
		len += strnlen(buf+len, bufSize - len);
	} else if (stId>=4){
		snprintf(buf+len, bufSize - len, "{\"module\":\"data\",\"children\":[]},");
		len += strnlen(buf+len, bufSize - len);
	}

	if (stId>0){
		//snprintf(dir,MAX_PATH,"%s/%s%d.dir",enclaveHome, "model", node->stakeholderId);
		retLen = getDirInfoJson(stId, "model", buf+len, bufSize-len, "/", 2);
		if (retLen>0){
			len += retLen;
		} else {
			snprintf(buf+len, bufSize - len, "{\"module\":\"model\",\"children\":[]}");
			len += strnlen(buf+len, bufSize - len);
		}
	}

	if (',' == *(buf+len-1))
		len--;

	snprintf(buf+len, bufSize - len, "]}");
	len += strnlen(buf+len, bufSize - len);
	//if (len >= bufSize-10)
	//	len = 0;
	return len;
}
