/*
 * vmoutput.h
 *
 *  Created on: Jan 25, 2019
 *      Author: jingwei
 */

#ifndef GT_ENCLAVE_INCLUDE_VMOUTPUT_H_
#define GT_ENCLAVE_INCLUDE_VMOUTPUT_H_

#define RESULT_BUF_SIZE 4096
#define RESULT_TYPE_SIZE 16
typedef struct _Result {
	int result_code;
	int cx; //Current length of the result buffer has been used
	char* result_type;
	char* result_buf;
} VM_Result;

extern VM_Result Result;

#endif /* GT_ENCLAVE_INCLUDE_VMOUTPUT_H_ */
