/*
 * conversion.h
 *
 *  Created on: Nov 19, 2018
 *      Author: accuser
 */

#ifndef GT_ENCLAVE_INCLUDE_CONVERSION_H_
#define GT_ENCLAVE_INCLUDE_CONVERSION_H_

#include <stdlib.h>

void hex2bin(const char *hex, unsigned char *bin, size_t resultBytes);
void bin2hex(const unsigned char *src, size_t inputBytes, char *out);

unsigned char *base64_decode(const char *data,
                             size_t input_length,
                             size_t *output_length);
char *base64_encode(const unsigned char *data,
                    size_t input_length,
                    size_t *output_length);
size_t base64_decode2buff(const char *data, size_t input_length,
		char *decoded_data, size_t max_output_length);

bool isLetter(uint8_t c);
bool isNumber(uint8_t c);
bool isLetterNumber(uint8_t c);
int str2int(char *s, size_t mLen);
char *scanNumber(const char *s, uint32_t *pNum);

size_t getLine(const uint8_t *buf, uint8_t *output, size_t maxLen);

void jsonEscape(uint8_t *buf, size_t *pInSize, size_t maxSize);
void removeDoubleSlash(char *path);
void toUpperCase(char *str, size_t maxLen);
void toLowerCase(char *str, size_t maxLen);

#endif //GT_ENCLAVE_INCLUDE_CONVERSION_H_
