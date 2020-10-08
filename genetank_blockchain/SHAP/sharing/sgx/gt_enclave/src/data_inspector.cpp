/*
 * data_inspector.cpp
 *
 *  Created on: Aug 13, 2018
 *      Author: accuser
 */
#include "gt_enclave_t.h"

#include <stddef.h>
#include "sgx_trts.h"
#include "sgx_error.h"
#include "sgx_tcrypto.h"


#include "data_inspector.h"
#include "dir_management.h"
#include "data_management.h"

#include "mtcsv2R.h"

#include "rvm.h"
#include "conversion.h"

#define DI_MAX_CODE_SIZE 100000
#define DI_MAX_BUFFER_SIZE 100000
#define DI_MAX_NAME_SIZE 1000
#define DI_MAX_COLUMN 26


size_t skipString(const uint8_t *buf, const size_t mLen){
	size_t len=0;
	uint8_t quote = *(buf-1);
	while (true){
		while (quote != *(buf+len) && len<mLen && '\0' != *(buf+len))
			len++;
		if (len>=mLen || '\0' == *(buf+len))
			break;
		if ('\\' == *(buf+len-1))
			len++;
		else if (quote == *(buf+len+1))
			len+=2;
		else
			break;
	}
	return len;
}

size_t getColNames(uint8_t *buf, const size_t mLen, uint8_t *col[], const size_t maxCol){
	size_t len = 0;
	size_t idx = 0;
	while (idx<maxCol){
		while (len<mLen && !isLetter(*(buf+len)) && '\0' != *(buf+len))
			len++;
		if ('"' == *(buf+len-1) || '\'' == *(buf+len-1)) {
			len += skipString(buf+len, mLen-len);
			continue;
		}
		if (('e' == *(buf+len) || 'E' == *(buf+len)) && isLetterNumber(*(buf+len-1))) {
			while (len<mLen && isLetterNumber(*(buf+len)))
				len++;
			continue;
		}
		col[idx] = buf+len;
		if ('\0' == *(buf+len) || (len >= mLen))
			break;
		while (len<mLen && isLetterNumber(*(buf+len)))
			len++;
		*(buf+len)='\0';
		for (int i=0; i<idx; i++){
			if (!strncmp((const char *)(col[i]),(const char *)(col[idx]),DI_MAX_NAME_SIZE)){ //get rid of repeated column.
				idx --;
				break;
			}
		}
		idx++;
	}
	return idx;
}
/*

RVM_equal<-FALSE
equ<-function(x,y){
  if(is.na(x))
    RVM_equal <<- is.na(y)
  else if (is.na(y))
    RVM_equal <<- FALSE
  else
    RVM_equal <<- (x == y)
}

RVM_colStatValue<-c()
RVM_colStatDist<-c()
colStat<-function(x, rows){
  a<-x
  b<-c()
  c<-c()
  i<-0;
  for (j in 1:rows) {
    equ(a[1], a[j])
    if (!RVM_equal || 1 == j) {
      i<-i+1
      b[i]<-a[j]
      c[i]<-0
      for (k in j:rows){
        equ(b[i], a[k])
        if(RVM_equal) {
           c[i] <- c[i]+1
           a[k]<-a[1]
         }
      }
    }
  }
  sum<-0
  for (j in 1:i)
    sum<-sum+c[j]
  if (sum != 0)
  for (j in 1:i)
    c[j]<-c[j]/sum
  RVM_colStatValue<<-b
  RVM_colStatDist<<-c
}

a<-c(1,2,3,4,5,6,7,8,9,10)
b<-c("a","b","c","d","e","f","g","h","i","j")
c<-c("a1","b2","c3","d4","e5","f6","g7","h8","i9","j0")

RVM_GlobalCondResult<-FALSE

f<-function(i){
  a<-a[i]
  b<-b[i]
  c<-c[i]
  result<-is.na(a) || is.na(b) || is.na(c)
  if (result)
  	RVM_GlobalCondResult<<-TRUE
  else
	RVM_GlobalCondResult<<-(a>3 && b!="h" && c!="i9")
}

cnt<-0;
for (i in 1:rows){
  f(i)
  if (RVM_GlobalCondResult){
    cnt<-cnt+1
    a[cnt]<-a[i]
    b[cnt]<-b[i]
    c[cnt]<-c[i]
  }
}

colStat(a, rows)

 */
