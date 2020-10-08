//file: data_management.cpp Author: Shifa Version:0.1

#include "data_management.h"
#include "dir_management.h"
#include "secp256k1.h"
#include "sgx_trts.h"
#include "sgx_tcrypto.h"
#include "sgx_tprotected_fs.h"
#include "sgx_thread.h"
#include "gt_enclave_t.h"
#include "crypto.h"

static char enclaveHome[MAX_PATH]={'\0'};

sgx_thread_mutex_t writerLockData = SGX_THREAD_MUTEX_INITIALIZER;
sgx_thread_mutex_t readerLockData = SGX_THREAD_MUTEX_INITIALIZER;
sgx_thread_mutex_t fileRWLockData = SGX_THREAD_MUTEX_INITIALIZER;
//char stakeFileName[MAX_PATH]={'\0'};

void getFullPathFileName(char *fullName, size_t maxSize, const char *fileName){
	snprintf(fullName,maxSize,"%s/%s",enclaveHome, fileName);
}

void getFileName(const char *dirName, int32_t dirId, char *fname, size_t maxNameLen){
	snprintf(fname, maxNameLen, "%s.%d", dirName, dirId);
}

void getInfoFileName(const char *dirName, int32_t dirId, char *fname, size_t maxNameLen){
	snprintf(fname, maxNameLen, "%s.%d.info", dirName, dirId);
}

void getFileNameOnDisk(char *fullName, size_t maxSize, const char *virDir, uint32_t fileIdInVirDir){
	snprintf(fullName,maxSize,"%s/%s/%s.%d",enclaveHome,ENCLACE_FILE_DIR, virDir, fileIdInVirDir);
}

void getInfoFileNameOnDisk(char *fullName, size_t maxSize, const char *virDir, uint32_t fileIdInVirDir){
	snprintf(fullName,maxSize,"%s/%s/%s.%d.info",enclaveHome,ENCLACE_FILE_DIR, virDir, fileIdInVirDir);
}

void getTmpFileNameOnDisk(char *fullName, size_t maxSize, const char *tmpFileName){
	snprintf(fullName,maxSize,"%s/tmp/%s",enclaveHome, tmpFileName);
}

sgx_status_t dmSealWrite(SGX_FILE *fp, size_t size, const uint8_t *data){
	sgx_status_t ret;

	//Seal the data before storing them to a file.
	//1. Use sgx_calc_sealed_data_size to calculate the number of bytes to allocate for the sgx_sealed_data_t structure.
	//2. Allocate memory for the sgx_sealed_data_t structure.
	//3. Call sgx_seal_data to perform sealing operation

	uint8_t data_buffer[BUFFER_SIZE];
	uint32_t sealed_data_size;

	if (NULL == fp)
		return (sgx_status_t)(-1);

	sealed_data_size = sgx_calc_sealed_data_size(0,size);
	if (sealed_data_size > BUFFER_SIZE) {
		return (sgx_status_t)(-1);
	}

	ret = sgx_seal_data(0, NULL, size, data, sealed_data_size, (sgx_sealed_data_t *)data_buffer);
	if (SGX_SUCCESS != ret) {
		return (sgx_status_t)(-1);
	}

	size_t  count = sgx_fwrite(data_buffer, sealed_data_size, 1, fp);
	if (1 != count) {
		return (sgx_status_t)(-1);
	}

	return SGX_SUCCESS;

}

/*
 * Return SGX_SUCCESS with the data read in *data and data size in *pDataSize
 * If return non SGX_SUCCESS, and *pDataSize == 0 means encounter end of file
 * Otherwise there is an error happened.
 */
