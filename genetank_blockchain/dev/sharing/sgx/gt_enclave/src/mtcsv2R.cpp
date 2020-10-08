/*
 * mtcsv2R.cpp
 *
 *  Created on: Aug 10, 2018
 *      Author: accuser
 */

#include "mtcsv2R.h"

#include <stdint.h>
#include <stdio.h> /* for size_t */

size_t countRows(const uint8_t *buf){
	size_t rows=0;

	while(true){
		if ('\n' == *buf)
			return (rows);
		while ('\n' != *buf && '\0' != *buf)
			buf++;
		if ('\0' == *buf)
			return (rows);
		buf++;rows++;
	}
}

size_t getMyLine(const uint8_t *buf, uint8_t *output, size_t maxLen){
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

size_t getFields(uint8_t *buf, uint8_t *value[], size_t maxFields){
	value[0] = buf;
	size_t idx=maxFields;
	for (int i=1;i<maxFields;i++){
		value[i]=value[i-1];
		if ('"' == *value[i])
			value[i]++; //skill the first quote(")
		while (',' != *(value[i])){
			if ('\0' == *(value[i])) {
				if (maxFields == idx)
					idx = i;
				break;//while
			}
			value[i] += 1;
		}
		if ('"' == *(value[i]-1))
			*(value[i]-1) = '\0'; //remove last quote (")
		if (',' == *(value[i])){
			*(value[i]) = '\0';
			value[i] += 1;
		}
		if (idx<maxFields)
			break;
	}
	return idx;
}

bool isRNumber(uint8_t *str){
	if ('N' == *str && 'A' == *(str+1) && '\0' == *(str+2))
		return true;
	if ('-' == *str || '+' == *str)
		str++;
	while (*str >= '0' && *str <= '9')
		str++;
	if ('.' == *str){
		str++;
		if (*str < '0' || *str > '9')
			return false;
		while (*str >= '0' && *str <='9')
			str++;
	}
	if ('e' == *str){
		str++;
		if ('-' == *str || '+' == *str)
			str++;
		if (*str < '0' || *str > '9')
			return false;
		while (*str >= '0' && *str <='9')
			str++;
	}
	return (0 == *str);
}

size_t getDataFrameCode(const uint8_t *buffer, size_t buf_len, const char *defaultName, char *code, size_t max_code_buf, size_t *pRows){
	uint8_t names[MTCSV_MAX_NAME_LENGTH];
	uint8_t *df_name, *col[MTCSV_MAX_COLUMN];

	*code = '\0';
	/*
	 * Get data frame name
	 */
	df_name = names;
	size_t size = 0, name_pos, lineSize;
	while ('\n' == *(buffer+size)) //skip newlines
		size++;
	if (size>=buf_len || 0 == *(buffer+size))
		return(size);
	if ('&' != *(buffer+size)){
		strncpy((char *)df_name, (char *)defaultName, MTCSV_MAX_NAME_LENGTH);
		name_pos = strnlen((const char *)df_name, MTCSV_MAX_NAME_LENGTH);
	} else {
		size++; //skip the '&'
		lineSize = getMyLine((uint8_t *)(buffer+size), df_name, MTCSV_MAX_NAME_LENGTH);
		name_pos = lineSize;
		size += lineSize;
		if (size>=buf_len || 0 == *(buffer+size))
			return(size);
		size++;
	}
	name_pos++;

	/*
	 * Get column names
	 */
	col[0]=names+name_pos;
	lineSize = getMyLine((uint8_t *)(buffer+size), col[0], MTCSV_MAX_NAME_LENGTH-name_pos);

	uint8_t idx = getFields(col[0], col, MTCSV_MAX_COLUMN);

	size += lineSize;
	name_pos +=lineSize;
	if (size>=buf_len || 0 == *(buffer+size))
		return(size);
	size++;name_pos++;

	size_t rows_offset = size;
	int offset = 0; //output code offset
	for (int i=0;i<idx;i++){
		uint8_t *row[MTCSV_MAX_COLUMN];

		size = rows_offset;

		int rowCnt=0;

		snprintf((char *)(code+offset),max_code_buf-offset,"%s<-c(",col[i]);
		size_t sz = strnlen((const char *)(code+offset),max_code_buf-offset);
		offset += sz;
		bool firstRow = true;
		char naVal[]="NA";
		while (true){
			char *valPtr;
			row[0] = names+name_pos;
			lineSize = getMyLine((uint8_t *)(buffer+size),row[0],MTCSV_MAX_NAME_LENGTH-name_pos);
			if (0 == lineSize)
				break;

			getFields(row[0], row, idx);
			char quote[2]={'"',0};
			if (isRNumber(row[i]) || '"' == *(row[i]) || '\0' == *(row[i]))
				quote[0]='\0';
			if ('\0' == *(row[i]))
				valPtr=naVal;
			else
				valPtr=(char *)(row[i]);
			if (firstRow){
				snprintf((char *)(code+offset),max_code_buf-offset,"%s%s%s",quote,valPtr,quote);
				firstRow = false;
			} else
				snprintf((char *)(code+offset),max_code_buf-offset,",%s%s%s",quote,valPtr,quote);
			size_t sz = strnlen((const char *)(code+offset),max_code_buf-offset);
			offset += sz;

			rowCnt++;

			size += lineSize;
			if (size>=buf_len || 0 == *(buffer+size))
				break;
			else
				size++;
		}
		snprintf((char *)(code+offset),max_code_buf-offset,")\n");
		sz = strnlen((const char *)(code+offset),max_code_buf-offset);
		offset += sz;
		*pRows = rowCnt;
	}

	snprintf((char *)(code+offset),max_code_buf-offset, "length(%s)\n",col[0]);
	return (size);
}