void getColStatCode(char *codeBuf, size_t len){
	char code[] = "RVM_equal<-FALSE\n\
			equ<-function(x,y){\n\
			  if(is.na(x))\n\
			    RVM_equal <<- is.na(y)\n\
			  else if (is.na(y))\n\
			    RVM_equal <<- FALSE\n\
			  else\n\
			    RVM_equal <<- (x == y)\n\
			}\n\
\n\
			RVM_colStatValue<-c()\n\
			RVM_colStatDist<-c()\n\
			colStat<-function(x, rows){\n\
			  a<-x\n\
			  b<-c()\n\
			  c<-c()\n\
			  i<-0;\n\
			  for (j in 1:rows) {\n\
			    equ(a[1], a[j])\n\
			    if (!RVM_equal || 1 == j) {\n\
			      i<-i+1\n\
			      b[i]<-a[j]\n\
			      c[i]<-0\n\
			      for (k in j:rows){\n\
			        equ(b[i], a[k])\n\
			        if(RVM_equal) {\n\
			           c[i] <- c[i]+1\n\
			           a[k]<-a[1]\n\
			         }\n\
			      }\n\
			    }\n\
			  }\n\
			  sum<-0\n\
			  for (j in 1:i)\n\
			    sum<-sum+c[j]\n\
			  if (sum != 0)\n\
			  for (j in 1:i)\n\
			    c[j]<-c[j]/sum\n\
			  RVM_colStatValue<<-b\n\
			  RVM_colStatDist<<-c\n\
			}\n";
	//run_code(code, NULL,0);
	strncpy(codeBuf,code,len);
}
void genSearchCode(uint8_t *col[], size_t cols, size_t rows, const char *cond, char *code, const size_t mSize){
	snprintf(code, mSize, "RVM_GlobalCondResult<-FALSE\n f<-function(i){\n");
	size_t codeLen = strnlen(code, mSize);
	if (codeLen >= mSize)
		return;
	for (int i=0; i<cols; i++){
		snprintf(code+codeLen, mSize-codeLen, "%s<-%s[i]\n",col[i],col[i]);
		codeLen += strnlen(code+codeLen, mSize-codeLen);
		if (codeLen >= mSize)
				return;
	}
//	snprintf(code+codeLen, mSize-codeLen,"result<-is.na(%s)",col[0]);
//	codeLen += strnlen(code+codeLen, mSize-codeLen);
//	if (codeLen >= mSize)
//			return;
	for (int i=0; i<cols; i++){
		snprintf(code+codeLen, mSize-codeLen,"if (is.na(%s)) %s<-0 \n", col[i], col[i]);
		codeLen += strnlen(code+codeLen, mSize-codeLen);
		if (codeLen >= mSize)
				return;
	}
//	snprintf(code+codeLen, mSize-codeLen,"\n if (result)\n\
//		  RVM_GlobalCondResult<<-FALSE\n\
//		else\n\
//		  RVM_GlobalCondResult<<-(%s)}\n",cond);
	snprintf(code+codeLen, mSize-codeLen, "RVM_GlobalCondResult<<-(%s)}\n",cond);

	codeLen += strnlen(code+codeLen, mSize-codeLen);
	if (codeLen >= mSize)
		return;
	snprintf(code+codeLen, mSize-codeLen,"cnt<-0\n for (i in 1:%u){\n\
			f(i)\n\
			if (RVM_GlobalCondResult){\n\
			  cnt<-cnt+1\n",(unsigned int)rows);
	codeLen += strnlen(code+codeLen, mSize-codeLen);
	if (codeLen >= mSize)
		return;
	for (int i=0; i<cols; i++){
		snprintf(code+codeLen, mSize-codeLen, "%s[cnt]<-%s[i]\n",col[i],col[i]);
		codeLen += strnlen(code+codeLen, mSize-codeLen);
		if (codeLen >= mSize)
				return;
	}
	snprintf(code+codeLen, mSize-codeLen,"}}\n cnt\n\n");
	//codeLen += strnlen(code+codeLen, mSize-codeLen);
	//if (codeLen >= mSize)
	//	return;
	//*(code+codeLen) = '\0';
	return;
}