sgx_status_t dmSealRead(SGX_FILE *fp, uint32_t *pDataSize, uint8_t *data){
	uint8_t data_buffer[BUFFER_SIZE];
	uint32_t max_buf_size=*pDataSize;
	uint32_t sealed_data_size = sgx_calc_sealed_data_size(0,*pDataSize);

	if (NULL == fp)
		return (sgx_status_t)(-1);

	if (sealed_data_size > BUFFER_SIZE) {
		return (sgx_status_t)(-1);
	}
	size_t count = sgx_fread(data_buffer, sealed_data_size, 1, fp);
	if (1 != count) {
		*pDataSize = 0; //indicate end of file
		return (sgx_status_t)(-1);
	}

	*pDataSize = sgx_get_encrypt_txt_len((sgx_sealed_data_t *)data_buffer);
	if (*pDataSize>max_buf_size) {
		return (sgx_status_t)(-1);
	}

	uint32_t additional_MACtext_length = 0, decrypted_text_length=*pDataSize;
	sgx_status_t ret = sgx_unseal_data((sgx_sealed_data_t *)data_buffer, NULL,
		&additional_MACtext_length, data, &decrypted_text_length);
	if (SGX_SUCCESS != ret || decrypted_text_length != *pDataSize) {
		return (sgx_status_t)(-1);
	}

	return SGX_SUCCESS;
}

sgx_status_t dmGetPublicKey(sgx_ec256_public_t *pubk){
	sgx_status_t ret=SGX_SUCCESS;
	char stakeFileName[MAX_PATH]={'\0'};
	snprintf(stakeFileName,MAX_PATH,"%s/%s",enclaveHome,ENCLAVE_DATA_FILE);
	SGX_FILE* file = sgx_fopen_auto_key(stakeFileName, "r");
	if (NULL == file)
		return (sgx_status_t)(-1);

	uint32_t offset = 0;//sgx_calc_sealed_data_size(0,sizeof(sgx_sha256_hash_t));
	offset += sizeof(sgx_ec256_private_t); //sgx_calc_sealed_data_size(0,sizeof(sgx_ec256_private_t));
	if (SGX_SUCCESS != sgx_fseek(file,offset,SEEK_CUR)){
		sgx_fclose(file);
		return (sgx_status_t)(-1);
	}
	//uint32_t sealed_data_size;
	uint32_t data_size=sizeof(sgx_ec256_public_t);
	//sealed_data_size = sgx_calc_sealed_data_size(0,sizeof(sgx_ec256_public_t));

	//ret = dmSealRead(file, &data_size, (uint8_t *)pubk);
	if (1 != sgx_fread(pubk, data_size, 1, file))
		ret = (sgx_status_t)(-1);
	else
		ret = SGX_SUCCESS;

	sgx_fclose(file);
	//if (sizeof(sgx_ec256_public_t) != data_size)
	//  return (sgx_status_t)(-1);
	return ret;
}

sgx_status_t dmGetPrivateKey(sgx_ec256_private_t *privk){
	sgx_status_t ret = SGX_SUCCESS;
	char stakeFileName[MAX_PATH]={'\0'};
	snprintf(stakeFileName,MAX_PATH,"%s/%s",enclaveHome,ENCLAVE_DATA_FILE);

	SGX_FILE* file = sgx_fopen_auto_key(stakeFileName, "r");
	if (NULL == file)
		return (sgx_status_t)(-1);

	uint32_t offset = 0;//sgx_calc_sealed_data_size(0,sizeof(sgx_sha256_hash_t));
	//sgx_fseek(file,offset,SEEK_CUR);
	//uint32_t sealed_data_size;
	uint32_t data_size=sizeof(sgx_ec256_private_t);
	//sealed_data_size = sgx_calc_sealed_data_size(0,sizeof(sgx_ec256_private_t));

	//ret = dmSealRead(file, &data_size, (uint8_t *)privk);
	if (1 != sgx_fread(privk, data_size, 1, file))
		ret = (sgx_status_t)(-1);
	else
		ret = SGX_SUCCESS;

	sgx_fclose(file);

	//if (sizeof(sgx_ec256_private_t) != data_size)
	//  return (sgx_status_t)(-1);
	return ret;
}

