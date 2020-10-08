/*
 * ra.h
 *
 *  Created on: Jul 12, 2018
 *      Author: accuser
 */

#ifndef GT_APP_INCLUDE_RA_H_
#define GT_APP_INCLUDE_RA_H_

// Needed to create enclave and do ecall.
#include "sgx_urts.h"

// Needed to query extended epid group id.
#include "sgx_uae_epid.h"

sgx_status_t oc_sendAttReq(const char * server_url, sgx_report_t *pReport, sgx_spid_t *pSpid_a,
		char *attestation_report, size_t reportSize);

/*
sgx_status_t simple_ra(const utility::string_t & server_url,
									const utility::string_t & spid,
									utility::string_t & mrenclave,
									utility::string_t & pubkey,
									utility::string_t & attestation_report);
*/


#endif /* GT_APP_INCLUDE_RA_H_ */
