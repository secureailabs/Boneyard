/*
 * conversion.cpp
 *
 *  Created on: Nov 19, 2018
 *      Author: accuser
 */
#include <stdlib.h>
#include <stdint.h>
#include "string"
#include "cstring"

char nible2hex(char c){
	c = c & 0x0f;
	if (c<10)
		return '0'+c;
	else
		return 'a'+c-10;
}


//void bin2hex(const unsigned char *src, size_t len, char *out){
//	for (int i=0;i<len;i++){
//		snprintf(out, 3, "%02x",src[i]);
//		out+=2;
//	}
//}

void bin2hex(const unsigned char *src, size_t binLen, char *out){
	for (int i=0;i<binLen;i++){
		//snprintf(out,2,"%02X",src[i]);
		char l,h;
		l = src[i];
		h = l>>4;
		*out = nible2hex(h);
		*(out+1) = nible2hex(l);
		out+=2;
	}
}


void hex2bin(const char *hex, unsigned char *bin, size_t resultBytes){
	if ('0'==*hex && ('x'==*(hex+1) || 'X'==*(hex+1)))
		hex+=2;
	for(int i=0; i<resultBytes; i++){
		bin[i]=0;
		for (int j=0;j<2;j++){
			if (*hex>='0' && *hex<='9')
				bin[i]+=*hex-'0';
			else if (*hex>='a' && *hex<='f')
				bin[i]+=*hex-'a'+10;
			else if (*hex>='A' && *hex<='F')
				bin[i]+=*hex-'A'+10;
			if (0==j)
				bin[i] = bin[i]<<4;
			hex+=1;
		}
	}
}

void toUpperCase(char *str, size_t maxLen){
	size_t cnt =0;
	while (*str != '\0' && cnt < maxLen){
		if (*str >= 'a' && *str <= 'z')
			*str = *str -'a' + 'A';
		str++;
	}
}

void toLowerCase(char *str, size_t maxLen){
	size_t cnt =0;
	while (*str != '\0' && cnt < maxLen){
		if (*str >= 'A' && *str <= 'Z')
			*str = *str -'A' + 'a';
		str++;
	}
}

static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                '4', '5', '6', '7', '8', '9', '+', '/'};
static char *decoding_table = NULL;
static int mod_table[] = {0, 2, 1};


void build_decoding_table() {

    decoding_table = (char *)malloc(256);

    for (int i=0; i < 255; i++)
    	decoding_table[i] = 0;
    for (int i = 0; i < 64; i++)
        decoding_table[(unsigned char) encoding_table[i]] = i;
}


void base64_cleanup() {
    free(decoding_table);
}

char *base64_encode(const unsigned char *data,
                    size_t input_length,
                    size_t *output_length) {

    *output_length = 4 * ((input_length + 2) / 3);

    char *encoded_data = (char *)malloc(*output_length+1);
    if (encoded_data == NULL) return NULL;

    for (int i = 0, j = 0; i < input_length;) {

        uint32_t octet_a = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_b = i < input_length ? (unsigned char)data[i++] : 0;
        uint32_t octet_c = i < input_length ? (unsigned char)data[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        encoded_data[j++] = encoding_table[(triple >> 3 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 2 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 1 * 6) & 0x3F];
        encoded_data[j++] = encoding_table[(triple >> 0 * 6) & 0x3F];
    }

    for (int i = 0; i < mod_table[input_length % 3]; i++)
        encoded_data[*output_length - 1 - i] = '=';

    encoded_data[*output_length]='\0';

    return encoded_data;
}

size_t base64_decode2buff(const char *data, size_t input_length,
		char *decoded_data, size_t max_output_length){
	size_t output_length;
    if (input_length % 4 != 0)
    	return 0;

    output_length = input_length / 4 * 3;
    if (data[input_length - 1] == '=')
    	(output_length)--;
    if (data[input_length - 2] == '=')
    	(output_length)--;

    if (output_length>max_output_length)
    	return 0;

    if (decoding_table == NULL)
    	build_decoding_table();

    for (int i = 0, j = 0; i < input_length;) {

        uint32_t sextet_a = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_b = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_c = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_d = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];

        uint32_t triple = (sextet_a << 3 * 6)
        + (sextet_b << 2 * 6)
        + (sextet_c << 1 * 6)
        + (sextet_d << 0 * 6);

        if (j < output_length) decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
        if (j < output_length) decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
        if (j < output_length) decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
    }
    return output_length;
}

unsigned char *base64_decode(const char *data,
                             size_t input_length,
                             size_t *output_length) {
    if (input_length % 4 != 0)
    	return NULL;

    *output_length = input_length / 4 * 3;
    if (data[input_length - 1] == '=')
    	(*output_length)--;
    if (data[input_length - 2] == '=')
    	(*output_length)--;

    unsigned char *decoded_data = (unsigned char *)malloc(*output_length);
    if (decoded_data == NULL)
    	return NULL;

    if (decoding_table == NULL)
    	build_decoding_table();

    for (int i = 0, j = 0; i < input_length;) {

        uint32_t sextet_a = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_b = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_c = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];
        uint32_t sextet_d = data[i] == '=' ? 0 & i++ : decoding_table[data[i++]];

        uint32_t triple = (sextet_a << 3 * 6)
        + (sextet_b << 2 * 6)
        + (sextet_c << 1 * 6)
        + (sextet_d << 0 * 6);

        if (j < *output_length) decoded_data[j++] = (triple >> 2 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 1 * 8) & 0xFF;
        if (j < *output_length) decoded_data[j++] = (triple >> 0 * 8) & 0xFF;
    }

    return decoded_data;
}