sgx_status_t dmInit(const char *home){
	//sgx_sha256_hash_t hash;
	char stakeFileName[MAX_PATH]={'\0'};

	//if ('\0' != enclaveHome[0]) //duplicated initialization is not allowed.
	//	return (sgx_status_t)-1;

	strncpy(enclaveHome, home, strnlen(home, MAX_PATH-1)+1);

	//if (strnlen(enclaveHome,MAX_PATH)+2+strnlen(ENCLACE_FILE_DIR,MAX_PATH)+FILE_NAME_SIZE-1>=MAX_PATH-1)
	//	return (sgx_status_t)(-1);
	snprintf(stakeFileName,MAX_PATH,"%s/%s",enclaveHome,ENCLAVE_DATA_FILE);

	SGX_FILE* file = sgx_fopen_auto_key(stakeFileName, "r");
	if (NULL == file) {
		if (SGX_SUCCESS != dmInitStakeFile((const char *)stakeFileName))
			return (sgx_status_t)(-1);
	} else
		sgx_fclose(file);
	sgx_ec256_public_t pubk;
	sgx_status_t ret = dmGetPublicKey(&pubk);
	if (SGX_SUCCESS != ret){
		return (sgx_status_t)(-1);
	}

	return SGX_SUCCESS;
}

sgx_status_t dmInitStakeFile(const char *dataFile//, const sgx_sha256_hash_t *pwHash
		) {
	sgx_ec256_private_t enclave_private;
	size_t keyLen=65;
	uint8_t genPubkey[65];

	if (SGX_SUCCESS != genK1KeyPair(&enclave_private, genPubkey, &keyLen))
		return (sgx_status_t)-1;
	SGX_FILE* file = sgx_fopen_auto_key(dataFile, "w");
	if (NULL == file)
		return (sgx_status_t)(-1);

	if (1 != sgx_fwrite((const void *)&enclave_private, sizeof(sgx_ec256_private_t), 1, file)){
		sgx_fclose(file);
		return (sgx_status_t)(-1);
	}

	if (1 != sgx_fwrite(genPubkey+1, 64, 1, file)){
		sgx_fclose(file);
		return (sgx_status_t)(-1);
	}

	sgx_fclose(file);
	return SGX_SUCCESS;
}

sgx_status_t dmAppendStake(StakeholderRec_t *rec){
	sgx_status_t ret;
	rec->isDeleted = false;
	LOCK(&writerLockData);
	char stakeFileName[MAX_PATH]={'\0'};
	snprintf(stakeFileName,MAX_PATH,"%s/%s",enclaveHome,ENCLAVE_DATA_FILE);
	SGX_FILE* file = sgx_fopen_auto_key(stakeFileName, "r+");
	if (NULL == file){
		UNLOCK(&writerLockData);
		return (sgx_status_t)(-1);
	}
	uint32_t offset = 0;//sgx_calc_sealed_data_size(0,sizeof(sgx_sha256_hash_t));
	offset += sizeof(sgx_ec256_private_t); //sgx_calc_sealed_data_size(0,sizeof(sgx_ec256_private_t));
	offset += sizeof(sgx_ec256_public_t); //sgx_calc_sealed_data_size(0,sizeof(sgx_ec256_public_t));
	uint32_t size = sizeof(StakeholderRec_t); //sgx_calc_sealed_data_size(0,sizeof(StakeholderRec_t));
	if (SGX_SUCCESS != sgx_fseek(file,0,SEEK_END)){
		sgx_fclose(file);
		UNLOCK(&writerLockData);
		return (sgx_status_t)(-1);
	}
	uint64_t fLen = sgx_ftell(file);
	if (-1 == fLen){
		sgx_fclose(file);
		UNLOCK(&writerLockData);
		return (sgx_status_t)(-1);
	}
	if (rec->seqNumber < 0x7fffffff){
		offset += size*rec->seqNumber;
		if (offset != fLen){
			sgx_fclose(file);
			UNLOCK(&writerLockData);
			return (sgx_status_t)(-1);
		}
	} else {
		rec->seqNumber = (fLen-offset)/size;
	}
	if (1 != sgx_fwrite((const uint8_t *)rec, sizeof(StakeholderRec_t), 1, file)){
		sgx_fclose(file);
		UNLOCK(&writerLockData);
		return (sgx_status_t)(-1);
	}

	sgx_fclose(file);
	UNLOCK(&writerLockData);
	return SGX_SUCCESS;
}

