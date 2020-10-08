/*
 * signal.h
 *
 *  Created on: Dec 17, 2018
 *      Author: jingwei
 */

#ifndef GT_ENCLAVE_PYVM_INCLUDE_ENCLAVE_WRAPPER_SIGNAL_H_
#define GT_ENCLAVE_PYVM_INCLUDE_ENCLAVE_WRAPPER_SIGNAL_H_

#define SIGINT 2

#define SIGSEGV 11

#define SIG_IGN (void(*)(int))0
#define SIG_DFL (void(*)(int))1
#define SIG_ERR (void(*)(int))-1

typedef void (*signalhandler_t)(int);

signalhandler_t signal(int signum, signalhandler_t handler);

#endif /* GT_ENCLAVE_PYVM_INCLUDE_ENCLAVE_WRAPPER_SIGNAL_H_ */
