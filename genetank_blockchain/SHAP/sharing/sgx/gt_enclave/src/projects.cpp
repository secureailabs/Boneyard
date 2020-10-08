/*
 * projects.cpp
 *
 *  Created on: Sep 18, 2019
 *      Author: slowloris
 */
#include "projects.h"
#include <string.h>
#include "cluster.h"
#include "queue.h"

Projects* projectsList=NULL;

Queue subResultQueue;

//Cluster* activeSubEnclaveList = NULL;
//char fedProjectName[MAX_PATH];

void cleanUpHubOnKill(){
	//while (NULL != activeSubEnclaveList){
	//	deleteSubEnclave(&activeSubEnclaveList, activeSubEnclaveList);
	//}
}

sgx_status_t Projects::addToProjList(Projects** head){
	this->next = *head;
	*head = this;
	return SGX_SUCCESS;
}

sgx_status_t Projects::deletFromProjList(Projects** head, char* name, uint32_t stakeHolderId){
	if((*head)->stakeHolderId==stakeHolderId && strncmp((*head)->name, name, strlen(name))==0){
		*head = (*head)->next;
	}
	else{
		Projects* ptr = *head;
		while(ptr!=NULL){
			if((*head)->stakeHolderId==stakeHolderId && strncmp(ptr->next->name, name, strlen(name))==0){
				if(ptr->next!=NULL)
				    ptr->next = ptr->next->next;
				else
					ptr->next = NULL;
				break;
			}
			Projects* cur = ptr;
			ptr = ptr->next;
			free(cur);
		}
	}
}

Projects* Projects::searchInProjList(Projects** head, char* name, uint32_t stakeHolderId){
	Projects* ptr = *head;
	while(ptr!=NULL){
        if((*head)->stakeHolderId==stakeHolderId && strncmp(ptr->name, name, strlen(name))==0){
        	break;
        }
        ptr=ptr->next;
	}
	return ptr;
}
