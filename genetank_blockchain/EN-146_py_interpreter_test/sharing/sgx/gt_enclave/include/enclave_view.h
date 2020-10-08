/*
 * enclave_view.h
 *
 *  Created on: Feb 23, 2019
 *      Author: accuser
 */
#ifndef INCLUDE_ENCLAVE_VIEW_H_
#define INCLUDE_ENCLAVE_VIEW_H_

#include "string"
#include <stdio.h>
#include <stdlib.h>
#include "sgx_utils.h"
#include "session.h"
//#include "sgx_tae_service.h"

#define MAX_NAME_LEN 256

class TreeElement{
public:
	TreeElement(const char *moduleName, int32_t dirIdInput, uint32_t keyIdInput, uint32_t stId,
		uint64_t timestamp, size_t file_size){
		children = sibling = NULL;
		module = NULL;
		dirId = dirIdInput;
		this->stId = stId;
		symLinkStId = 0;
		keyId = keyIdInput;
		tm = timestamp;
		size = file_size;
		if (NULL != moduleName){
			int len = strnlen(moduleName, MAX_NAME_LEN);
			if (0 == len)
				return;
			module = (char *)malloc(len+1);
			strncpy(module, moduleName, len+1);
		}
	}

	~TreeElement(){
		if (NULL != module)
			free(module);
		if (NULL != children)
			delete children;
		if (NULL != sibling)
			delete sibling;
	}

	int32_t getDirId(){
		return dirId;
	}

	uint32_t getKeyId(){
		return keyId;
	}

	char *getModule(){
		return module;
	}

	TreeElement *getChild(){
		return children;
	}

	TreeElement *getSibling(){
		return sibling;
	}

	void setChild(TreeElement *element){
		children = element;
	}

	void setSibling(TreeElement *element){
		sibling = element;
	}

	void addSibling(TreeElement& element){
		element.setSibling(sibling);
		sibling = &element;
	}

	void addChild(TreeElement& element){
		if (NULL == children){
			element.setSibling(NULL);
			children = &element;
		} else {
			children->addSibling(element);
		}
	}

	uint32_t stId;
	uint32_t symLinkStId;
	int32_t dirId;
	uint32_t keyId;
	uint64_t tm;
	size_t size;
private:
	char *module;
	TreeElement *children;
	TreeElement *sibling;
};

long getFileList(int stId, char * rspBuf, size_t maxLen, const char *path);
long tree2JsonString(TreeElement& tree, char *buf, long bufSize, uint32_t maxLayer, bool readDataInfo);
TreeElement *getDirTree(char *dir, const char *virDir, char *path);
long getEnclaveInfoJson(uint32_t stId, char * buf, long bufSize, const char *path);

long getElementInfoJson(uint32_t stId, char * buf, long bufSize, const char *path);
#endif /* INCLUDE_ENCLAVE_VIEW_H_ */
