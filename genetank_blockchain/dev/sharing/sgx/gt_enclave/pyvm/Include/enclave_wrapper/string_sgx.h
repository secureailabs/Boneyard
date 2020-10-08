/*
 * string_sgx.h
 *
 *  Created on: Dec 14, 2018
 *      Author: jingwei
 */

#ifndef GT_ENCLAVE_PYVM_INCLUDE_ENCLAVE_WRAPPER_STRING_SGX_H_
#define GT_ENCLAVE_PYVM_INCLUDE_ENCLAVE_WRAPPER_STRING_SGX_H_

#include <string.h>

char* strcat(char* dest, const char* src);
int strcmp(const char* str1, const char* str2);
char* strcpy(char* dest, const char* src);
//char* strdup(const char* s);

#endif /* GT_ENCLAVE_PYVM_INCLUDE_ENCLAVE_WRAPPER_STRING_SGX_H_ */
