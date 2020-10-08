#include "../include/stdafx.h"
#include "../include/tools.h"

//for sleep()
#include <unistd.h>
//#include <stdio.h>
//#include <limits.h>
// Needed for definition of remote attestation messages.
//#include "remote_attestation_result.h"

//for string_t
using namespace utility;

#include "../gt_enclave_u.h"

// Needed to call untrusted key exchange library APIs, i.e. sgx_ra_proc_msg2.
#include "sgx_ukey_exchange.h"

// In addition to generating and sending messages, this application
// can use pre-generated messages to verify the generation of
// messages and the information flow.
#include "../include/sample_messages.h"

// Needed to get service provider's information, in your real project, you will
// need to talk to real server.
//#include "network_ra.h"
#include "../include/ra_network.h"

#include "../include/enclave_control.h"

// Needed to create enclave and do ecall.
#include "sgx_urts.h"

// Needed to query extended epid group id.
#include "sgx_uae_service.h"

// This is the public EC key of SP, this key is hard coded in gt_enclave.
// It is based on NIST P-256 curve. Not used in the SP code.
static const sample_ec_pub_t g_sp_pub_key = {
	{
		0x72, 0x12, 0x8a, 0x7a, 0x17, 0x52, 0x6e, 0xbf,
		0x85, 0xd0, 0x3a, 0x62, 0x37, 0x30, 0xae, 0xad,
		0x3e, 0x3d, 0xaa, 0xee, 0x9c, 0x60, 0x73, 0x1d,
		0xb0, 0x5b, 0xe8, 0x62, 0x1c, 0x4b, 0xeb, 0x38
	},
	{
		0xd4, 0x81, 0x40, 0xd9, 0x50, 0xe2, 0x57, 0x7b,
		0x26, 0xee, 0xb7, 0x41, 0xe7, 0xc6, 0x14, 0xe2,
		0x24, 0xb7, 0xbd, 0xc9, 0x03, 0xf2, 0x9a, 0x28,
		0xa8, 0x3c, 0xc8, 0x10, 0x11, 0x14, 0x5e, 0x06
	}
};

#ifndef SAFE_FREE
#define SAFE_FREE(ptr) {if (NULL != (ptr)) {free(ptr); (ptr) = NULL;}}
#endif

uint8_t* msg1_samples[] = { msg1_sample1, msg1_sample2 };
uint8_t* msg2_samples[] = { msg2_sample1, msg2_sample2 };
uint8_t* msg3_samples[] = { msg3_sample1, msg3_sample2 };
uint8_t* attestation_msg_samples[] =
    { attestation_msg_sample1, attestation_msg_sample2};

// Some utility functions to output some of the data structures passed between
// the ISV app and the remote attestation service provider.
void PRINT_BYTE_ARRAY(
    FILE *file, void *mem, uint32_t len)
{
    if(!mem || !len)
    {
        fprintf(file, "\n( null )\n");
        return;
    }
    uint8_t *array = (uint8_t *)mem;
    fprintf(file, "%u bytes:\n{\n", len);
    uint32_t i = 0;
    for(i = 0; i < len - 1; i++)
    {
        fprintf(file, "%2x ", array[i]);
        if(i % 32 == 31) fprintf(file, "\n");
    }
	fprintf(file, "%2x ", array[i]);
    fprintf(file, "\n}\n");
}


