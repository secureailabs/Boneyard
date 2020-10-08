#include "stdafx.h"
#include "tools.h"

//for sleep()
#include <unistd.h>
//#include <stdio.h>
//#include <limits.h>
// Needed for definition of remote attestation messages.
//#include "remote_attestation_result.h"

//for string_t
using namespace utility;

//#include "../gt_enclave_u.h"
#include "ecalls.h"

// Needed to call untrusted key exchange library APIs, i.e. sgx_ra_proc_msg2.
#include "sgx_ukey_exchange.h"

// In addition to generating and sending messages, this application
// can use pre-generated messages to verify the generation of
// messages and the information flow.
//#include "sample_messages.h"

// Needed to get service provider's information, in your real project, you will
// need to talk to real server.
//#include "network_ra.h"
//#include "ra_network.h"

#include "enclave_control.h"

// Needed to create enclave and do ecall.
#include "sgx_urts.h"
#include "../gt_enclave_u.h"

// Needed to query extended epid group id.
#include "sgx_uae_epid.h"

#ifndef SAFE_FREE
#define SAFE_FREE(ptr) {if (NULL != (ptr)) {free(ptr); (ptr) = NULL;}}
#endif

#include "ra_msg.h"
//#include "ra_network.h"
#include "tools.h"

using namespace std;
using namespace web;
using namespace utility;
using namespace http;
using namespace web::http::experimental::listener;

sgx_status_t oc_init_quote(sgx_target_info_t *pTarget_info, sgx_epid_group_id_t *pGid){
	sgx_status_t ret = SGX_SUCCESS;

	ret = sgx_init_quote(pTarget_info, pGid);
	return ret;
}
// @param server_url String name of the server URL
// @param p_req Pointer to the message to be sent.
// @param p_resp Pointer to a pointer of the response message.

// @return int
sgx_status_t ra_network_send_receive(utility::string_t server_url, const ra_samp_request_header_t *p_req,
	ra_samp_response_header_t **p_resp)
{
	sgx_status_t ret = SGX_SUCCESS;

	utility::ostringstream_t buf;

	std::string strEpidGrp;
	http_response response;
	http::status_code status_c;

	json::value obj;

	utility::string_t address = server_url;
	utility::string_t path = U("attestation");
	utility::string_t content_type = U("application/json");
	//utility::string_t address = U("http://localhost:4200/attestation"); //server_url

	http::uri uri = http::uri(address);

	web::http::client::http_client gtServiceProvider(http::uri_builder(uri).to_uri());

	if ((NULL == p_req) ||
		(NULL == p_resp))
	{
		return SGX_ERROR_UNEXPECTED;
	}

	vector<unsigned char> cv((const unsigned char *)(p_req->body),
		(const unsigned char *)(p_req->body)+p_req->size);

	switch (p_req->type)
	{
	case TYPE_RA_SIMPLE:
		obj[U("msgType")] = json::value::number(TYPE_RA_SIMPLE);
		obj[U("msgSize")] = json::value::number(p_req->size);
		obj[U("quote")] = json::value::string(utility::conversions::to_base64(cv));

		response = gtServiceProvider.request(methods::POST,path,obj).get();

		//ucout << response.to_string() << endl;

		status_c = response.status_code();

		if (status_c != 200)
		{
			fprintf(stderr, "\nError, simple_ra request fail [%s].",
				__FUNCTION__);
			ret = SGX_ERROR_UNEXPECTED;
		}
		else {
			obj = response.extract_json().get();
			//ucout << U("JSON received:") << obj.serialize() << endl;
			//int msg_size = obj.at(U("msgSize")).as_integer();
			utility::string_t report = obj.at(U("attestationResult")).serialize();
                        int msg_size = report.length();

			ra_samp_response_header_t *p_resp_msg = (ra_samp_response_header_t*)malloc(
				(msg_size + 2)*sizeof(wchar_t) +
				sizeof(ra_samp_response_header_t));
			if (!p_resp_msg)
			{
				fprintf(stderr, "\nError, out of memory in [%s].", __FUNCTION__);
				ret = SGX_ERROR_UNEXPECTED;
				break;
			}
			p_resp_msg->type = (uint8_t)obj.at(U("msgType")).as_integer();
			p_resp_msg->status[0] = 0;//uint8_t)obj.at(U("msgStatus0")).as_integer();
			p_resp_msg->status[1] = 0;//(uint8_t)obj.at(U("msgStatus1")).as_integer();
			p_resp_msg->size = msg_size;

			//wcsncpy_s((wchar_t*)p_resp_msg->body, msg_size+1, report.c_str(), msg_size);
			strncpy((char *)p_resp_msg->body, report.c_str(), msg_size);

			*p_resp = p_resp_msg;
		}
		break;
	default:
		ret = SGX_ERROR_UNEXPECTED;
		fprintf(stderr, "\nError, unknown ra message type. Type = %d [%s].",
			p_req->type, __FUNCTION__);
		break;
	}

	return ret;
}

// Used to free the response messages.  In the sample code, the
// response messages are allocated by the SP code.
//
//
// @param resp Pointer to the response buffer to be freed.

void ra_free_network_response_buffer(ra_samp_response_header_t *resp)
{
	if (resp != NULL)
	{
		free(resp);
	}
}

sgx_status_t oc_sendAttReq(const char * server_url, sgx_report_t *pReport, sgx_spid_t *pSpid_a,
		char *attestation_report, size_t reportSize){
	sgx_status_t ret, status;
	sgx_target_info_t target_info;
	sgx_epid_group_id_t gid;
	uint32_t quote_size;
	sgx_quote_t *p_quote;

	FILE* OUTPUT = stdout;

	ret = sgx_init_quote(&target_info, &gid);

	if (SGX_SUCCESS != ret) {
		fprintf(OUTPUT, "\nError, call sgx_init_quote() fail [%s].",
			__FUNCTION__);
		return ret;
	}
	ret = call_ec_get_report(&status, &target_info, pReport);

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

	ret = sgx_get_quote(pReport,
						SGX_LINKABLE_SIGNATURE,
						pSpid_a,
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
	strncpy(attestation_report, (const char*)p_msg_resp->body, reportSize);

	SAFE_FREE(p_quote);
	SAFE_FREE(p_msg_req);
	SAFE_FREE(p_msg_resp);
	return SGX_SUCCESS;

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
/*
sgx_status_t simple_ra(const utility::string_t & server_url,
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

	ret = call_ec_get_report(&status, &target_info, &report);

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
*/