size_t getColStatResult(char *buffer, size_t mLen, char *code, size_t mCodeLen){
	size_t pos=0;
	snprintf((char *)code, mCodeLen, "length(RVM_colStatValue)\n");
	VM_Result *pRes = run_R_code(code, NULL, 0);
	int cx = pRes->cx-2;
	while (cx>0 && isNumber(*(pRes->result_buf+cx)))
		cx--;
	cx++;
	int valRows = str2int(pRes->result_buf+cx,RESULT_BUF_SIZE);
	snprintf(buffer+pos, mLen-pos, "%d", valRows);
	pos += strnlen(buffer+pos, mLen-pos)+1;
	if (pos>=mLen)
		return pos;
	for (int i=1; i<=valRows; i++){
		snprintf((char *)code, mCodeLen, "RVM_colStatValue[%d]\n",i);
		pRes = run_R_code(code, NULL, 0);
		snprintf(buffer+pos, mLen-pos, "%s", pRes->result_buf);
		pos += strnlen(buffer+pos, mLen-pos)+1;
		if (pos>=mLen)
			break;
		snprintf((char *)code, mCodeLen, "RVM_colStatDist[%d]\n",i);
		pRes = run_R_code(code, NULL, 0);
		snprintf(buffer+pos, mLen-pos, "%s", pRes->result_buf);
		pos += strnlen(buffer+pos, mLen-pos)+1;
		if (pos>=mLen)
			break;
	}
	return pos;
}

sgx_status_t ecall_delete_code(const char *name, size_t nam_len){
	return(dmDelDirEntryByName(modelDir, name));
}

sgx_status_t ecall_getModelList(uint8_t *buffer, size_t bufSize, size_t *outSize){
	*outSize = bufSize;
    return(dmGetDirList(modelDir, NULL, (char *)buffer, outSize));
}

void genColStat(char *code, size_t mCodeLen, char * colName, char *rspBuf, size_t maxLen, size_t *pCurLen){
	size_t curLen = *pCurLen;

	snprintf((char *)code, mCodeLen, "length(RVM_colStatValue)\n");
	VM_Result *pRes = run_R_code(code, NULL, 0);
	int cx = pRes->cx-2;
	while (cx>0 && isNumber(*(pRes->result_buf+cx)))
		cx--;
	cx++;
	int valRows = str2int(pRes->result_buf+cx,RESULT_BUF_SIZE);

	//size_t curLen = *pCurLen;
	snprintf((rspBuf+curLen), maxLen-curLen-1, "{\"name\":\"%s\"", colName);
	curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);

	snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"attrValue\":[");
	curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);

	bool firstAttr=true;

	for (int i=1; i<=valRows; i++){
		snprintf((char *)code, mCodeLen, "RVM_colStatValue[%d]\n",i);
		pRes = run_R_code(code, NULL, 0);
		if (pRes->cx>0)
			pRes->result_buf[pRes->cx-1]='\0';
		if (!strncmp(pRes->result_buf+4,"NA",3))
			strncpy(pRes->result_buf+4, "\"NA\"",5);
		if (firstAttr){
			snprintf((rspBuf+curLen), maxLen-curLen-1, "{\"value\":%s,",pRes->result_buf+4);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
			firstAttr = false;
		} else {
			snprintf((rspBuf+curLen), maxLen-curLen-1, ",{\"value\":%s,",pRes->result_buf+4);
			curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		}
		//snprintf(buffer+pos, mLen-pos, "%s", pRes->result_buf);
		snprintf((char *)code, mCodeLen, "RVM_colStatDist[%d]\n",i);
		pRes = run_R_code(code, NULL, 0);
		if (pRes->cx>0)
			pRes->result_buf[pRes->cx-1]='\0';
		//float ratio;
		//sscanf((const char *)(pRes->result_buf+3),"%f",&ratio);
		snprintf((rspBuf+curLen), maxLen-curLen-1, "\"ratio\":%s}",pRes->result_buf+3);
		curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
		//snprintf(buffer+pos, mLen-pos, "%s", pRes->result_buf);
	}
	snprintf((rspBuf+curLen), maxLen-curLen-1, "]},");
	curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);

	*pCurLen = curLen;
}