bool isLetter(uint8_t c){
	return((c>='a'&&c<='z') || (c>='A' && c<='Z'));
}

bool isNumber(uint8_t c){
	return(c>='0' && c<='9');
}

bool isLetterNumber(uint8_t c){
	return (isLetter(c) || (c>='0' && c<='9') || (c == '_'));
}

int str2int(char *s, size_t mLen){
	int i=0, val=0;
	while (i<mLen){
		if (isNumber(*(s+i)))
			val=val*10+(int)(*(s+i)-'0');
		else
			break;
		i++;
	}
	return val;
}


char *scanNumber(const char *path, uint32_t *pNum){
	*pNum = 0;
	char c = *path;
	while (c>='0' && c<='9'){
		*pNum = *pNum * 10 + c-'0';
		path += 1;
		c = *path;
	}
	return (char *)path;
}

size_t getLine(const uint8_t *buf, uint8_t *output, size_t maxLen){
	size_t len=0;
	while(len<maxLen){
		*output = *buf;
		if ('\0' == *buf)
			return(len);
		if ('\n' == *buf){
			*output = '\0';
			return(len);
		}
		if ('\r' == *buf){
			*output = '\0';
			if ('\n' == *(buf+1)){
				len++;
			}
			return(len);
		}
		output++; buf++; len++;
	}
	return (len);
}

void jsonEscape(uint8_t *buf, size_t *pInSize, size_t maxSize){
	uint8_t *tmp = (uint8_t *)malloc(maxSize);
	if (tmp == NULL)
		return;
	size_t i, j=0;
	for (i=0;i<*pInSize;i++){
		char c = (char)*(buf+i);
		//char c_1 = (char)*(buf+i-1);
		if (('\0' == c) || (j>=(maxSize-2)) )
			break;
		switch (c) {
		case '\n':
			*(tmp+j)='\\';
			*(tmp+j+1)='n';
			break;
		case '\r':
			*(tmp+j)='\\';
			*(tmp+j+1)='r';
			break;
		case '\t':
			*(tmp+j)='\\';
			*(tmp+j+1)='t';
			break;
		case '\\':{
			char c1 = (char)*(buf+i+1);
			*(tmp+j)='\\';
			*(tmp+j+1)='\\';
			/*if (c1 == '\"' || c1 == '\'' || c1 == '\\'){
				*(tmp+j+1) = c1;
				i++;
			} else {
				*(tmp+j+1)='\\';
			}*/
			break;
		}
		case '\"':
			*(tmp+j)='\\';
			*(tmp+j+1)='\"';
			break;
		//case '\'':
		//	*(tmp+j)='\\';
		//	*(tmp+j+1)='\'';n
		//	break;
		default:
			*(tmp+j)=c;
			j--;
			break;
		}
		j+=2;
	}
	*(tmp+j)='\0';
	strncpy((char *)buf, (char *)tmp, j+1);
	*pInSize = j;
	free(tmp);
}

void jsonEscapeNoQuote(uint8_t *buf, size_t *pInSize, size_t maxSize){
	uint8_t *tmp = (uint8_t *)malloc(maxSize);
	if (tmp == NULL)
		return;
	size_t i, j=0;
	for (i=0;i<*pInSize;i++){
		char c = (char)*(buf+i);
		//char c_1 = (char)*(buf+i-1);
		if (('\0' == c) || (j>=(maxSize-2)) )
			break;
		switch (c) {
		case '\n':
			*(tmp+j)='\\';
			*(tmp+j+1)='n';
			break;
		case '\r':
			*(tmp+j)='\\';
			*(tmp+j+1)='r';
			break;
		case '\t':
			*(tmp+j)='\\';
			*(tmp+j+1)='t';
			break;
		case '\\':
			*(tmp+j)='\\';
			*(tmp+j+1)='\\';
			break;
		//case '\"':
		//	*(tmp+j)='\\';
		//	*(tmp+j+1)='\"';
		//	break;
		//case '\'':
		//	*(tmp+j)='\\';
		//	*(tmp+j+1)='\'';
		//	break;
		default:
			*(tmp+j)=c;
			j--;
			break;
		}
		j+=2;
	}
	*(tmp+j)='\0';
	strncpy((char *)buf, (char *)tmp, j+1);
	*pInSize = j;
	free(tmp);
}

void removeDoubleSlash(char *path){
	if (NULL == path)
		return;
	while ('\0' != *path){
		if ('/' == *path){
			if ('/' == *(path+1)){
				char *p = path+1;
				while ('\0' != *p){
					*p = *(p+1);
					p++;
				}
			} else
				path++;
		} else
			path++;
	}
}