void PRINT_ATTESTATION_SERVICE_RESPONSE(
    FILE *file,
    ra_samp_response_header_t *response)
{
    if(!response)
    {
        fprintf(file, "\t\n( null )\n");
        return;
    }

    fprintf(file, "RESPONSE TYPE:   0x%x\n", response->type);
    fprintf(file, "RESPONSE STATUS: 0x%x 0x%x\n", response->status[0],
            response->status[1]);
    fprintf(file, "RESPONSE BODY SIZE: %u\n", response->size);

    if(response->type == TYPE_RA_MSG2)
    {
        sgx_ra_msg2_t* p_msg2_body = (sgx_ra_msg2_t*)(response->body);

        fprintf(file, "MSG2 gb - ");
        PRINT_BYTE_ARRAY(file, &(p_msg2_body->g_b), sizeof(p_msg2_body->g_b));

        fprintf(file, "MSG2 spid - ");
        PRINT_BYTE_ARRAY(file, &(p_msg2_body->spid), sizeof(p_msg2_body->spid));

        fprintf(file, "MSG2 quote_type : %hx\n", p_msg2_body->quote_type);

        fprintf(file, "MSG2 kdf_id : %hx\n", p_msg2_body->kdf_id);

        fprintf(file, "MSG2 sign_gb_ga - ");
        PRINT_BYTE_ARRAY(file, &(p_msg2_body->sign_gb_ga),
                         sizeof(p_msg2_body->sign_gb_ga));

        fprintf(file, "MSG2 mac - ");
        PRINT_BYTE_ARRAY(file, &(p_msg2_body->mac), sizeof(p_msg2_body->mac));

        fprintf(file, "MSG2 sig_rl - ");
        PRINT_BYTE_ARRAY(file, &(p_msg2_body->sig_rl),
                         p_msg2_body->sig_rl_size);
    }
    else if(response->type == TYPE_RA_ATT_RESULT)
    {
        sample_ra_att_result_msg_t *p_att_result =
            (sample_ra_att_result_msg_t *)(response->body);
        fprintf(file, "ATTESTATION RESULT MSG platform_info_blob - ");
        PRINT_BYTE_ARRAY(file, &(p_att_result->platform_info_blob),
                         sizeof(p_att_result->platform_info_blob));

        fprintf(file, "ATTESTATION RESULT MSG mac - ");
        PRINT_BYTE_ARRAY(file, &(p_att_result->mac), sizeof(p_att_result->mac));

        fprintf(file, "ATTESTATION RESULT MSG secret.payload_tag - %u bytes\n",
                p_att_result->secret.payload_size);

        fprintf(file, "ATTESTATION RESULT MSG secret.payload - ");
        PRINT_BYTE_ARRAY(file, p_att_result->secret.payload,
                p_att_result->secret.payload_size);
    }
    else
    {
        fprintf(file, "\nERROR in printing out the response. "
                       "Response of type not supported %d\n", response->type);
    }
}

