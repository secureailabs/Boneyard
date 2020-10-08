/*
 * rvm.h
 *
 *  Created on: Aug 10, 2018
 *      Author: accuser
 */

#ifndef GT_ENCLAVE_RVM_INCLUDE_RVM_H_
#define GT_ENCLAVE_RVM_INCLUDE_RVM_H_

#include "../../include/vmoutput.h"

void setup_Rmainloop();
int Rf_initialize_R();
//void R_setjmp();
//void check_session_exit();
void Rdummy();
VM_Result *run_R_code(const char *r_code,char *result, size_t result_size);

#endif /* GT_ENCLAVE_RVM_INCLUDE_RVM_H_ */
