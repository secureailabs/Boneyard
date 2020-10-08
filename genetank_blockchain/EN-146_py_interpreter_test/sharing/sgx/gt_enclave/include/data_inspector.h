/*
 * data_inpector.h
 *
 *  Created on: Aug 13, 2018
 *      Author: accuser
 */

#ifndef GT_ENCLAVE_INCLUDE_DATA_INSPECTOR_H_
#define GT_ENCLAVE_INCLUDE_DATA_INSPECTOR_H_

#include "sgx_trts.h"
#include <stdio.h> /* for size_t */
//#include "string.h"

//#include <stddef.h>

sgx_status_t getDataList(const char *fileName, size_t fname_size,
	const char *attrName, size_t aName_size,
        const char *cond, size_t cSize,
	char *buffer, size_t *pFileLen, size_t *pAttrLen, size_t *pValueLen, size_t maxLen);

sgx_status_t genDataArray(char *dirName, char *fname, char *cond, char *rspBuf, size_t maxLen, size_t *pCurLen);

#endif /* GT_ENCLAVE_INCLUDE_DATA_INSPECTOR_H_ */
