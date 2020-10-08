/*
 * datetime.c
 *
 *  Created on: Feb 23, 2019
 *      Author: jingwei
 */
#include "../Include/enclave_wrapper/time.h"

static struct tm dummytm={0,0,0,1,0,1900,0,0,0,0,"GMT"};

struct tm* gmtime(const time_t *timep){
	return &dummytm;
}

struct tm* localtime(const time_t *timep){
    return &dummytm;
}

clock_t clock(void){
	return 1;
}

time_t time(time_t* timer){
	return 3600;
}

time_t mktime(struct tm* tmptr){
	return 3600;
}

void tzset(void){

}

int gettimeofday(struct timeval* tv, struct timezone* tz){
	return 0;
}

int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout){
	return 0;
}


