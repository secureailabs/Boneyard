/*
 * ecalls.cpp
 *
 *  Created on: Jul 11, 2018
 *      Author: accuser
 */
//#include "ecalls.h"
#include "../gt_enclave_u.h"
#include "enclave_control.h"
#include "enclave_tab.h"

#include <stddef.h> /* for size_t */


sgx_status_t call_ec_get_pubkey(sgx_status_t* retval, char * enclavePublic){
	EnclTabNode *pTabNode = enclaveTab->getNode(HOST_STAKEHOLDER_ID);
	if (NULL == pTabNode)
		return (sgx_status_t)-1;
	return ec_get_pubkey(pTabNode->pEncl->getEnclaveId(), retval, enclavePublic, 64);
}

sgx_status_t call_ec_get_report(sgx_status_t* retval, sgx_target_info_t* p_target, sgx_report_t* p_report){
	EnclTabNode *pTabNode = enclaveTab->getNode(HOST_STAKEHOLDER_ID);
	if (NULL == pTabNode)
		return (sgx_status_t)-1;
	return get_report(pTabNode->pEncl->getEnclaveId(), retval, p_target, p_report);
}

sgx_status_t call_enclave_set_eth(sgx_status_t *retval, eth_addr_t *runnerEthAddr, eth_addr_t *trainerEthAddr){
	EnclTabNode *pTabNode = enclaveTab->getNode(HOST_STAKEHOLDER_ID);
	if (NULL == pTabNode)
		return (sgx_status_t)-1;
	return enclave_set_eth(pTabNode->pEncl->getEnclaveId(), retval, runnerEthAddr, trainerEthAddr);
}

sgx_status_t call_ec_task_manager(sgx_status_t *retval, uint64_t tid_app, uint32_t index, uint32_t stId){
	EnclTabNode *pTabNode = enclaveTab->getNode(stId);
	if (NULL == pTabNode)
		return (sgx_status_t)-1;
	return ec_task_manager(pTabNode->pEncl->getEnclaveId(), retval, tid_app, index);
}
