/*
 * ecalls.h
 *
 *  Created on: Jul 11, 2018
 *      Author: accuser
 */

#ifndef GT_APP_INCLUDE_ECALLS_H_
#define GT_APP_INCLUDE_ECALLS_H_

#include <stdlib.h> /* for size_t */

// Needed to create enclave and do ecall.
#include "sgx_urts.h"
//#include "sgx_tcrypto.h"

// Needed to query extended epid group id.
#include "sgx_uae_epid.h"

#include "msg_def.h"

#include <cpprest/asyncrt_utils.h>
using namespace utility;

//sgx_status_t call_ec_session(sgx_status_t* retval, uint32_t session_id, const uint8_t *reqMsg, size_t reqLen, uint8_t *rspMsg,
//		size_t maxRspSize, size_t *rspLen, uint8_t *iv);
//sgx_status_t call_ec_new_session(sgx_status_t* retval, uint32_t *session_id, const char *reqMsg,
//		uint8_t *rspMsg,
//		size_t maxRspSize, size_t *rspLen, uint8_t *iv);
sgx_status_t call_ec_get_pubkey(sgx_status_t* retval, char * enclavePublic);
//sgx_status_t call_ec_enclave_init(sgx_status_t *retval, const char* home, size_t path_size);
sgx_status_t call_enclave_set_eth(sgx_status_t *retval, eth_addr_t *runnerEthAddr, eth_addr_t *trainerEthAddr, uint32_t *stId);
sgx_status_t call_ec_get_report(sgx_status_t* retval, sgx_target_info_t* p_target, sgx_report_t* p_report);
//sgx_status_t call_ec_R_init(sgx_status_t* pRetVal, const char* init_code, size_t message_size,
//		char* result, size_t result_size);
//sgx_status_t call_ec_Py_init(sgx_status_t* pRetVal, const char* init_code, size_t message_size,
//		char* result, size_t result_size);
//sgx_status_t call_ec_import_lib(const char *fileName, const uint8_t *buff, size_t size);
//sgx_status_t call_ec_lib_imported();
//sgx_status_t call_ec_Py_clean(sgx_status_t *retval);
sgx_status_t call_ec_task_manager(sgx_status_t *retval,uint64_t tid_app,uint32_t index, uint32_t stId);


#endif /* GT_APP_INCLUDE_ECALLS_H_ */
