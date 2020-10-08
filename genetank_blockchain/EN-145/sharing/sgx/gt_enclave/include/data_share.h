/*
 * data_share.h
 *
 *  Created on: Sep 26, 2019
 *      Author: accuser
 */

#ifndef GT_ENCLAVE_INCLUDE_DATA_SHARE_H_
#define GT_ENCLAVE_INCLUDE_DATA_SHARE_H_
#include "../gt_enclave_t.h"
//#include "sgx_tae_service.h"

#define DATA_SHARE_INFO_VERSION 2
/*
 * Sharing information file format:
 * 1) header: numbers of sharing
 * 2) Each sharing's information record
 * 3) Data information JSON
 */
typedef struct _ShareInfoFileHeader{
	uint32_t version;
	uint32_t shares;
}ShareInfoFileHeader_t;

typedef struct _ShareInfoRec{
	uint32_t destStId; //shared target stakeholder ID
	uint64_t tm;
	uint64_t startTime;
	uint64_t endTime;
}ShareInfoRec_t;

sgx_status_t shareData2Guest(uint32_t stId, char *srcPath, char *dstPath, char *dataName, char *guestPubKey, uint64_t startTime,
		uint64_t endTime, char *dataInfo, bool isAnonymized);
uint8_t *readShareDataInfo(char *infoFileName, sgx_key_128bit_t *encKey, uint32_t symLinkStId, size_t *pLen);
bool validShareInfo(char *infoFileName, uint32_t encKeyId, uint32_t destStId, uint64_t tm);
sgx_status_t saveDataInfo(char *srcPath, char *dataName, char *dataInfo, uint32_t stId);

#endif /* GT_ENCLAVE_INCLUDE_DATA_SHARE_H_ */