/*
 * The hash can be changed from all 0's to a normal hash, or from normal hash to all 1's
 * All 1's hash means the data has been deleted.
 */
sgx_status_t dmUpdateStakeHash(const StakeholderRec_t *pRec){
	LOCK(&writerLockData);
	sgx_status_t ret;
	StakeholderRec_t r_rec;
	char stakeFileName[MAX_PATH]={'\0'};
	if(r_rec.stId == 0) {
		r_rec.seqNumber = 0;
	} else {
		r_rec.seqNumber = r_rec.role + (3 * (r_rec.stId - 1));
	}
	snprintf(stakeFileName,MAX_PATH,"%s/%s",enclaveHome,ENCLAVE_DATA_FILE);
	SGX_FILE* file = sgx_fopen_auto_key(stakeFileName, "r+");
	if (NULL == file){
		UNLOCK(&writerLockData);
		return (sgx_status_t)(-1);
	}

	uint32_t offset = 0;//sgx_calc_sealed_data_size(0,sizeof(sgx_sha256_hash_t));
	offset += sizeof(sgx_ec256_private_t); //sgx_calc_sealed_data_size(0,sizeof(sgx_ec256_private_t));
	offset += sizeof(sgx_ec256_public_t); //sgx_calc_sealed_data_size(0,sizeof(sgx_ec256_public_t));
	uint32_t size = sizeof(StakeholderRec_t); //sgx_calc_sealed_data_size(0,sizeof(StakeholderRec_t));
	offset += size*pRec->seqNumber;
	if (SGX_SUCCESS != sgx_fseek(file,offset,SEEK_SET)){
		sgx_fclose(file);
		UNLOCK(&writerLockData);
		return (sgx_status_t)(-1);
	}
	//uint32_t sealed_data_size;
	uint32_t data_size=sizeof(StakeholderRec_t);
	//sealed_data_size = sgx_calc_sealed_data_size(0,sizeof(StakeholderRec_t));

	//ret = dmSealRead(file, &data_size, (uint8_t *)&r_rec);
	//if (SGX_SUCCESS == ret){
	if (1 == sgx_fread(&r_rec, data_size, 1, file)){
		if (r_rec.stId != pRec->stId || r_rec.role != pRec->role){
			sgx_fclose(file);
			UNLOCK(&writerLockData);
			return (sgx_status_t)(-1);
		}
		//strncpy((char *)(r_rec.stakeFileName), (const char *)(rec->stakeFileName), FILE_NAME_SIZE-1);
		uint8_t allZero=1;
		for (int i=0;i<SGX_SHA256_HASH_SIZE;i++)
			if ('\0' != r_rec.dataHash[i]){
				allZero=0;
				break;
			}
		if (!allZero)
			for (int i=0;i<SGX_SHA256_HASH_SIZE;i++)
				if (0xff != pRec->dataHash[i]){ //If the hash is none-zero, then it can only be changed to all ones
					sgx_fclose(file);
					UNLOCK(&writerLockData);
					return SGX_SUCCESS;
				}
		memcpy(r_rec.dataHash, pRec->dataHash, SGX_SHA256_HASH_SIZE);
	}

	if (SGX_SUCCESS != sgx_fseek(file,offset,SEEK_SET)){
		sgx_fclose(file);
		UNLOCK(&writerLockData);
		return (sgx_status_t)(-1);
	}

	//if (SGX_SUCCESS != dmSealWrite(file, sizeof(StakeholderRec_t), (const uint8_t *)&r_rec)){
	if (1 != sgx_fwrite((const uint8_t *)&r_rec, sizeof(StakeholderRec_t), 1, file)){
		sgx_fclose(file);
		UNLOCK(&writerLockData);
		return (sgx_status_t)(-1);
	}

	sgx_fclose(file);
	UNLOCK(&writerLockData);
	return SGX_SUCCESS;
}