sgx_status_t genDataArray(char *dirName, char *fname, char *cond, char *rspBuf, size_t maxLen, size_t *pCurLen){
	sgx_status_t ret;

	if (NULL == fname || '\0' == *fname)
		return SGX_SUCCESS;

	char code[DI_MAX_CODE_SIZE];
	uint8_t rd_buf[DI_MAX_BUFFER_SIZE];

	size_t mLen = DI_MAX_BUFFER_SIZE;

	//ret = dmReadDataFile(fname, rd_buf, &mLen);
	ret = dmReadDataOrModel(dirName, fname, rd_buf, &mLen);
	if (SGX_SUCCESS != ret){
		return SGX_SUCCESS; //return the file list normally even read file failed.
	}

	uint8_t colNameBuf[DI_MAX_NAME_SIZE];
	//uint8_t lineSiz;
	getLine(rd_buf, colNameBuf, DI_MAX_NAME_SIZE);

	uint8_t *col[DI_MAX_COLUMN];
	size_t cols = getFields(colNameBuf, col, DI_MAX_COLUMN);
	//genFieldArrayJson(cols, colNameBuf, col, rspBuf, maxLen, pCurLen);
	if (cols<3) //maybe not a CSV file
		return SGX_SUCCESS;

	uint8_t *colInCond[DI_MAX_COLUMN];
	uint8_t copyCond[DI_MAX_NAME_SIZE];
	if (NULL == cond || '\0' == *cond)
		strncpy((char *)copyCond, "TRUE", DI_MAX_NAME_SIZE);
	else
		strncpy((char *)copyCond, (const char *)cond, DI_MAX_NAME_SIZE-1);

	//uint8_t result[DI_MAX_NAME_SIZE];
	size_t rows;
	size_t position = getDataFrameCode(rd_buf, mLen, fname, code, DI_MAX_CODE_SIZE, &rows);
	run_R_code(code, NULL, 0);

	genSearchCode(col, cols, rows, (const char *)copyCond, code, DI_MAX_CODE_SIZE);
	VM_Result *pRes = run_R_code((char *)code, NULL, 0);
	int cx = pRes->cx-2;
	while (cx>0 && isNumber(*(pRes->result_buf+cx)))
		cx--;
	cx++;
	int cnt = str2int(pRes->result_buf+cx, RESULT_BUF_SIZE);

	//loadColStatFunc();
	size_t curLen = *pCurLen;
	snprintf((rspBuf+curLen), maxLen-curLen-1, ",\"attributes\":[");
	curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
	size_t valueLen=0;
	for(int i=0;i<cols;i++){
		getColStatCode(code, DI_MAX_CODE_SIZE);
		size_t len = strnlen(code, DI_MAX_CODE_SIZE);
		snprintf((char *)code+len, DI_MAX_CODE_SIZE-len-1, "\ncolStat(%s, %d)\n", col[i],cnt);
		run_R_code(code, NULL, 0);
		genColStat(code, DI_MAX_CODE_SIZE, (char *)col[i], rspBuf, maxLen, &curLen);
	}
	curLen--; //remove last comma
	snprintf((rspBuf+curLen), maxLen-curLen-1, "]");
	curLen += strnlen((char *)(rspBuf+curLen), maxLen-curLen);
	*pCurLen = curLen;

	return SGX_SUCCESS;
}

