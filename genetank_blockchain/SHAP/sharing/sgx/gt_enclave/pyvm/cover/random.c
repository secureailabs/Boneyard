/*
 * random.c
 *
 *  Created on: Dec 7, 2018
 *      Author: jingwei
 */

#include "../Include/enclave_wrapper/random.h"

unsigned long getrandom(){
	void *buf;
	sgx_read_rand((unsigned char*)buf, 4);
    return (unsigned long*)buf;
}


