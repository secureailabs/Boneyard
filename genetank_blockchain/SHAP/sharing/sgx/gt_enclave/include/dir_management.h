/*
 * dir_management.h
 *
 *  Created on: Feb 5, 2019
 *      Author: accuser
 */

#ifndef INCLUDE_DIR_MANAGEMENT_H_
#define INCLUDE_DIR_MANAGEMENT_H_

#include "sgx_trts.h"
#include "sgx_error.h"
#include "sgx_tcrypto.h"
#include "string.h"
#include <stdio.h>
#include "sgx_tprotected_fs.h"
#include "sgx_tseal.h"
#include "sgx_report.h"
#include "sgx_key.h"
#include "sgx_thread.h"
//#include "sgx_tae_service.h"
#include "msg_def.h"
#include <stddef.h> /* for size_t */

#define DIR_FILE_NAME_SIZE 64

#define SET_SUBDIR_ATTR(attr) attr = attr | (1<<8)
#define SET_FILE_ATTR(attr) attr = attr & ~(1<<8)
#define IS_SUBDIR(attr) (attr & (1<<8))

#define SET_SHARED(attr) attr = attr | (1<<6)
#define CLR_SHARED(attr) attr = attr & ~(1<<6)
#define IS_SHARED(attr) (attr & (1<<6))

#define SET_SYMLNK(attr) attr = attr | (1<<7)
#define CLR_SYMLNK(attr) attr = attr & ~(1<<7)
#define IS_SYMLNK(attr) (attr & (1<<7))

#define LOCK(mutex_lock) sgx_thread_mutex_lock(mutex_lock)
#define UNLOCK(mutex_lock) sgx_thread_mutex_unlock(mutex_lock)

#define SHAREDIN_INDEX 2
#define SHAREDOUT_INDEX 3
typedef struct _DirSymLink{
	uint32_t symBlockNumber; //for symbol links and directory ".."
	uint32_t symFileIdx; //for symbol links and directory "..",
		                  //if pointing to different stakeholder's data: stakeholderId*BLOCK_DIR_NUMBER+fileIdx
}DirSymLink_t;

enum IsSymLink{
	NON_SYM_LINK=0,
	SYM_LINK=1,
};
typedef struct _SymLinkInfo{
	uint32_t blockNumber;
	uint32_t fileId;
	uint32_t stId;
	uint32_t srcKeyId;
	uint64_t tm;
} SymLinkInfo_t;
//Real data file name on disk is "data"/"model" + stakeholder id + ".dir." + dirId
//Where dirId is the location of the file in the directory file.
typedef struct _DirRec{
	uint8_t name[DIR_FILE_NAME_SIZE];
	/*
	 * (attr & 1<<8)? 0: file, 1: directory
	 * (attr & 1<<7)? 0: non-symbol link, 1: symbol link
	 * (attr & 1<<6)? 0: not shared, 1: shared
	 *
	 */
	uint32_t attr;
	uint64_t tm;
	union {
		size_t size; //for file entry
		uint32_t nextBlock; //Entry 0 in a directory block: next block in the directory
		DirSymLink_t s;     //Entry 1 ("..") in the first directory block of a sub directory:
		                    //  symBlockNumber: parent block number, symFileIdx: parent file index in the block
	};
	//uint32_t fileCount;
	union {
		uint32_t keySeqNumber; //for file entry
		uint32_t prevBlock; //For the entry 0 (".") in the first directory block , it is the block number the first block (pointing to itself)
		                    //for the entry 0 in consecutive blocks of a sub directory only, pointing to the previous block
		uint32_t subDirBlockNumber;//For directory record, point to the first block of the sub directory.
                                //for the entry 1 ("..") in the first block of a sub directory: the first block number of parent dir
	};
}DirRec_t;

#define BLOCK_SIZE_ORDER 8
#define BLOCK_DIR_NUMBER (1<<BLOCK_SIZE_ORDER)

