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


#ifndef _SERVICE_PROVIDER_H
#define _SERVICE_PROVIDER_H

#include "remote_attestation_result.h"
#include "ias_ra.h"
#include "../gt_app/include/ra_msg.h"

#ifdef  __cplusplus
extern "C" {
#endif

int sp_ra_proc_msg0_req(const sample_ra_msg0_t *p_msg0,
    uint32_t msg0_size);

int sp_ra_proc_msg1_req(const sample_ra_msg1_t *p_msg1,
						uint32_t msg1_size,
						ra_samp_response_header_t **pp_msg2);

int sp_ra_proc_msg3_req(const sample_ra_msg3_t *p_msg3,
                        uint32_t msg3_size,
                        ra_samp_response_header_t **pp_att_result_msg);

//int sp_ra_free_msg2(
//    sample_ra_msg2_t *p_msg2);

typedef int (*sample_enroll)(int sp_credentials, spid_t* spid,
    int* authentication_token);

typedef int(*sample_get_sigrl)(const epid_group_id_t gid, uint32_t* p_sig_rl_size,
    uint8_t** p_sig_rl);

typedef int(*sample_verify_attestation_evidence)(sample_quote_t* p_gt_quote,
    uint8_t* pse_manifest,
    ias_att_report_t* attestation_verification_report);


typedef struct sample_extended_epid_group
{
    uint32_t extended_epid_group_id;
    sample_enroll enroll;
    sample_get_sigrl get_sigrl;
    sample_verify_attestation_evidence verify_attestation_evidence;
} sample_extended_epid_group;

#ifdef  __cplusplus
}
#endif

#endif
