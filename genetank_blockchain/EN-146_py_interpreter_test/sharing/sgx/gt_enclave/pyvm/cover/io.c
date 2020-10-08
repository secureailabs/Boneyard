#include "../Include/enclave_wrapper/io.h"
#include "../../include/data_management.h"
#include "../Include/Python.h"
#include <string.h>
#include <stdio.h>


//int do_fcntl(int, int, va_list);
/*
 * Moved to dir_management.cpp
size_t fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream){
	//dmWriteDataFile(stream, (const uint8_t *)ptr, size*nmemb);
	if(stream==stdout){
		strcat(outbuf.buf, ptr);
	    outbuf.cx+=nmemb;
	    return nmemb;
	}
	else if(stream==stdin){
		strcat(inbuf.buf, ptr);
	    inbuf.cx+=nmemb;
	    return nmemb;
	}
	else if(stream==stderr){
		strcat(errsbuf.buf, ptr);
	    errsbuf.cx+=nmemb;
	    return nmemb;
	}
	else{
	    return sgx_fwrite(ptr,size, nmemb, stream);
	}
}
 */

//
ssize_t write(int fildes, const char *buf, size_t nbyte){
	size_t len1 = strlen(buf);
	switch(fildes){
	    case 0:{
	    	size_t len2 = MSG_BUF_LEN-inbuf.cx-1;
	    	size_t len = len2>len1?len1:len2;
	    	strncpy(inbuf.buf+inbuf.cx, buf, len);
	    	inbuf.cx+=len;
	    	return len;
	    }
	    case 1:{
			size_t len2 = MSG_BUF_LEN-outbuf.cx-1;
			size_t len = len2>len1?len1:len2;
			strncpy(outbuf.buf+outbuf.cx, buf, len);
	        outbuf.cx+=len;
	        return len;
	    }
	    case 2:{
		    size_t len2 = MSG_BUF_LEN-errsbuf.cx-1;
			size_t len = len2>len1?len1:len2;
			strncpy(errsbuf.buf+errsbuf.cx, buf, len);
			errsbuf.cx+=len;
			return len;
	    }
	    default:{
	    	PyErr_SetString(PyExc_IOError, "fd is disabled");
	    }
	}

	return 0;
}

//size_t fread(void *ptr, size_t size, size_t nmemb, FILE *stream){
//	//size_t tmp_len=size*nmemb;
//	//dmReadDataFile(stream, (uint8_t *)ptr, &tmp_len);
//	return sgx_fread(ptr,size,nmemb,stream);
//}

//FILE *fopen(const char *filename, const char *mode){
//    return sgx_fopen_auto_key(filename, mode);
//}

int printf(const char *format, ...){
	va_list arg;
	int ret;

	va_start(arg,format);
	ret=vfprintf(stdout,format,arg);
	va_end(arg);

	return ret;
}

//int fclose(FILE *stream){
//	return sgx_fclose(stream);
//}

int fprintf(FILE *stream, const char *format, ...){
    va_list arg;
    int done;

    va_start(arg, format);
    done = vfprintf(stream, format, arg);
    va_end(arg);

    return done;
}

int vfprintf(FILE *stream, const char *format, va_list arg){

    if(stream==stdout){
    	int tmp=vsnprintf(outbuf.buf+outbuf.cx, MSG_BUF_LEN-outbuf.cx-1, format, arg);
    	outbuf.cx+=tmp;
    }
    else if(stream==stdin){
    	int tmp=vsnprintf(inbuf.buf+outbuf.cx, MSG_BUF_LEN-outbuf.cx-1, format, arg);
    	inbuf.cx+=tmp;
    }
    else if(stream==stderr){
    	int tmp=vsnprintf(errsbuf.buf+outbuf.cx, MSG_BUF_LEN-outbuf.cx-1, format, arg);
    	errsbuf.cx+=tmp;
    }
    else{
    	char tmp_buf[MSG_BUF_LEN];
    	int tmp;
    	tmp=vsnprintf(tmp_buf, MSG_BUF_LEN, format, arg);
    	if(tmp<0)
    	    return -1;
        fwrite(tmp_buf, sizeof(char), MSG_BUF_LEN, stream);
    }
    return 1;
}

