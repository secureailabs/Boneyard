/*
 * enclave_internal_io.h
 *
 *  Created on: Dec 3, 2018
 *      Author: jingwei
 */

#ifndef GT_ENCLAVE_PYVM_INCLUDE_ENCLAVE_WRAPPER_IO_H_
#define GT_ENCLAVE_PYVM_INCLUDE_ENCLAVE_WRAPPER_IO_H_

#include <stdarg.h> //for variable arguments
#include <stddef.h> //for size_t
#include <time.h>   //for time_t
#include <sys/types.h> //for ssize_t
#include <sgx_tprotected_fs.h>

#ifdef __cplusplus
extern "C"{
#endif

#define BUFLEN 1024
//-------------Type conversion---------
#define FILE SGX_FILE
//-------------------------------------

//------System macro defination--------
#define O_RDONLY 00
#define O_CREAT 0100
#define O_TRUNC 01000
#define O_APPEND 02000
#define O_RDWR 02
#define O_WRONLY 01
#define O_EXCL 0200

#define _IONBF 2
#define _IOLBF 1
#define _IOFBF 0

#define SEEK_SET 0
#define SEEK_CUR 1
#define SEEK_END 2

#define S_IXUSR 100
#define S_IXGRP 10
#define S_IXOTH 1

#define F_GETFL 1
#define F_DUPFD 2
#define F_SETFL 3
#define F_GETFD 4
#define F_SETFD 5

#define FD_CLOEXEC 6

#define O_NONBLOCK 04000

#define S_IFDIR 0040000

#define S_ISDIR(m) 0
//-------------------------------------



//-----User defined macros-------------
#define MAX_OPEN_FILE_NUM 20
#define MSG_BUF_LEN 4096
//-------------------------------------

struct msgbuf{
	unsigned int cx;
	char* buf;
};

extern struct msgbuf inbuf;
extern struct msgbuf outbuf;
extern struct msgbuf errsbuf;

extern FILE* stdin;
extern FILE* stdout;
extern FILE* stderr;

size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);
ssize_t write(int fildes, const char *buf, size_t nbyte);
size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream);
ssize_t read(int fildes, void* buf, size_t nbyte);
FILE *fopen(const char *filename, const char *mode);
//int open(const char path, int flag);
int fclose(FILE *stream);

FILE* fdopen(int fd, const char* mode);
//int close(int fildes);

int fprintf(FILE *stream, const char *format, ...);
int vfprintf(FILE *stream, const char *format, va_list arg);
int printf(const char *format, ...);
int sprintf(char *str, const char *string,...);

int fscanf(FILE* stream, const char* format, ...);
int sscanf(const char* str, const char* format, ...);

int fputs(const char *str, FILE *stream);
char* fgets (char * str, int num, FILE * stream );
int fputc(int character, FILE* stream);
int fgetc(FILE* stream);

int fseeko(FILE *stream, off_t offset, int whence);
off_t ftello(FILE *stream);

int getc(FILE* stream);
int ungetc(int character, FILE* stream);
int putc(int character, FILE* stream);
int puts(const char* str);

int putchar(int ch);

void clearerr(FILE* stream);
void perror(const char* str);

int feof(FILE *stream);

int fflush(FILE* stream);

int fileno(FILE* stream);
//int fcntl(int fd, int cmd, ...);
int isatty(int fd);

//int stat(const char* path, struct stat* buf);
//int fstat(int fd, struct stat* buf);
int unlink(const char* path);

int ferror(FILE* stream);
int fseek(FILE* stream, long int offset, int origin);

void flockfile(FILE* f);
void funlockfile(FILE* f);
int getc_unlocked(FILE* f);

long int ftell(FILE* stream);
off_t lseek(int fildes, off_t offset, int whence);
//int ftruncate(int fildes, off_t length);
int setvbuf(int fildes, char* buffer, int mode, size_t size);

int isdir(char *path);

int close(int fildes);
int open(const char* path, int oflag, ...);

void rewind(FILE* stream);
int remove(const char* filename);

#ifdef __cplusplus
}
#endif

#endif /* GT_ENCLAVE_PYVM_INCLUDE_ENCLAVE_WRAPPER_IO_H_ */
