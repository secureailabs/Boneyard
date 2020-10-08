/*
 * string_sgx.c
 *
 *  Created on: Dec 14, 2018
 *      Author: jingwei
 */
#include "../Include/enclave_wrapper/string_sgx.h"
#include <string.h>
#include <stdlib.h>

char* strcat(char* dest, const char* src){
	strcpy(dest+strlen(dest),src);
	return dest;
}

int strcmp(const char *p1, const char *p2)
{
  const unsigned char *s1 = (const unsigned char *) p1;
  const unsigned char *s2 = (const unsigned char *) p2;
  unsigned char c1, c2;
  do
    {
      c1 = (unsigned char) *s1++;
      c2 = (unsigned char) *s2++;
      if (c1 == '\0')
        return c1 - c2;
    }
  while (c1 == c2);
  return c1 - c2;
}

char* strcpy(char* dest, const char* src){
	return memcpy(dest, src, strlen(src)+1);
}

//char* strdup(const char* s){
//	size_t len = strlen(s)+1;
//	void *new = malloc(len);
//
//	if(new==NULL)
//		return NULL;
//
//	return (char*)memcpy(new,s,len);
//}