sgx_status_t getDataList(const char *fileName, const char *cond,
	char *buffer, size_t maxLen, size_t *pFileLen, size_t *pAttrLen, size_t *pValueLen, size_t *pRows){

	sgx_status_t ret;
    size_t fileLen = maxLen;

    ret =  dmGetDirList(dataDir, NULL, buffer, &fileLen);
	if (SGX_SUCCESS != ret)
		return ret;

	char code[DI_MAX_CODE_SIZE];
	uint8_t rd_buf[DI_MAX_BUFFER_SIZE];

	size_t mLen = DI_MAX_BUFFER_SIZE;

	if (NULL == fileName || '\0' == *fileName){
		*pFileLen = fileLen;
		*pAttrLen = 0;
		*pValueLen = 0;
		*pRows = 0;
		return SGX_SUCCESS;
	}

	ret = dmReadDataFile(fileName, rd_buf, &mLen);
	if (SGX_SUCCESS != ret){
		*pFileLen = fileLen;
		*pAttrLen = 0;
		*pValueLen = 0;
		*pRows = 0;
		return SGX_SUCCESS; //return the file list normally even read file failed.
	}

	uint8_t colNameBuf[DI_MAX_NAME_SIZE];
	//uint8_t lineSiz;
	getLine(rd_buf, colNameBuf, DI_MAX_NAME_SIZE);

	uint8_t *col[DI_MAX_COLUMN];
	size_t cols = getFields(colNameBuf, col, DI_MAX_COLUMN);

	size_t attrLen = 0;
	for (int i=0; i<cols; i++){
		snprintf(buffer+fileLen+attrLen, maxLen-fileLen-attrLen, "%s", col[i]);
		attrLen += strnlen(buffer+fileLen+attrLen, maxLen-fileLen-attrLen) + 1;
	}

	uint8_t *colInCond[DI_MAX_COLUMN];
	uint8_t copyCond[DI_MAX_NAME_SIZE];
	if (NULL == cond || '\0' == *cond)
		strncpy((char *)copyCond, "TRUE", DI_MAX_NAME_SIZE);
	else {
		strncpy((char *)copyCond, (const char *)cond, DI_MAX_NAME_SIZE-1);
	    size_t colsInCond = getColNames(copyCond, DI_MAX_NAME_SIZE, colInCond, DI_MAX_COLUMN);

		for (int i=0; i<colsInCond; i++){
			int j;
			for (j=0; j<cols; j++)
				if (!strncmp((const char *)colInCond[i], (const char *)col[j], DI_MAX_NAME_SIZE))
					break;
			if (j == cols){ //column name not found in the condition
				snprintf(buffer+fileLen+attrLen, maxLen-fileLen-attrLen,"No row meets the condition.");
				size_t valueLen = strnlen(buffer+fileLen+attrLen, maxLen-fileLen-attrLen) + 2;
				*(buffer+fileLen+attrLen+valueLen) = '\0';
				*pFileLen = fileLen;
				*pAttrLen = attrLen;
				*pValueLen = valueLen;
				*pRows = 1;
				return SGX_SUCCESS;
			}
		}
		strncpy((char *)copyCond, (const char *)cond, DI_MAX_NAME_SIZE-1);
	}

	uint8_t result[DI_MAX_NAME_SIZE];
	size_t rows;
	size_t position = getDataFrameCode(rd_buf, mLen, fileName, code, DI_MAX_CODE_SIZE, &rows);
	run_R_code(code, (char *)result, DI_MAX_NAME_SIZE);

	genSearchCode(col, cols, rows, (const char *)copyCond, code, DI_MAX_CODE_SIZE);
	VM_Result *pRes = run_R_code((char *)code, (char *)result, DI_MAX_NAME_SIZE);
	int cx = pRes->cx-2;
	while (cx>0 && isNumber(*(pRes->result_buf+cx)))
		cx--;
	cx++;
	int cnt = str2int(pRes->result_buf+cx, RESULT_BUF_SIZE);

	size_t valueLen=0;
	for(int i=0;i<cols;i++){
		snprintf((char *)code, DI_MAX_CODE_SIZE,"colStat(%s, %d)\n", col[i],cnt);
		run_R_code(code, (char *)result, 0);
		valueLen += getColStatResult(buffer+fileLen+attrLen+valueLen, maxLen-fileLen-attrLen-valueLen, code, DI_MAX_CODE_SIZE);
	}

	*(buffer+fileLen+attrLen+valueLen) = '\0';
	*pFileLen = fileLen;
	*pAttrLen = attrLen;
	*pValueLen = valueLen;
	*pRows = cnt;
	return SGX_SUCCESS;
}

sgx_status_t uploadData(const char *filename, size_t fSize, const uint8_t *buf, size_t bufLen,
		char *receipt, size_t rcvSize){
	sgx_status_t ret = dmWriteDataFile(filename, buf, bufLen);
	snprintf(receipt, rcvSize, "0x2342342393594976458");
	return ret;
}

sgx_status_t getTabList(const char *data_name, char *result, size_t *pLen){
	return SGX_SUCCESS;
}

/*
 * Get one attribute's distribution on its values
 */
sgx_status_t getDataAttrDistList(const char *dataName, char *result, size_t *pLen){
	return SGX_SUCCESS;
}

/*
 * conditions: condition string, can include multiple boolean expressions separated by commas.
 * supports:
 *   1) Compares operators: ">", "<", "=". Format:"op"+operand1+operand2,
 *      the operands can be attribute name or instant value.
 *   2) When op is "=", the instant value can be a condition string(support wild casts:"*", "."
 *   3) Boolean expression (op: AND, OR, NOT). Format: "op"+operand1+operand2(option),
 *      the operands are the sequence numbers of the expressions whose values will be used.
 *   4) The evaluation of the expressions will be done for once, no iteration.
 *   5) The last expression will be the final value of the conditions
 *
 * Use R condition expression?
 *
 */
sgx_status_t getAttributeStatList(const char *dataName, const char * conditions,
		char *result, size_t *pLen){
	return SGX_SUCCESS;
}
