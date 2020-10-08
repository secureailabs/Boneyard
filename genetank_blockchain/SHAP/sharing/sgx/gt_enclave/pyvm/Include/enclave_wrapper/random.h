/*
 * random.h
 *
 *  Created on: Dec 7, 2018
 *      Author: jingwei
 */

#ifndef GT_ENCLAVE_PYVM_INCLUDE_ENCLAVE_WRAPPER_RANDOM_H_
#define GT_ENCLAVE_PYVM_INCLUDE_ENCLAVE_WRAPPER_RANDOM_H_

#include <sgx_trts.h>
#include <sys/types.h>

unsigned long getrandom();

#endif /* GT_ENCLAVE_PYVM_INCLUDE_ENCLAVE_WRAPPER_RANDOM_H_ */