/*
* Update's stake records
* 1- When a host is removed the stakerecord's isDeleted is set to true which indicates
* 	 that the record can be over written.
* 2- When a host is updated the stakeholder record is read and overwritten by the new host
* 	 credentials.
*/
sgx_status_t dmUpdateStake(StakeholderRec_t *newRec, bool removeStake) {
	sgx_status_t ret;
	StakeholderRec_t oldRec;

	if(newRec->stId < 0 || newRec->role < 0) {
		return (sgx_status_t)(-1);
	}
	// read the existing stakeholder dmReadRecord
	oldRec.seqNumber = -1;
	oldRec.stId = newRec->stId;
	oldRec.role = newRec->role;
	ret = dmReadStake(&oldRec);
	if(SGX_SUCCESS != ret) {
		return (sgx_status_t)(-1);
	}
	// updating an existing stakeholder record
	if(!removeStake) {
		if(!oldRec.isDeleted) {
			return (sgx_status_t)(-1);
		}
		newRec->isDeleted = false;
	}
	else { // for removing a stakeholder record, mark it as isDeleted
		if(memcmp(oldRec.ethAddr, newRec->ethAddr, 20)){
			return (sgx_status_t)(-1);
		}
	}

	if(oldRec.stId != newRec->stId || oldRec.role != newRec->role) {
		return (sgx_status_t)(-1);
	}

	LOCK(&writerLockData);
	char stakeFileName[MAX_PATH]={'\0'};
	snprintf(stakeFileName,MAX_PATH,"%s/%s",enclaveHome,ENCLAVE_DATA_FILE);
	SGX_FILE* file = sgx_fopen_auto_key(stakeFileName, "r+");
	if (NULL == file){
		UNLOCK(&writerLockData);
		return (sgx_status_t)(-1);
	}
	uint32_t offset = 0;//sgx_calc_sealed_data_size(0,sizeof(sgx_sha256_hash_t));
	offset += sizeof(sgx_ec256_private_t); //sgx_calc_sealed_data_size(0,sizeof(sgx_ec256_private_t));
	offset += sizeof(sgx_ec256_public_t); //sgx_calc_sealed_data_size(0,sizeof(sgx_ec256_public_t));
	uint32_t size = sizeof(StakeholderRec_t); //sgx_calc_sealed_data_size(0,sizeof(StakeholderRec_t));
	newRec->seqNumber = oldRec.seqNumber;
	offset += size*newRec->seqNumber;
	if (SGX_SUCCESS != sgx_fseek(file,offset, SEEK_SET)){
		sgx_fclose(file);
		UNLOCK(&writerLockData);
		return (sgx_status_t)(-1);
	}

	if (1 != sgx_fwrite((const uint8_t *)newRec, sizeof(StakeholderRec_t), 1, file)){
		sgx_fclose(file);
		UNLOCK(&writerLockData);
		return (sgx_status_t)(-1);
	}
	sgx_fclose(file);
	UNLOCK(&writerLockData);
	return SGX_SUCCESS;
}

/*
 * Runner stakeholder record is often used for accessing the directory file. So, to improve performance, put it in
 * a cache. But the cache could reduce security while under cache attacks.
 */
StakeholderRec_t runnerRec;
bool runnerRecReady=false;
/*
 * Input: pRec->seqNumber, the stakeholder with the number is to be read.
 *   If seqNumber is big, e.g. (uint32_t)(-1), it will read the last record
 * Output:
 *   *pRec: the record read.
 */
