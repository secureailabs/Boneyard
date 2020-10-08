/*
 * util.h
 *
 *  Created on: Oct 3, 2019
 *      Author: accuser
 */

#ifndef GT_ENCLAVE_INCLUDE_UTIL_H_
#define GT_ENCLAVE_INCLUDE_UTIL_H_


#include "cjson.h"
#include "../gt_enclave_t.h"

#ifdef __cplusplus
extern "C"{
#endif

sgx_status_t getJsonNumber(const cJSON *json, const char * id, uint32_t *out);
sgx_status_t getJsonNumberDouble(const cJSON *json, const char * id, uint64_t *out);
sgx_status_t cpJsonString(const cJSON *json, const char * id, char *out, size_t outLen);
char * getJsonStringPtr(const cJSON *json, const char * id);
cJSON *getJsonObject(const cJSON *json, const char * id);
int getJsonStrArr(const cJSON* json, const char* id, char*** ret);
int findKeyIndex(uint8_t* key, char** arr, int size);
void oc_print(const char* location, const char *message);

#ifdef __cplusplus
}
#endif


#define LOCK(mutex_lock) sgx_thread_mutex_lock(mutex_lock)
#define UNLOCK(mutex_lock) sgx_thread_mutex_unlock(mutex_lock)

#endif /* GT_ENCLAVE_INCLUDE_UTIL_H_ */
