/*
 * sys.c
 *
 *  Created on: Jan 8, 2019
 *      Author: jingwei
 */

#include "../Include/enclave_wrapper/sys.h"
#include "../Include/enclave_wrapper/signal.h"
#include <stdio.h>

char* getenv(const char* name){
    for(int i=0;i<ENVLIMIT;i++){
    	if(envlist[i]!=NULL){
    	    if(strcmp(envlist[i],name)==0){
    		    return envlist[i];
    	    }
    	}
    }
    return NULL;
}

void exit(int status){
	longjmp(top_env, 1);
}

signalhandler_t signal(int signum, signalhandler_t handler){
	return handler;
}

int stat(const char* pathname, struct stat* stabuf){

	stabuf->st_mode=100000;    //protection bits;
	stabuf->st_ino=0;     //inode
	stabuf->st_dev=4;     //device
	stabuf->st_nlink=0;   //number of hard links
	stabuf->st_uid=1;    //user ID of owner
	stabuf->st_gid=1;    //group ID of owner
	stabuf->st_size=4096;   //total size, in bytes
	stabuf->st_atime=0;   //time of last access
	stabuf->st_mtime=0;  //time of last modification
	stabuf->st_ctime=0;   //time of last change"

	return 0;
}

int fstat(int fd, struct stat *stabuf){
	stabuf->st_mode=100000;    //protection bits;
	stabuf->st_ino=0;     //inode
	stabuf->st_dev=4;     //device
	stabuf->st_nlink=0;   //number of hard links
	stabuf->st_uid=1;    //user ID of owner
	stabuf->st_gid=1;    //group ID of owner
	stabuf->st_size=4096;   //total size, in bytes
	stabuf->st_atime=0;   //time of last access
	stabuf->st_mtime=0;  //time of last modification
	stabuf->st_ctime=0;   //time of last change"

	return 0;

}