//Simple remote attestation: 
// 1. The application calls the enclave to create a report of the enclave
// 2. The application calls QE to generate a quote for the report
// 3. The application sends the quote to the service provider with HTTP POST request
//    and the quote JSON as the body of the request
// 4. The service provider (trainer) calls Intel IAS to verify the quote and response
//    the signed attestation report
// 5. The service provider response the application with the attestation report
// 6. The application sends the attestation report to the runner Dapp
sgx_status_t EnclaveControl::simple_ra(const utility::string_t & server_url,
									const utility::string_t & runner_address,
									const utility::string_t & spid,
									utility::string_t & mrenclave,
									utility::string_t & pubkey,
									utility::string_t & attestation_report) {
	sgx_status_t ret, status;
	sgx_target_info_t target_info;
	sgx_epid_group_id_t gid;
	uint32_t quote_size;
	sgx_report_t report;
	sgx_spid_t spid_a = { 0x7B,0xC0,0x33,0x8D,0x24,0x78,0x1F,0x74,
		0x48,0x75,0x17,0x8F,0xE7,0x38,0x1F,0x76 };
	sgx_quote_t *p_quote;
	size_t len;

	if (spid.length() != 16 * 2 + 2) 
		return SGX_ERROR_UNEXPECTED;//something wrong in the SPID

	for (int i = 0; i < 16; i++) {
		uint8_t hi = hexnibble((char)spid[i * 2 + 2]) << 4;
		uint8_t low = hexnibble((char)spid[i * 2 + 3]);
		spid_a.id[i] = hi + low;
	}

	FILE* OUTPUT = stdout;

	//std::string mstr = std::string(R"(sdflsdfjlkdf)") + std::string(R"(sdflklksdflklksdf)");

	//char runner_addr[MAX_ADDR_SIZE];

	//wcstombs_s(&len, runner_addr, runner_address.c_str(), runner_address.length()); //convert string_t to char *

	ret = sgx_init_quote(&target_info, &gid);

	if (SGX_SUCCESS != ret) {
		fprintf(OUTPUT, "\nError, call sgx_init_quote() fail [%s].",
			__FUNCTION__);
		return ret;
	}

	ret = get_report(enclave_id, &status, runner_address.c_str(), len, &target_info, &report);

	mrenclave = mBin2Hex(report.body.mr_enclave.m,32);
	pubkey = mBin2Hex(report.body.report_data.d, 64);

	if (SGX_SUCCESS != ret) {
		fprintf(OUTPUT, "\nError, call get_report() fail [%s].",
			__FUNCTION__);
		return ret;
	}

	uint32_t err_code = (uint32_t)status;
	switch (err_code) {
	case 0x9001:
		fprintf(OUTPUT, "\nError:runner address exist and doesn't match, call get_report() fail [%s].", __FUNCTION__);
		ret = SGX_ERROR_UNEXPECTED;
		fprintf(OUTPUT, "\nError, call get_report() fail [%s].",
			__FUNCTION__);
		return status;
		break;
	case 0x9002:
	case 0x9003:
	case 0x9004:
	case 0x9005:
		fprintf(OUTPUT, "\nError:sealing or storing runner address error, call get_report() fail [%s].", __FUNCTION__);
		return status;
		break;
	default:
		break;
	}

	ret = sgx_calc_quote_size(NULL, 0, &quote_size);

	if (SGX_SUCCESS != ret) {
		fprintf(OUTPUT, "\nError, call sgx_calc_quote_size() fail [%s].",
			__FUNCTION__);
		return ret;
	}

	p_quote = (sgx_quote_t *)malloc(quote_size);
	if (NULL == p_quote) {
		fprintf(OUTPUT, "\nError, malloc() for quote fail [%s].",
				__FUNCTION__);
		return ret;
	}
	
	ret = sgx_get_quote(&report, 
						SGX_LINKABLE_SIGNATURE, 
						&spid_a, 
						NULL, //&nonce,
						NULL, //sig_rl
		                0, //sig_rl size
						NULL, //&qe_report,
						p_quote, 
						quote_size);

	if (SGX_SUCCESS != ret) {
		fprintf(OUTPUT, "\nError, call sgx_get_quote() fail [%s] Error code:%d.",
			__FUNCTION__, ret);
		SAFE_FREE(p_quote);
		return ret;
	}

	ra_samp_request_header_t *p_msg_req = NULL;
	ra_samp_response_header_t *p_msg_resp = NULL;

	p_msg_req = (ra_samp_request_header_t*)
		malloc(sizeof(ra_samp_request_header_t) + quote_size);

	if (NULL == p_msg_req) {
		fprintf(OUTPUT, "\nError, malloc() for message request fail [%s].",
			__FUNCTION__);
		SAFE_FREE(p_quote);
		return ret;
	}

	p_msg_req->type = TYPE_RA_SIMPLE;
	p_msg_req->size = quote_size;

	//if (memcpy_s(p_msg_req->body,quote_size,p_quote,quote_size)){
	if (NULL == memcpy(p_msg_req->body,p_quote,quote_size)){
		fprintf(OUTPUT, "\nError, malloc() for message request fail [%s].",
			__FUNCTION__);
		SAFE_FREE(p_quote);
		SAFE_FREE(p_msg_req);
		return ret;
	}

	ret = ra_network_send_receive(server_url, p_msg_req, &p_msg_resp);

	if (SGX_SUCCESS != ret) {
		fprintf(OUTPUT, "\nError, ra_network_send_receive() fail [%s].",
			__FUNCTION__);
		SAFE_FREE(p_quote);
		SAFE_FREE(p_msg_req);
		return ret;
	}

	//attestation_report.append((wchar_t *)p_msg_resp->body);
	attestation_report.append((const char*)p_msg_resp->body);

	SAFE_FREE(p_quote);
	SAFE_FREE(p_msg_req);
	SAFE_FREE(p_msg_resp);
	return SGX_SUCCESS;
}

