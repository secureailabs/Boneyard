/*
 * sys.h
 *
 *  Created on: Jan 8, 2019
 *      Author: jingwei
 */

#ifndef GT_ENCLAVE_PYVM_INCLUDE_ENCLAVE_WRAPPER_SYS_H_
#define GT_ENCLAVE_PYVM_INCLUDE_ENCLAVE_WRAPPER_SYS_H_

#include <setjmp.h>
#include <time.h>

#define ENVLIMIT 20

typedef unsigned short mode_t;
typedef unsigned long ino_t;
typedef short dev_t;
typedef unsigned short nlink_t;
typedef long off_t;
typedef unsigned short uid_t;
typedef unsigned short gid_t;
typedef int pid_t;

typedef int sig_atomic_t;

struct stat{
    mode_t st_mode;    //protection bits;
    ino_t st_ino;     //inode
    dev_t st_dev;     //device
    nlink_t st_nlink;   //number of hard links
    uid_t st_uid;    //user ID of owner
    gid_t st_gid;    //group ID of owner
    off_t st_size;   //total size, in bytes
    time_t st_atime;   //time of last access
    time_t st_mtime;  //time of last modification
    time_t st_ctime;   //time of last change"
};

extern jmp_buf top_env;

extern char** envlist;

#ifdef __cplusplus
extern "C"{
#endif

char* getenv(const char* name);

void exit(int status);

int stat(const char* pathname, struct stat* stabuf);
int fstat(int fd, struct stat *stabuf);

#ifdef __cplusplus
}
#endif

#endif /* GT_ENCLAVE_PYVM_INCLUDE_ENCLAVE_WRAPPER_SYS_H_ */
