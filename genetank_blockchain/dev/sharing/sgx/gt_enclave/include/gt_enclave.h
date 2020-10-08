/*
 * gt_enclave.h
 *
 *  Created on: Dec 8, 2018
 *      Author: accuser
 */

#ifndef INCLUDE_GT_ENCLAVE_H_
#define INCLUDE_GT_ENCLAVE_H_

#include <stdint.h>
#include "../gt_enclave_t.h"

void run_code(const char *code, char *result, size_t result_size, const char* code_type);

//#define GET_PRED_BUF_SIZE 10000
//sgx_status_t get_prediction(const uint8_t *buffer, size_t len, uint8_t *result, size_t resSize);

#endif /* INCLUDE_GT_ENCLAVE_H_ */
