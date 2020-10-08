#pragma once

#include "enclave_def.h"
#include "../gt_enclave_t.h"

//#ifdef _MSC_VER
//# include <Shlobj.h>
//#else
# include <unistd.h>
//# define MAX_PATH 256
//#endif

typedef struct _StakeholderInfo_t{
	uint32_t infoTag; //must be all '1'
	uint32_t type; //ROLE_DONOR, ROLE_DATA_SCIENTIST, ROLE_DATA_SCIENTIST_GROUP
	uint32_t hostDataDonor; //valid for ROLE_DATA_SCIENTIST, ROLE_DATA_SCIENTIST_GROUP
	uint32_t hostDSGroup; //Data Scientist Group, valid for data scientist
	uint64_t padding1, padding2; //must be 0
}StakeholderInfo_t;

typedef struct _StakeholderRec_t{
	uint32_t seqNumber;
	uint8_t ethAddr[ETH_ADDR_SIZE];
	//uint8_t dataFileName[FILE_NAME_SIZE];
	//uint32_t fileId;
	union{
		sgx_sha256_hash_t dataHash;
		StakeholderInfo_t info;
	};
	sgx_key_128bit_t encKey;
}StakeholderRec_t;

void getFullPathFileName(char *fullName, size_t maxSize, const char *fileName);
void getFileNameOnDisk(char *fullName, size_t maxSize, const char *virDir, uint32_t fileIdInVirDir);
void getFileName(const char *dirName, int32_t dirId, char *fname, size_t maxNameLen);
void getInfoFileName(const char *dirName, int32_t dirId, char *fname, size_t maxNameLen);
void getInfoFileNameOnDisk(char *fullName, size_t maxSize, const char *virDir, uint32_t fileIdInVirDir);
void getTmpFileNameOnDisk(char *fullName, size_t maxSize, const char *tmpFileName);

sgx_status_t dmInit(const char *home);
//sgx_status_t dmInitDataFile(const char *dataFile//, const sgx_sha256_hash_t *pwHash
//		);
sgx_status_t dmSealWrite(SGX_FILE *fp, size_t size, const uint8_t *data);
sgx_status_t dmSealRead(SGX_FILE *fp, uint32_t *pDataSize, uint8_t *data);

sgx_status_t dmInit(const char *home);
sgx_status_t dmInitStakeFile(const char *dataFile);
sgx_status_t dmGetPublicKey(sgx_ec256_public_t *pubk);
sgx_status_t dmGetPrivateKey(sgx_ec256_private_t *privk);

sgx_status_t dmAppendStake(StakeholderRec_t *rec);
sgx_status_t dmReadStake(StakeholderRec_t *pRec);
sgx_status_t dmSearchRole(uint8_t *ethAddr, uint32_t *pStId);

//sgx_status_t dmAddRunnerEthAddr(const eth_addr_t *p_ethAddr);
//sgx_status_t dmAddHostEthAddr(const eth_addr_t *p_ethAddr);
int dmAddUserEthAddr(const eth_addr_t *p_ethAddr, int stId);
sgx_status_t dmReadEthAddr(int stId, eth_addr_t *p_ethAddr);
sgx_status_t dmUpdateStakeHash(const StakeholderRec_t *rec);

sgx_status_t dmWriteFile(const char *filename, sgx_key_128bit_t *key, const uint8_t *data, size_t len);
sgx_status_t dmAppendFile(const char *filename, sgx_key_128bit_t *key, const uint8_t *data, size_t len);
sgx_status_t dmWriteLargeFile(const char *filename, sgx_key_128bit_t *key, const uint8_t *data, size_t len, uint32_t offset);
sgx_status_t dmReadFile(const char *filename, sgx_key_128bit_t *key, uint8_t *data, size_t *pBufLen);

sgx_status_t dmWriteDataFile(const char *name, const uint8_t *buf, size_t len);
sgx_status_t dmWriteModelFile(const char *name, const uint8_t *buf, size_t len);
sgx_status_t dmReadDataFile(const char *name, uint8_t *buf, size_t *len);
sgx_status_t dmReadModelFile(const char *name, uint8_t *buf, size_t *len);

sgx_status_t dmReadWithId(const uint32_t stakeholderId, const char *dataVsModel,
		const char *name, uint8_t *buf, size_t *len);
sgx_status_t dmWriteWithId(const uint32_t stakeholderId, const char *dataVsModel,
		const char *name, const uint8_t *buf, size_t len);