sgx_status_t dmReadStake(StakeholderRec_t *pRec){
	sgx_status_t ret;
	int64_t fret;
	char stakeFileName[MAX_PATH]={'\0'};

	if (0 == pRec->stId && runnerRecReady){
		memcpy(pRec, &runnerRec, sizeof(StakeholderRec_t));
		return SGX_SUCCESS;
	}
	snprintf(stakeFileName,MAX_PATH,"%s/%s",enclaveHome,ENCLAVE_DATA_FILE);
	//lock_local
	SGX_FILE* file = sgx_fopen_auto_key(stakeFileName, "r");
	//unlock_local
	if (NULL == file){
		return (sgx_status_t)(-1);
	}
	uint32_t offset = 0;//sgx_calc_sealed_data_size(0,sizeof(sgx_sha256_hash_t));
	offset += sizeof(sgx_ec256_private_t); //sgx_calc_sealed_data_size(0,sizeof(sgx_ec256_private_t));
	offset += sizeof(sgx_ec256_public_t); //sgx_calc_sealed_data_size(0,sizeof(sgx_ec256_public_t));
	uint32_t size = sizeof(StakeholderRec_t); //sgx_calc_sealed_data_size(0,sizeof(StakeholderRec_t));

	if((int32_t)(pRec->seqNumber) < 0) {
		if((int32_t)(pRec->stId) == 0) {
			pRec->seqNumber = 0;
			offset += size*pRec->seqNumber;
		} else if((int32_t)(pRec->stId) > 0 && (int32_t)(pRec->role) > 0) {
			pRec->seqNumber = pRec->role + (3 * (pRec->stId - 1));
			offset += size*pRec->seqNumber;
		} else {
			fret = sgx_fseek(file, 0, SEEK_END);
			fret = sgx_ftell(file);
			if (fret == -1){
				sgx_fclose(file);
				return (sgx_status_t)(-1);
			}
			offset = fret-size;
		}
	} else {
		offset += size*pRec->seqNumber;
	}

	fret = sgx_fseek(file, offset, SEEK_SET);

	if (SGX_SUCCESS != fret){
		sgx_fclose(file);
		//UNLOCK(&writerLockData);
		return (sgx_status_t)(-1);
	}

	//uint32_t sealed_data_size;
	uint32_t data_size = sizeof(StakeholderRec_t);
	uint32_t seqNumber = pRec->seqNumber;
	//sealed_data_size = sgx_calc_sealed_data_size(0,sizeof(StakeholderRec_t));

	//ret = dmSealRead(file, &data_size, (uint8_t *)pRec);
	size_t retLen = sgx_fread((void *)pRec, data_size, 1, file);
	sgx_fclose(file);
	if (1 == retLen) {
		if ((int32_t)seqNumber >= 0 && seqNumber != pRec->seqNumber){
			return (sgx_status_t)(-1);
		}
		if (0 == seqNumber){
			memcpy(&runnerRec, pRec, sizeof(StakeholderRec_t));
			runnerRecReady = true;
		}
		return SGX_SUCCESS;
	}
	return (sgx_status_t)(-1);
}

