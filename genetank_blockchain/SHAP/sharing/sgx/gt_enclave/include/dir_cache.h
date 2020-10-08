/*
 * dir_cache.h
 *
 *  Created on: Oct 23, 2019
 *      Author: accuser
 */

#ifndef GT_ENCLAVE_INCLUDE_DIR_CACHE_H_
#define GT_ENCLAVE_INCLUDE_DIR_CACHE_H_


#include "./dir_management.h"
#include "enclave_def.h"

#define DIR_CACHE_SIZE 16

void initDirCache(const char *dir, bool force);
sgx_status_t getDirCache(DirBlock_t **ppDirBlock, uint32_t blockNo);


#endif /* GT_ENCLAVE_INCLUDE_DIR_CACHE_H_ */
