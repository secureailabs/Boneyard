/*
 * util.cpp
 *
 *  Created on: Oct 3, 2019
 *      Author: accuser
 */
#include "cjson.h"
#include "util.h"
#include "gt_enclave_t.h"
#include "string.h"
#include "conversion.h"

sgx_status_t getJsonNumber(const cJSON *json, const char * id, uint32_t *out){
	if (!cJSON_HasObjectItem(json, id)){
		return (sgx_status_t)-1;
	}
	cJSON *sIdObj = cJSON_GetObjectItem(json, id);
	if (!cJSON_IsNumber(sIdObj)){
		return (sgx_status_t)-1;
	}
	*out = sIdObj->valuedouble;
	return SGX_SUCCESS;
}

sgx_status_t getJsonNumberDouble(const cJSON *json, const char * id, uint64_t *out){
	if (!cJSON_HasObjectItem(json, id)){
		return (sgx_status_t)-1;
	}
	cJSON *sIdObj = cJSON_GetObjectItem(json, id);
	if (!cJSON_IsNumber(sIdObj)){
		return (sgx_status_t)-1;
	}
	*out = sIdObj->valuedouble;
	return SGX_SUCCESS;
}

sgx_status_t cpJsonString(const cJSON *json, const char * id, char *out, size_t outLen){
	if (!cJSON_HasObjectItem(json, id)){
		return (sgx_status_t)-1;
	}
	cJSON *sIdObj = cJSON_GetObjectItem(json, id);
	if (!cJSON_IsString(sIdObj)){
		return (sgx_status_t)-1;
	}
	strncpy((char *)out, sIdObj->valuestring, outLen);
	return SGX_SUCCESS;
}

char * getJsonStringPtr(const cJSON *json, const char * id){
	if (!cJSON_HasObjectItem(json, id)){
		return NULL;
	}
	cJSON *sIdObj = cJSON_GetObjectItem(json, id);
	if (!cJSON_IsString(sIdObj)){
		return NULL;
	}
	return sIdObj->valuestring;
}

cJSON *getJsonObject(const cJSON *json, const char * id){
	if (!cJSON_HasObjectItem(json, id)){
		return NULL;
	}
	cJSON *sIdObj = cJSON_GetObjectItem(json, id);
	return sIdObj;
}

int getJsonStrArr(const cJSON* json, const char* id, char*** ret){

	if (!cJSON_HasObjectItem(json, id)){
		return NULL;
	}
	cJSON *arr = cJSON_GetObjectItem(json, id);
	int size = cJSON_GetArraySize(arr);

	*ret = (char**)malloc(size*sizeof(char*));

	for(int i=0;i<size;i++){
		cJSON *str = cJSON_GetArrayItem(arr, i);
		char* tmp = cJSON_GetStringValue(str);
		(*ret)[i] = tmp;
	}

	return size;
}

int findKeyIndex(uint8_t* key, char** arr, int size){
	//char strKey[130];
	//bin2hex(key, 64, strKey);
	//strKey[128]='\0';
	int i;
	for(i=0;i<size;i++){
		uint8_t binKey[64];
		//if(strncmp(strKey, arr[i], strlen(strKey))==0)
		hex2bin(arr[i], binKey, 64);
		if (!memcmp(key, binKey, 64)) //using bin comparing is much reliable than text comparing
			break;
	}
	if (i<size)
		return i;
	return -1;
}

void oc_print(const char* location, const char *message){
        sgx_status_t ret;
        char buf[10240];
        size_t len = strnlen(location, 128);
        strncpy(buf, location, len);
        buf[len] = '\0';
        ocall_print(&ret, buf, len+1);
        if (NULL != message){
            len = strnlen(message, 10238);
             strncpy(buf, message, len);
        } else
        	len = 0;
        buf[len] = '\n';
        buf[len+1] = '\0';
        ocall_print(&ret, buf, len+2);
}