sgx_status_t dmSearchRole(uint8_t *ethAddr, uint32_t *pStId, uint32_t *pRoleId){
	sgx_status_t ret;
	StakeholderRec_t sRec;

	char stakeFileName[MAX_PATH]={'\0'};
	snprintf(stakeFileName,MAX_PATH,"%s/%s",enclaveHome,ENCLAVE_DATA_FILE);
	SGX_FILE* file = sgx_fopen_auto_key(stakeFileName, "r");
	if (NULL == file){
		return (sgx_status_t)(-1);
	}
	uint32_t offset = 0;//sgx_calc_sealed_data_size(0,sizeof(sgx_sha256_hash_t));
	offset += sizeof(sgx_ec256_private_t); //sgx_calc_sealed_data_size(0,sizeof(sgx_ec256_private_t));
	offset += sizeof(sgx_ec256_public_t); //sgx_calc_sealed_data_size(0,sizeof(sgx_ec256_public_t));
	uint32_t size = sizeof(StakeholderRec_t); //sgx_calc_sealed_data_size(0,sizeof(StakeholderRec_t));
	uint32_t seqNumber;
	if (-1 != *pStId){
		seqNumber = (*pRoleId) + (3 * ((*pStId) - 1));
		offset += size * seqNumber;
		if (SGX_SUCCESS != sgx_fseek(file, offset, SEEK_SET))
			return (sgx_status_t)-1;
		if (1 == sgx_fread(&sRec, sizeof(StakeholderRec_t), 1, file)){
			if ((sRec.isDeleted) || memcmp(sRec.ethAddr, ethAddr, 20)){
				sgx_fclose(file);
				return (sgx_status_t)-1;
			}
		} else {
			sgx_fclose(file);
			return (sgx_status_t)-1;
		}
	} else {
		if (SGX_SUCCESS != sgx_fseek(file, offset+0*size, SEEK_SET))
			return (sgx_status_t)-1;
		//*pStId = -1;
		int32_t seqNum=0;
		while (true){
			if (1 == sgx_fread(&sRec, sizeof(StakeholderRec_t), 1, file)){
				if ((!sRec.isDeleted) && !memcmp(sRec.ethAddr, ethAddr, 20)){
					*pStId = seqNum;
					*pRoleId = sRec.role;
					break;
				}
			} else{
				if (sgx_feof(file))
					break;
				sgx_fclose(file);
				return (sgx_status_t)-1;
			}
			seqNum++;
		}
	}

	sgx_fclose(file);
	return (SGX_SUCCESS);
}
/*
sgx_status_t dmAddHostEthAddr(const eth_addr_t *p_ethAddr){
	StakeholderRec_t sRec;
	memcpy(sRec.ethAddr, p_ethAddr, sizeof(eth_addr_t));
	memset(sRec.dataHash,'\0',SGX_SHA256_HASH_SIZE);
	sgx_status_t ret = sgx_read_rand((unsigned char *)&(sRec.encKey), sizeof(sgx_key_128bit_t));
	if (SGX_SUCCESS != ret)
		return (sgx_status_t)(-1);
	for (int i=1; i<4; i++) {
		sRec.seqNumber = i;
		ret = dmAppendStake(&sRec);
		if (SGX_SUCCESS != ret)
			return (sgx_status_t)(-1);
	}
	return ret;
}
*/
int dmAddUserEthAddr(const eth_addr_t *p_ethAddr, int stId, uint32_t role){ // stId = -1 means adding a new/unknown guest user
	StakeholderRec_t sRec;
	sgx_status_t ret;
	sRec.seqNumber = -1;
	sRec.stId = stId;
	sRec.role = role;

	if (stId >= 0){
		ret = dmReadStake(&sRec);
		if (SGX_SUCCESS == ret){
			if(sRec.isDeleted) {	// overwrite the deleted record
				memcpy(sRec.ethAddr,p_ethAddr,ETH_ADDR_LEN);
				if( dmUpdateStake(&sRec, false) != SGX_SUCCESS) {
					return -1;
				}
				return sRec.seqNumber;
			}
			else if ((!sRec.isDeleted) && !memcmp(sRec.ethAddr,p_ethAddr->addr,ETH_ADDR_LEN)){
				return 0;
			} else {
				return -1;
			}
		}
		sRec.stId = stId;
		sRec.role = role;
	} else {
		ret = dmSearchRole((uint8_t *)p_ethAddr, (uint32_t *)&stId, &role);
		if (SGX_SUCCESS == ret && stId >= 0){
			return stId;
		}
	}
	memcpy(sRec.ethAddr,p_ethAddr,ETH_ADDR_LEN);
	if (sRec.stId == 0) {
		sRec.seqNumber = 0;
	} else {
		sRec.seqNumber = sRec.role + (3 * (sRec.stId - 1));
	}
	ret = sgx_read_rand((unsigned char *)sRec.encKey, sizeof(sgx_key_128bit_t));
	if (SGX_SUCCESS != ret){
		return -1;
	}
	//ret = sgx_read_rand((unsigned char *)sRec.stakeFileName, FILE_NAME_SIZE);
	//if (SGX_SUCCESS != ret)
	//	return (sgx_status_t)(-1);
	ret = dmAppendStake(&sRec);
	if (SGX_SUCCESS != ret){
		return -1;
	}
	return sRec.seqNumber;
}

sgx_status_t dmReadEthAddr(const int32_t stId, eth_addr_t *p_ethAddr){
	StakeholderRec_t rec;
	rec.seqNumber = stId;
	sgx_status_t ret = dmReadStake(&rec);
	if (SGX_SUCCESS != ret || rec.isDeleted){
		return (sgx_status_t)-1;
	}

	memcpy(p_ethAddr, rec.ethAddr, ETH_ADDR_SIZE);
	return SGX_SUCCESS;
}
//uint32_t freeDirEntry=BLOCK_DIR_NUMBER;


