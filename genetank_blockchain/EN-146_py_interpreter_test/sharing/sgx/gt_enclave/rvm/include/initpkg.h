/*
 * initpkg.h
 *
 *  Created on: Nov 1, 2018
 *      Author: jingwei
 */

#ifndef GT_ENCLAVE_RVM_INCLUDE_INITPKG_H_
#define GT_ENCLAVE_RVM_INCLUDE_INITPKG_H_

#ifndef R_EXT_RDYNLOAD_H_
#include <R_ext/Rdynload.h>
#endif

void R_init_stats(DllInfo *dll);
void R_init_randomForest(DllInfo *dll);



#endif /* GT_ENCLAVE_RVM_INCLUDE_INITPKG_H_ */
