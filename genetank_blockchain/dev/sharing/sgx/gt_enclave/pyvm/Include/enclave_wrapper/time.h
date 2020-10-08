/*
 * time.h
 *
 *  Created on: Dec 10, 2018
 *      Author: jingwei
 */

#ifndef GT_ENCLAVE_PYVM_INCLUDE_ENCLAVE_WRAPPER_TIME_H_
#define GT_ENCLAVE_PYVM_INCLUDE_ENCLAVE_WRAPPER_TIME_H_

//#include <sgx_tae_service.h>
#include <time.h>

#define CLOCK_REALTIME 0
#define CLOCK_MONOTONIC 1

#define clockid_t long

struct timespec{
	long tv_sec;
	long tv_nsec;
};

struct timeval{
	long tv_sec;
	long tv_usec;
};

typedef struct{
	long fd_bits[1024/64];
}fd_set;

struct timezone{
	int tz_minutewest;
	int tz_dsttime;
};

struct tm* gmtime(const long *timep);
struct tm* localtime(const long *timep);
//clock_t clock(void);
long time(long* timer);
long mktime(struct tm* tmptr);
void tzset(void);
int gettimeofday(struct timeval* tv, struct timezone* tz);
int select(int nfds, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout);

#endif /* GT_ENCLAVE_PYVM_INCLUDE_ENCLAVE_WRAPPER_TIME_H_ */
