/*
 * randomForest.c
 *
 *  Created on: Oct 21, 2018
 *      Author: jingwei
 */

#include <string.h>

struct ext_fun{
	char* name;
	void* fun;
};

ext_fun extfuntab[]={
		{classRF, &classRF},
		{regRF, &regRF},
		{regForest, &regForest},
		{classForest, &classForest},
};

int exttab_len=4;

void* find_extfun(char* name, int len){
	for(int i=0;i<len;i++)
		if(strcmp(name, extfuntab[i].name)==0)
			return extfuntab[i].fun;
	return (void*)0;
}