//char* fgets (char* str, int num, FILE* stream ){
//	fread(str, sizeof(char), num, stream);
//	return str;
//}
//
int fputs(const char *str, FILE *stream){
	size_t len1 =strlen(str);
	if(stream==stdout){
		size_t len2 = MSG_BUF_LEN-outbuf.cx-1;
	    size_t len = len2>len1?len1:len2;
	    strncpy(outbuf.buf+outbuf.cx, str, len);
	    outbuf.cx+=len;
		return len;
    }
	else if(stream==stdin){
		size_t len2 = MSG_BUF_LEN-inbuf.cx-1;
	    size_t len = len2>len1?len1:len2;
	    strncpy(inbuf.buf+inbuf.cx, str, len);
		inbuf.cx+=len;
		return len;
	}
	else if(stream==stderr){
		size_t len2 = MSG_BUF_LEN-errsbuf.cx-1;
	    size_t len = len2>len1?len1:len2;
		strncpy(errsbuf.buf+errsbuf.cx, str, len);
		errsbuf.cx+=len;
		return len;
	}
    return fwrite(str, sizeof(char), strlen(str), stream);
}

int sprintf(char *str, const char *string,...){
	va_list arg;
	int ret;

	va_start(arg, string);
	ret=vsnprintf(str,200,string,arg);
	va_end(arg);

	return ret;
}

//int fcntl(int fd, int cmd, ...){
//    va_list ap;
//    int res;
//
//    va_start(ap,cmd);
//    res=do_fcntl(fd,cmd,ap);
//    va_end(ap);
//
//    return res;
//}

//int do_fcntl(int fd, int cmd, va_list ap){
////   void* arg=va_arg(ap);
//
////    switch(cmd){
////    case F_GETFL :{
////        return fildes[fd].stat_flag;
////    }
////    case F_DUPFD :{
////        fildes[fd].f=fildes[(int)(*arg)].f;
////       return 1;
////    }
////    }
//    return 1;
//}

int getc(FILE* stream){
	char ret;
	int bytes=fread(&ret, sizeof(char), 1, stream);
	if(bytes==1)
	    return ret;
	else
		return -1;
}

int ungetc(int character, FILE* stream){
	int pos = ftell(stream)-1;
	fseek(stream,pos,SEEK_SET);
	if(getc(stream) == character) {
		pos = ftell(stream)-1;
		fseek(stream,pos,SEEK_SET);
	    return character;
	} else {
		//[EN-221] Not supported. So return EOF.
		return EOF;
	}
}

//int fputc(int character, FILE* stream){
//    char tmp=(char)character;
//	fwrite(&tmp, 1, sizeof(char), stream);
//    return character;
//}

int putc(int character, FILE* stream){
	char tmp=(char)character;
	if(outbuf.cx+1<MSG_BUF_LEN){
	    strncpy(outbuf.buf+outbuf.cx,&tmp,1);
	    outbuf.cx+=1;
	    outbuf.buf[outbuf.cx]='\0';
	    return character;
	}
	return 0;
}

//void clearerr(FILE* stream){
//	return sgx_clearerr(stream);
//}

void perror(const char* str){
    strncpy(errsbuf.buf+errsbuf.cx, str, MSG_BUF_LEN-errsbuf.cx-1);
    errsbuf.cx+=strnlen(errsbuf.buf+errsbuf.cx, MSG_BUF_LEN-errsbuf.cx-1);
}
//
//int feof(FILE *stream){
//	return sgx_feof(stream);
//}

