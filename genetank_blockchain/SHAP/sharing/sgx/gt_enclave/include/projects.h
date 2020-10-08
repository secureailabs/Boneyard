/*
 * projects.h
 *
 *  Created on: Sep 18, 2019
 *      Author: slowloris
 */

#ifndef GT_ENCLAVE_INCLUDE_PROJECTS_H_
#define GT_ENCLAVE_INCLUDE_PROJECTS_H_

#include "enclave_def.h"
#include "sgx_utils.h"
#include "stddef.h"
#include "string"
#include "queue.h"
#include "cluster.h"

class Projects{
public:
	char* code;
	char* name;
	Projects* next;
	uint32_t stakeHolderId;

	Projects(){
		next = NULL;
		code = NULL;
		name = NULL;
		stakeHolderId = 0;
	}

	Projects(const char* code, const char* name, uint32_t stakeHolderId){
		size_t codeLen=strnlen(code, 1024*1024);
		this->code = (char *)malloc(codeLen+1);
		if (NULL != this->code){
			memcpy(this->code, code, codeLen+1);
		}

		size_t nameLen=strnlen(name, 1024);
		this->name = (char *)malloc(nameLen+1);
		if (NULL != this->name){
			memcpy(this->name, name, nameLen+1);
		}
		this->stakeHolderId = stakeHolderId;
		next = NULL;
	}
	~Projects(){
		if (code != NULL)
			free(code);
		if (name != NULL)
			free(name);
	}

	sgx_status_t addToProjList(Projects** head);

	static sgx_status_t deletFromProjList(Projects** head, char* name, uint32_t stakeHolderId);
	static Projects* searchInProjList(Projects** head, char* name, uint32_t stakeHolderId);
};

void cleanUpHubOnKill();

extern Projects* projectsList;
extern Queue subResultQueue;

//extern Cluster* activeSubEnclaveList;
//extern char fedProjectName[];

#endif /* GT_ENCLAVE_INCLUDE_PROJECTS_H_ */
