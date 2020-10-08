/*
 * mtcsv.h
 *
 *  Created on: Aug 10, 2018
 *      Author: accuser
 */

#ifndef GT_ENCLAVE_INCLUDE_MTCSV2R_H_
#define GT_ENCLAVE_INCLUDE_MTCSV2R_H_


#define MTCSV_MAX_NAME_LENGTH 200
#define MTCSV_MAX_COLUMN 20

#include "string.h"
#include <stdint.h>
#include <stddef.h> /* for size_t */

size_t countRows(const uint8_t *buf);
size_t getLine(const uint8_t *buf, uint8_t *output, size_t maxLen);
size_t getFields(uint8_t *buf, uint8_t *value[], size_t maxFields);
bool isNumber(uint8_t *str);
size_t getDataFrameCode(const uint8_t *buffer, size_t buf_len, const char *defaultName, char *code, size_t max_code_buf, size_t *pRows);

#endif /* GT_ENCLAVE_INCLUDE_MTCSV2R_H_ */
