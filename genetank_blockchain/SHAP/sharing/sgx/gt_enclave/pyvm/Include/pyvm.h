/*
 * pyvm.h
 *
 *  Created on: Jan 10, 2019
 *      Author: jingwei
 */

#ifndef GT_ENCLAVE_PYVM_INCLUDE_PYVM_H_
#define GT_ENCLAVE_PYVM_INCLUDE_PYVM_H_

#ifdef __cplusplus
extern "C"{
#endif

#include "../../include/vmoutput.h"

//-------------------------by Jingwei
VM_Result* run_py(const char *py_code, char *result, size_t result_size);
void add_native();
void add_numpy();
void add_scipy();
void add_sklearn();
void add_pandas();
void add_matplotlib();
void add_xgboost();

#ifdef __cplusplus
}
#endif

#endif /* GT_ENCLAVE_PYVM_INCLUDE_PYVM_H_ */
