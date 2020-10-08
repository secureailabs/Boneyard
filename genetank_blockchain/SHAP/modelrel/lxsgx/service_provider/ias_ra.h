/*
 *   Copyright(C) 2011-2017 Intel Corporation All Rights Reserved.
 *
 *   The source code, information  and  material ("Material") contained herein is
 *   owned  by Intel Corporation or its suppliers or licensors, and title to such
 *   Material remains  with Intel Corporation  or its suppliers or licensors. The
 *   Material  contains proprietary information  of  Intel or  its  suppliers and
 *   licensors. The  Material is protected by worldwide copyright laws and treaty
 *   provisions. No  part  of  the  Material  may  be  used,  copied, reproduced,
 *   modified, published, uploaded, posted, transmitted, distributed or disclosed
 *   in any way  without Intel's  prior  express written  permission. No  license
 *   under  any patent, copyright  or  other intellectual property rights  in the
 *   Material  is  granted  to  or  conferred  upon  you,  either  expressly,  by
 *   implication, inducement,  estoppel or  otherwise.  Any  license  under  such
 *   intellectual  property  rights must  be express  and  approved  by  Intel in
 *   writing.
 *
 *   *Third Party trademarks are the property of their respective owners.
 *
 *   Unless otherwise  agreed  by Intel  in writing, you may not remove  or alter
 *   this  notice or  any other notice embedded  in Materials by Intel or Intel's
 *   suppliers or licensors in any way.
 *
 */


#ifndef _IAS_RA_H
#define _IAS_RA_H

#include "ecp.h"

#include "../gt_app/include/ra_msg.h"

// This data structures in this file are for demonstration only.
// See the IAS API for the full definition of the Attestation data structures

//#ifdef  __cplusplus
//extern "C" {
//#endif

int ias_enroll(int sp_credentials, spid_t* spid,
	int* authentication_token);
int ias_get_sigrl(const epid_group_id_t gid, uint32_t* p_sig_rl_size,
	uint8_t** p_sig_rl);
int ias_verify_attestation_evidence(sample_quote_t* p_gt_quote,
	uint8_t* pse_manifest,
	ias_att_report_t* attestation_verification_report);
//#ifdef  __cplusplus
//}
//#endif

#endif