sgx_status_t EnclaveControl::remote_attestation(utility::string_t server_url, utility::string_t runner_addr_s)
{
	sgx_status_t ret = SGX_SUCCESS;
    ra_samp_request_header_t *p_msg0_full = NULL;
    ra_samp_response_header_t *p_msg0_resp_full = NULL;
    ra_samp_request_header_t *p_msg1_full = NULL;
    ra_samp_response_header_t *p_msg2_full = NULL;
    sgx_ra_msg3_t *p_msg3 = NULL;
    ra_samp_response_header_t* p_att_result_msg_full = NULL;
    int busy_retry_time = 4;
    sgx_status_t status = SGX_SUCCESS;
    ra_samp_request_header_t* p_msg3_full = NULL;

	sgx_ra_context_t context = INT_MAX;

	FILE* OUTPUT = stdout;

	ret = enclave_init_ra(enclave_id,
		&status,
		b_pse,
		&context);

	if (SGX_ERROR_ENCLAVE_LOST == ret) {
		ret = SGX_ERROR_UNEXPECTED;
		fprintf(OUTPUT, "\nError, call enclave_init_ra fail [%s].",
			__FUNCTION__);
		return ret;
	}


    // Preparation for remote attestation by configuring extended epid group id.
    {
        uint32_t extended_epid_group_id = 0;
        ret = sgx_get_extended_epid_group_id(&extended_epid_group_id);
        if (SGX_SUCCESS != ret)
        {
            ret = SGX_ERROR_UNEXPECTED;
            fprintf(OUTPUT, "\nError, call sgx_get_extended_epid_group_id fail [%s].",
                __FUNCTION__);
            return ret;
        }
        fprintf(OUTPUT, "\nCall sgx_get_extended_epid_group_id success.");

        p_msg0_full = (ra_samp_request_header_t*)
            malloc(sizeof(ra_samp_request_header_t)
            +sizeof(uint32_t));
        if (NULL == p_msg0_full)
        {
            ret = SGX_ERROR_UNEXPECTED;
            goto CLEANUP;
        }
        p_msg0_full->type = TYPE_RA_MSG0;
        p_msg0_full->size = sizeof(uint32_t);

        *(uint32_t*)((uint8_t*)p_msg0_full + sizeof(ra_samp_request_header_t)) = extended_epid_group_id;
        {

            fprintf(OUTPUT, "\nMSG0 body generated -\n");

            PRINT_BYTE_ARRAY(OUTPUT, p_msg0_full->body, p_msg0_full->size);

        }
        // The ISV application sends msg0 to the SP.
        // The ISV decides whether to support this extended epid group id.
        fprintf(OUTPUT, "\nSending msg0 to remote attestation service provider.\n");

        ret = ra_network_send_receive(server_url, p_msg0_full,
            &p_msg0_resp_full);
        if (ret != 0)
        {
            fprintf(OUTPUT, "\nError, ra_network_send_receive for msg0 failed "
                "[%s].", __FUNCTION__);
            goto CLEANUP;
        }
        fprintf(OUTPUT, "\nSent MSG0 to remote attestation service.\n");
    }
    // Remote attestation will be initiated the ISV server challenges the ISV
    // app or if the ISV app detects it doesn't have the credentials
    // (shared secret) from a previous attestation required for secure
    // communication with the server.
    {
        // gt application call uke sgx_ra_get_msg1
        p_msg1_full = (ra_samp_request_header_t*)
                      malloc(sizeof(ra_samp_request_header_t)
                             + sizeof(sgx_ra_msg1_t));
        if(NULL == p_msg1_full)
        {
            ret = SGX_ERROR_UNEXPECTED;
            goto CLEANUP;
        }
        p_msg1_full->type = TYPE_RA_MSG1;
        p_msg1_full->size = sizeof(sgx_ra_msg1_t);
        do
        {
            ret = sgx_ra_get_msg1(context, enclave_id, sgx_ra_get_ga,
                                  (sgx_ra_msg1_t*)((uint8_t*)p_msg1_full
                                  + sizeof(ra_samp_request_header_t)));
			if (SGX_ERROR_BUSY == ret)
				//Sleep(3 * 1000); // Wait 3s between retries
                                sleep(3);
        } while (SGX_ERROR_BUSY == ret && busy_retry_time--);
        if(SGX_SUCCESS != ret)
        {
            ret = SGX_ERROR_UNEXPECTED;
            fprintf(OUTPUT, "\nError, call sgx_ra_get_msg1 fail [%s].",
                    __FUNCTION__);
            goto CLEANUP;
        }
        else
        {
            fprintf(OUTPUT, "\nCall sgx_ra_get_msg1 success.\n");

            fprintf(OUTPUT, "\nMSG1 body generated -\n");

            PRINT_BYTE_ARRAY(OUTPUT, p_msg1_full->body, p_msg1_full->size);

        }

        // The ISV application sends msg1 to the SP to get msg2,
        // msg2 needs to be freed when no longer needed.
        // The ISV decides whether to use linkable or unlinkable signatures.
        fprintf(OUTPUT, "\nSending msg1 to remote attestation service provider."
                        "Expecting msg2 back.\n");


        ret = ra_network_send_receive(server_url, p_msg1_full,
                                      &p_msg2_full);

        if(ret != 0 || !p_msg2_full)
        {
            fprintf(OUTPUT, "\nError, ra_network_send_receive for msg1 failed "
                            "[%s].", __FUNCTION__);
                goto CLEANUP;
        }
        else
        {
            // Successfully sent msg1 and received a msg2 back.
            // Time now to check msg2.
            if(TYPE_RA_MSG2 != p_msg2_full->type)
            {

                fprintf(OUTPUT, "\nError, didn't get MSG2 in response to MSG1. "
                                "[%s].", __FUNCTION__);
                goto CLEANUP;
            }

            fprintf(OUTPUT, "\nSent MSG1 to remote attestation service "
                            "provider. Received the following MSG2:\n");
            PRINT_BYTE_ARRAY(OUTPUT, p_msg2_full,
                             sizeof(ra_samp_response_header_t)
                             + p_msg2_full->size);

            fprintf(OUTPUT, "\nA more descriptive representation of MSG2:\n");
            PRINT_ATTESTATION_SERVICE_RESPONSE(OUTPUT, p_msg2_full);
        }

        sgx_ra_msg2_t* p_msg2_body = (sgx_ra_msg2_t*)((uint8_t*)p_msg2_full
                                      + sizeof(ra_samp_response_header_t));

        uint32_t msg3_size = 0;
        {
            busy_retry_time = 2;
            // The ISV app now calls uKE sgx_ra_proc_msg2,
            // The ISV app is responsible for freeing the returned p_msg3!!
            do
            {
                ret = sgx_ra_proc_msg2(context,
                                   enclave_id,
                                   sgx_ra_proc_msg2_trusted,
                                   sgx_ra_get_msg3_trusted,
                                   p_msg2_body,
                                   p_msg2_full->size,
                                   &p_msg3,
                                   &msg3_size);
            } while (SGX_ERROR_BUSY == ret && busy_retry_time--);
            if(!p_msg3)
            {
                fprintf(OUTPUT, "\nError, call sgx_ra_proc_msg2 fail. "
                                "p_msg3 = 0x%p [%s].", p_msg3, __FUNCTION__);
                ret = SGX_ERROR_UNEXPECTED;
                goto CLEANUP;
            }
            if(SGX_SUCCESS != (sgx_status_t)ret)
            {
                fprintf(OUTPUT, "\nError, call sgx_ra_proc_msg2 fail. "
                                "ret = 0x%08x [%s].", ret, __FUNCTION__);
                ret = SGX_ERROR_UNEXPECTED;
                goto CLEANUP;
            }
            else
            {
                fprintf(OUTPUT, "\nCall sgx_ra_proc_msg2 success.\n");
				fprintf(OUTPUT, "\nMSG3 - \n");
			}
        }

        PRINT_BYTE_ARRAY(OUTPUT, p_msg3, msg3_size);

        p_msg3_full = (ra_samp_request_header_t*)malloc(
                       sizeof(ra_samp_request_header_t) + msg3_size);
        if(NULL == p_msg3_full)
        {
            ret = SGX_ERROR_UNEXPECTED;
            goto CLEANUP;
        }
        p_msg3_full->type = TYPE_RA_MSG3;
        p_msg3_full->size = msg3_size;
        //if(memcpy_s(p_msg3_full->body, msg3_size, p_msg3, msg3_size))
        if(NULL == memcpy(p_msg3_full->body, p_msg3, msg3_size))
        {
            fprintf(OUTPUT,"\nError: INTERNAL ERROR - memcpy failed in [%s].",
                    __FUNCTION__);
            ret = SGX_ERROR_UNEXPECTED;
            goto CLEANUP;
        }

        // The ISV application sends msg3 to the SP to get the attestation
        // result message, attestation result message needs to be freed when
        // no longer needed. The ISV service provider decides whether to use
        // linkable or unlinkable signatures. The format of the attestation
        // result is up to the service provider. This format is used for
        // demonstration.  Note that the attestation result message makes use
        // of both the MK for the MAC and the SK for the secret. These keys are
        // established from the SIGMA secure channel binding.
        ret = ra_network_send_receive(server_url, p_msg3_full,
                                      &p_att_result_msg_full);
        if(ret || !p_att_result_msg_full)
        {
            ret = SGX_ERROR_UNEXPECTED;
            fprintf(OUTPUT, "\nError, sending msg3 failed [%s].", __FUNCTION__);
            goto CLEANUP;
        }


        sample_ra_att_result_msg_t * p_att_result_msg_body =
            (sample_ra_att_result_msg_t *)((uint8_t*)p_att_result_msg_full
                                           + sizeof(ra_samp_response_header_t));
        if(TYPE_RA_ATT_RESULT != p_att_result_msg_full->type)
        {
            ret = SGX_ERROR_UNEXPECTED;
            fprintf(OUTPUT, "\nError. Sent MSG3 successfully, but the message "
                            "received was NOT of type att_msg_result. Type = "
                            "%d. [%s].", p_att_result_msg_full->type,
                             __FUNCTION__);
            goto CLEANUP;
        }
        else
        {
            fprintf(OUTPUT, "\nSent MSG3 successfully. Received an attestation "
                            "result message back\n.");
        }

        fprintf(OUTPUT, "\nATTESTATION RESULT RECEIVED - ");
        PRINT_BYTE_ARRAY(OUTPUT, p_att_result_msg_full->body,
                         p_att_result_msg_full->size);


        // Check the MAC using MK on the attestation result message.
        // The format of the attestation result message is ISV specific.
        // This is a simple form for demonstration. In a real product,
        // the ISV may want to communicate more information.
        ret = verify_att_result_mac(enclave_id,
                &status,
                context,
                (uint8_t*)&p_att_result_msg_body->platform_info_blob,
                sizeof(ias_platform_info_blob_t),
                (uint8_t*)&p_att_result_msg_body->mac,
                sizeof(sgx_mac_t));
        if((SGX_SUCCESS != ret) ||
           (SGX_SUCCESS != status))
        {
            ret = SGX_ERROR_UNEXPECTED;
            fprintf(OUTPUT, "\nError: INTEGRITY FAILED - attestation result "
                            "message MK based cmac failed in [%s].",
                            __FUNCTION__);
            goto CLEANUP;
        }

        bool attestation_passed = true;
        // Check the attestation result for pass or fail.
        // Whether attestation passes or fails is a decision made by the ISV Server.
        // When the ISV server decides to trust the enclave, then it will return success.
        // When the ISV server decided to not trust the enclave, then it will return failure.
        if(0 != p_att_result_msg_full->status[0]
           || 0 != p_att_result_msg_full->status[1])
        {
            fprintf(OUTPUT, "\nError, attestation result message MK based cmac "
                            "failed in [%s].", __FUNCTION__);
            attestation_passed = false;
        }

        // The attestation result message should contain a field for the Platform
        // Info Blob (PIB).  The PIB is returned by the IAS in the attestation report.
        // It is not returned in all cases, but when it is, the ISV app
        // should pass it to the blob analysis API called sgx_report_attestation_status()
        // along with the trust decision from the ISV server.
        // The ISV application will take action based on the update_info.
        // returned in update_info by the API.  
        // This call is stubbed out for the sample.
        // 
        // sgx_update_info_bit_t update_info;
        // ret = sgx_report_attestation_status(
        //     &p_att_result_msg_body->platform_info_blob,
        //     attestation_passed ? 0 : 1, &update_info);

        // Get the shared secret sent by the server using SK (if attestation
        // passed)
        if(attestation_passed)
        {
            ret = put_secret_data(enclave_id,
                                  &status,
                                  context,
                                  p_att_result_msg_body->secret.payload,
                                  p_att_result_msg_body->secret.payload_size,
                                  p_att_result_msg_body->secret.payload_tag);
            if((SGX_SUCCESS != ret)  || (SGX_SUCCESS != status))
            {
                fprintf(OUTPUT, "\nError, attestation result message secret "
                                "using SK based AESGCM failed in [%s]. ret = "
                                "0x%0x. status = 0x%0x", __FUNCTION__, ret,
                                 status);
                goto CLEANUP;
            }
        }
        fprintf(OUTPUT, "\nSecret successfully received from server.");
        fprintf(OUTPUT, "\nRemote attestation success!");
    }

CLEANUP:
    // Clean-up
    // Need to close the RA key state.
    if(INT_MAX != context)
    {
        sgx_status_t ret_save = ret;
        ret = enclave_ra_close(enclave_id, &status, context);
        if(SGX_SUCCESS != ret || status)
        {
            ret = SGX_ERROR_UNEXPECTED;
            fprintf(OUTPUT, "\nError, call enclave_ra_close fail [%s].",
                    __FUNCTION__);
        }
        else
        {
            // enclave_ra_close was successful, let's restore the value that
            // led us to this point in the code.
            ret = ret_save;
        }
        fprintf(OUTPUT, "\nCall enclave_ra_close success.");
    }

    //sgx_destroy_enclave(enclave_id);


    ra_free_network_response_buffer(p_msg0_resp_full);
    ra_free_network_response_buffer(p_msg2_full);
    ra_free_network_response_buffer(p_att_result_msg_full);

    // p_msg3 is malloc'd by the untrusted KE library. App needs to free.
    SAFE_FREE(p_msg3);
    SAFE_FREE(p_msg3_full);
    SAFE_FREE(p_msg1_full);
    SAFE_FREE(p_msg0_full);
    //printf("\nEnter a character before exit ...\n");
    //getchar();
    return ret;
}