typedef struct _DirBlock{
	DirRec_t fileRecs[BLOCK_DIR_NUMBER];
}DirBlock_t;

#define isExtFile(fp) (fp>=extFiles && fp < (extFiles+256))
void *addExtFile(void *fp);
void delExtFile(void *fp);

void dmInitDirSetting(const char *sandbox, const char *model);
sgx_status_t initDir(const char * dir, sgx_key_128bit_t *pEncKey);
sgx_status_t dmInitDataDir(int stId);
sgx_status_t dmFindDirEntry(const char *dir, const char *name, //const uint32_t fileId,
		uint32_t *keyId, int32_t *dirId, DirRec_t *dirRec, uint32_t *newStId);
sgx_status_t dmGetFreeEntryInCurrentDir(SGX_FILE *fp, DirBlock_t *pBlock, uint32_t *pBlockNumber, uint32_t *pFileIdx);

sgx_status_t dmGetFileNameArray(const char *dir, const char *path, char *rspBuf, size_t maxLen, size_t *pCurLen);
sgx_status_t dmGetDirList(const char *dir, const char *path, char *result, size_t *pLen);

sgx_status_t dmGetFreeDirEntry(SGX_FILE *fp, const char *name, DirBlock_t **pBlock, uint32_t *pBlockNumber, uint32_t *pFileIdx);
sgx_status_t dmGetFreeDirBlock(SGX_FILE *fp, uint32_t *pBlockNumber);

sgx_status_t dmDelDirByDirId(const char *dir, int32_t *dirId, const char *name);
sgx_status_t dmDelDirEntryByName(const char *dir, const char *name);

sgx_status_t dmAddDir(const char *dir, const char *name, const SymLinkInfo_t *symInfo, uint32_t *pKeyId, int32_t *dirId);
sgx_status_t dmSearchPath(SGX_FILE **fp, const char *path, DirBlock_t **pDirBlock, uint32_t *pBlockNumber,
		uint32_t *pFileIdx, const char *dirType, uint32_t *newStId);
sgx_status_t dmSearchFile(SGX_FILE *fp, const char *fileName, DirBlock_t **pDirBlock, uint32_t *pBlockNumber, uint32_t *pFileIdx);

sgx_status_t dmReadRecord(SGX_FILE *file, uint32_t block, uint32_t fileId, DirRec_t *pDirRec);
sgx_status_t dmReadBlock(SGX_FILE *file, uint32_t block, DirBlock_t *pDirBlock);

sgx_status_t dmWriteDataOrModel(const char *dir, const char *name, const uint8_t *buf, size_t len,
		uint32_t smallOrLarge, uint32_t offset);
sgx_status_t dmReadDataOrModel(const char *dir, const char *name, uint8_t *buf, size_t *len);
sgx_status_t dmWriteFileInfo(const char *dirFileName, const char *fileName, const char *infoStr);

sgx_status_t createSymLink(const char *dirType, uint32_t linkFromStId, uint32_t linkToStId, const char *fromName, const char *toName);

int dmRemove(const char *filename);
uint32_t parseDir(const char *dir, char *dirType, size_t maxLen);

sgx_status_t dmSetFileAttr(const char *dirName, uint32_t fileId, uint32_t attr);
sgx_status_t dmUpdateFileSize(const char *dirName, uint32_t fileId, size_t size);

void dmInitDirBlock(DirBlock_t *pBlock);
void dmSetSandbox(const char *sandbox);
bool isGlobalSandbox();

sgx_status_t dmNewSubDir(SGX_FILE *fp, DirRec_t *pRec, uint32_t currBlkNum, uint32_t currFileIdx, uint32_t firstBlkOfCurrDir,
		uint32_t *pNewBlockNumber, DirBlock_t *pBlockBuff);

sgx_status_t dmOpenFileWithStId(const char *name, const char *mode, void **file, uint32_t stId);

extern sgx_thread_mutex_t fileRWLockDir;

#endif /* INCLUDE_DIR_MANAGEMENT_H_ */
