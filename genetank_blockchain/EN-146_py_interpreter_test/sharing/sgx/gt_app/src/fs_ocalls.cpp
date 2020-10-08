/*
 * fs_ocalls.cpp
 *
 *  Created on: Apr 2, 2019
 *      Author: accuser
 */
//#include "stdafx.h"
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include "../gt_enclave_u.h"
//#include <fstream>

void * oc_fopen(const char *filename, const char *mode){
	return (fopen(filename, mode));
}

int32_t oc_fclose(void* f){
	return(fclose((FILE *)f));
}

size_t oc_fread(void *ptr, size_t total, size_t size, size_t nmemb, void *file){
	return(fread(ptr, size, nmemb, (FILE *)file));
}

int32_t oc_feof(void *file){
	return(feof((FILE *)file));
}

int32_t oc_fflush(void* file){
	return(fflush((FILE *)file));
}

int64_t oc_ftell(void* file){
	return(ftell((FILE *)file));
}

int32_t oc_ferror(void *file){
	return(ferror((FILE *)file));
}

void oc_clearerr(void* file){
	return(clearerr((FILE *)file));
}

int oc_remove(const char *filename){
	return(remove(filename));
}

int oc_fseek(void* file, int64_t offset, int origin){
	return(fseek((FILE *)file, offset, origin));
}

int oc_fgetc(void *fp){
	return(fgetc((FILE *)fp));
}

int oc_fputc(int chr, void *file){
	return(fputc(chr, (FILE *)file));
}

size_t oc_fwrite(const void *ptr, size_t totalb, size_t size, size_t nmemb, void *stream){
	return(fwrite(ptr, size, nmemb, (FILE *)stream));
}

int oc_isdir(char *path){
    DIR* directory = opendir(path);

    if(directory != NULL)
    {
     closedir(directory);
     return 1;
    }

    return 0;
}