int isatty(int fd){
	if(fd==0||fd==1||fd==2)
		return 1;
	else
		return 0;
}

int sscanf(const char* str, const char* format, ...){
	return 0;
}

FILE* fdopen(int fd, const char* mode){
	return fopen("tmp",mode);
}

//int fflush(FILE* stream){
//	return sgx_fflush(stream);
//}

int fileno(FILE* stream){
	PyErr_SetString(PyExc_IOError, "fd is disabled");
	return 2;
}

//int stat(const char* path, struct stat* buf){
////	buf->st_dev;
////	buf->st_ino;
////	buf->st_mode;
////	buf->st_nlink;
////	buf->st_uid;
////	buf->st_gid;
////	buf->st_rdev;
////	buf->st_size;
////	buf->st_mtime;
//	return 0;
//}
//
//int fstat(int fd, struct stat* buf){
//	return 0;
//}

int unlink(const char* path){
    return 0;
}

//int ferror(FILE* stream){
//	return sgx_ferror(stream);
//}
//
//int fseek(FILE* stream, long int offset, int origin){
//	return sgx_fseek(stream,offset,origin);
//}

//int open(const char* path, int flag){
//	return 0;
//}
//
//int close(int fildes){
//	if(fildes!=0&&fildes!=1&&fildes!=2){
//
//	}
//	return 0;
//}

ssize_t read(int fildes, void* buf, size_t nbyte){
	switch(fildes){
		    case 0:{
		        strncpy(buf, inbuf.buf, nbyte);
		        inbuf.cx-=nbyte;
		        inbuf.cx=inbuf.cx<0?inbuf.cx:0;
		        break;
		    }
		    case 1:{
		    	strncpy(buf, outbuf.buf, nbyte);
		    	outbuf.cx-=nbyte;
		    	outbuf.cx=outbuf.cx<0?outbuf.cx:0;
		    	break;
		    }
		    case 2:{
		    	strncpy(buf, errsbuf.buf, nbyte);
		    	errsbuf.cx-=nbyte;
		    	errsbuf.cx=errsbuf.cx<0?errsbuf.cx:0;
		    	break;
		    }
		    default:{
		    	PyErr_SetString(PyExc_IOError, "fd is disabled");
		    }
		}
		return nbyte;
}

int puts(const char* str){
	if(strlen(str)+outbuf.cx<MSG_BUF_LEN){
	    strncpy(outbuf.buf+outbuf.cx, str, strlen(str));
	    outbuf.cx+=strlen(str);
	    return strlen(str);
	}
	return 0;
}

int putchar(int ch){
	char tmp=(char)ch;
	if(outbuf.cx+1<MSG_BUF_LEN){
	    strncpy(outbuf.buf,&tmp,1);
	    outbuf.cx+=1;
	    outbuf.buf[outbuf.cx]='\0';
	    return ch;
	}
	return 0;
}

int fscanf(FILE* stream, const char* format, ...){
	return 0;
}

void flockfile(FILE* f){
}
void funlockfile(FILE* f){
}
int getc_unlocked(FILE* f){
	return getc(f);
}

//long int ftell(FILE* stream){
//	return sgx_ftell(stream);
//}

off_t lseek(int fildes, off_t offset, int whence){
	return 0;
}

int fseeko(FILE *stream, off_t offset, int whence){
	return fseek(stream, (long int)offset, whence);
}

off_t ftello(FILE *stream){
	return (off_t)ftell(stream);
}


int setvbuf(int fildes, char* buffer, int mode, size_t size){
	return 0;
}

int close(int fildes){
	return 0;
}

int open(const char* path, int oflag, ...){
	if(NULL!=fopen(path, "w"))
	    return 2;
	else
		return -1;
}

void rewind(FILE* stream){
    fseek(stream, 0, SEEK_SET);
}

/*
 * Moved to dir_management.cpp
 */
//int remove(const char* filename){
//    return sgx_remove(filename);
//}