sgx_status_t dmWriteFile(const char *filename, sgx_key_128bit_t *key, const uint8_t *data, size_t len){
	char dataFileName[MAX_PATH];

	snprintf(dataFileName,MAX_PATH,"%s/%s/%s", enclaveHome, ENCLACE_FILE_DIR, filename);

	SGX_FILE* file = sgx_fopen(dataFileName, "w", key);
	if (NULL == file){
		return (sgx_status_t)(-1);
	}
	if (1 != sgx_fwrite(data, len, 1, file)){
		sgx_fclose(file);
		return (sgx_status_t)(-1);
	}

	sgx_fclose(file);
	return SGX_SUCCESS;
}

sgx_status_t dmAppendFile(const char *filename, sgx_key_128bit_t *key, const uint8_t *data, size_t len){
	char dataFileName[MAX_PATH];

	snprintf(dataFileName,MAX_PATH,"%s/%s/%s", enclaveHome, ENCLACE_FILE_DIR, filename);

	SGX_FILE* file = sgx_fopen(dataFileName, "r+", key);
	if (NULL == file){
		return (sgx_status_t)(-1);
	}
	if (SGX_SUCCESS != sgx_fseek(file,0,SEEK_END)){
		sgx_fclose(file);
		return (sgx_status_t)(-1);
	}
	if (1 != sgx_fwrite(data, len, 1, file)){
		sgx_fclose(file);
		return (sgx_status_t)(-1);
	}

	sgx_fclose(file);
	return SGX_SUCCESS;
}

sgx_status_t dmWriteLargeFile(const char *filename, sgx_key_128bit_t *key, const uint8_t *data, size_t len, uint32_t offset){
	char dataFileName[MAX_PATH];

	snprintf(dataFileName,MAX_PATH,"%s/%s/%s", enclaveHome, ENCLACE_FILE_DIR, filename);

	SGX_FILE* file = sgx_fopen(dataFileName, "a", key);
	if (NULL == file){
		return (sgx_status_t)(-1);
	}
	if (SGX_SUCCESS != sgx_fseek(file,offset,SEEK_SET)){
		sgx_fclose(file);
		return (sgx_status_t)(-1);
	}
	if (1 != sgx_fwrite(data, len, 1, file)){
		sgx_fclose(file);
		return (sgx_status_t)(-1);
	}
	sgx_fclose(file);
	return SGX_SUCCESS;
}

sgx_status_t dmReadFile(const char *filename, sgx_key_128bit_t *key, uint8_t *data, size_t *pBufLen){
	char dataFileName[MAX_PATH];
	size_t bufSize = *pBufLen, possibleLen=0;

	snprintf(dataFileName,MAX_PATH,"%s/%s/%s", enclaveHome, ENCLACE_FILE_DIR, filename);

	SGX_FILE* file = sgx_fopen(dataFileName, "r", key);
	if (NULL == file){
		return (sgx_status_t)(-1);
	}

	*pBufLen = sgx_fread(data, 1, *pBufLen, file);

	if( *pBufLen != bufSize) {
		data[(*pBufLen)++] = '\0';
	}

	sgx_fclose(file);
	return SGX_SUCCESS;
}

/*
 * Input: dir: the directory file name, can be either "model.dir" or "data.dir"
 *   name: name of the data/model
 *   buf: the buffer of the data to be written
 *   len: the size of the data to be written
 */
sgx_status_t dmWriteWithId(const uint32_t stakeholderId, const char *dataVsModel,
		const char *name, const uint8_t *buf, size_t len){
	char dirName[128];
	snprintf(dirName,127,"%s%d.dir",dataVsModel,stakeholderId);
	return(dmWriteDataOrModel(dirName, name, buf, len, 0,0));
}

sgx_status_t dmReadWithId(const uint32_t stakeholderId, const char *dataVsModel,
		const char *name, uint8_t *buf, size_t *len){
	char dirName[128];
	snprintf(dirName,127,"%s%d.dir",dataVsModel,stakeholderId);
	return(dmReadDataOrModel(dirName, name, buf, len));
}

sgx_status_t dmReadDataFile(const char *name, uint8_t *buf, size_t *len){
	return(dmReadDataOrModel(dataDir, name, buf, len));
}
