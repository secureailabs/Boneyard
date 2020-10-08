#include "../include/ra_network.h"
#include "../include/tools.h"

using namespace std;
using namespace web;
using namespace utility;
using namespace http;
using namespace web::http::experimental::listener;

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
	sample_ra_msg0_t *ra_msg0 = NULL;
	sample_ra_msg1_t *ra_msg1 = NULL;
	sample_ra_msg3_t *ra_msg3 = NULL;

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

		ucout << response.to_string() << endl;

		status_c = response.status_code();

		if (status_c != 200)
		{
			fprintf(stderr, "\nError, simple_ra request fail [%s].",
				__FUNCTION__);
			ret = SGX_ERROR_UNEXPECTED;
		}
		else {
			obj = response.extract_json().get();
			ucout << U("JSON received:") << obj.serialize() << endl;
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
	case TYPE_RA_MSG0:
		obj[U("msgType")] = json::value::number(TYPE_RA_MSG0);
		if (p_req->size != sizeof(sample_ra_msg0_t))
		{
			ret = SGX_ERROR_UNEXPECTED;
			break;
		}
		ra_msg0 = (sample_ra_msg0_t *)((uint8_t*)p_req + sizeof(ra_samp_request_header_t));

		obj[U("sgxExtendedEpidGroupId")] = json::value::number(ra_msg0->extended_epid_group_id);
		buf.clear();

		response = gtServiceProvider.request(methods::POST, buf.str(), obj).get();

		ucout << response.to_string() << endl;

		status_c = response.status_code();

		if (status_c!=200)
		{
			fprintf(stderr, "\nError, msg0_req message fail [%s].",
				__FUNCTION__);
			ret = SGX_ERROR_UNEXPECTED;
		}
		break;

	case TYPE_RA_MSG1:
		//ret = sp_ra_proc_msg1_req((const sample_ra_msg1_t*)((uint8_t*)p_req
		//	+ sizeof(ra_samp_request_header_t)),
		//	p_req->size,
		//	&p_resp_msg);
		obj[U("msgType")] = json::value::number(1);
		if (p_req->size != sizeof(sample_ra_msg1_t))
		{
			ret = SGX_ERROR_UNEXPECTED;
			break;
		}
		ra_msg1 = (sample_ra_msg1_t *)((uint8_t*)p_req + sizeof(ra_samp_request_header_t));

		obj[U("sgxGroupId")] = json::value(mBin2Hex(ra_msg1->gid,sizeof(epid_group_id_t)));
		obj[U("sgxEcPubGx")] = json::value(mBin2Hex(ra_msg1->g_a.gx, SAMPLE_ECP_KEY_SIZE));
		obj[U("sgxEcPubGy")] = json::value(mBin2Hex(ra_msg1->g_a.gy, SAMPLE_ECP_KEY_SIZE));

		ucout << U("JSON generated:") << obj.serialize() << endl;

		buf.clear();

		response = gtServiceProvider.request(methods::POST, buf.str(), obj).get();

		ucout << response.to_string() << endl;

		status_c = response.status_code();

		if (200 != status_c)
		{
			fprintf(stderr, "\nError, msg1_req fail [%s].",
				__FUNCTION__);
			ret = SGX_ERROR_UNEXPECTED;
		}
		else
		{
			obj = response.extract_json().get();
			ucout << U("JSON received:") << obj.serialize() << endl;
			int sig_rl_size = obj.at(U("sgxSigRlSize")).as_integer();
			uint32_t msg2_size = sizeof(sample_ra_msg2_t) + sig_rl_size;
			ra_samp_response_header_t *p_msg2_full = (ra_samp_response_header_t*)malloc(msg2_size
				+ sizeof(ra_samp_response_header_t));
			if (!p_msg2_full)
			{
				fprintf(stderr, "\nError, out of memory in [%s].", __FUNCTION__);
				ret = SGX_ERROR_UNEXPECTED;
				break;
			}
			p_msg2_full->type = (uint8_t)obj.at(U("msgType")).as_integer();
			p_msg2_full->status[0] = (uint8_t)obj.at(U("msgStatus0")).as_integer();
			p_msg2_full->status[1] = (uint8_t)obj.at(U("msgStatus1")).as_integer();
			p_msg2_full->size = obj.at(U("msgSize")).as_integer();
			sample_ra_msg2_t *p_msg2 = (sample_ra_msg2_t *)p_msg2_full->body;
			myHex2Bin(obj.at(U("sgxEcPubGx")).as_string(), SAMPLE_ECP_KEY_SIZE, p_msg2->g_b.gx);
			myHex2Bin(obj.at(U("sgxEcPubGy")).as_string(), SAMPLE_ECP_KEY_SIZE, p_msg2->g_b.gy);
			myHex2Bin(obj.at(U("sgxSpid")).as_string(), 16, p_msg2->spid.id);
			
			p_msg2->quote_type = (uint16_t)obj.at(U("sgxQuoteType")).as_integer();
			p_msg2->kdf_id = (uint16_t)obj.at(U("sgxKdfId")).as_integer();

			myHex2Bin(obj.at(U("sgxSignGbGaX")).as_string(), SAMPLE_NISTP256_KEY_SIZE * 4, (uint8_t *)(p_msg2->sign_gb_ga.x));
			myHex2Bin(obj.at(U("sgxSignGbGaY")).as_string(), SAMPLE_NISTP256_KEY_SIZE * 4, (uint8_t *)(p_msg2->sign_gb_ga.y));
			myHex2Bin(obj.at(U("sgxMac")).as_string(), SAMPLE_MAC_SIZE, p_msg2->mac);
			myHex2Bin(obj.at(U("sgxSigRl")).as_string(), sig_rl_size, p_msg2->sig_rl);
			p_msg2->sig_rl_size = sig_rl_size;
			
			*p_resp = p_msg2_full;
		}
		break;

	case TYPE_RA_MSG3:
		//ret = sp_ra_proc_msg3_req((const sample_ra_msg3_t*)((uint8_t*)p_req +
		//	sizeof(ra_samp_request_header_t)),
		//	p_req->size,
		//	&p_resp_msg);
		obj[U("msgType")] = json::value::number(p_req->type);
		obj[U("msgSize")] = json::value::number(p_req->size);
		ra_msg3 = (sample_ra_msg3_t *)((uint8_t*)p_req + sizeof(ra_samp_request_header_t));

		obj[U("sgxMac")] = json::value(mBin2Hex(ra_msg3->mac, SAMPLE_MAC_SIZE));
		obj[U("sgxGaGx")] = json::value(mBin2Hex(ra_msg3->g_a.gx, SAMPLE_ECP_KEY_SIZE));
		obj[U("sgxGaGy")] = json::value(mBin2Hex(ra_msg3->g_a.gy, SAMPLE_ECP_KEY_SIZE));
		obj[U("sgxPropDesc")] = json::value(mBin2Hex(ra_msg3->ps_sec_prop.sample_ps_sec_prop_desc, 256));
		obj[U("sgxQuote")] = json::value(mBin2Hex(ra_msg3->quote, p_req->size- SAMPLE_MAC_SIZE-2* SAMPLE_ECP_KEY_SIZE-256));

		ucout << U("JSON generated:") << obj.serialize() << endl;

		buf.clear();

		response = gtServiceProvider.request(methods::POST, buf.str(), obj).get();

		ucout << response.to_string() << endl;

		status_c = response.status_code();

		if (200 != status_c)
		{
			fprintf(stderr, "\nError, msg3_req fail [%s].",
				__FUNCTION__);
			ret = SGX_ERROR_UNEXPECTED;
		}
		else
		{
			obj = response.extract_json().get();
			ucout << U("JSON received:") << obj.serialize() << endl;
			int msg_size = obj.at(U("msgSize")).as_integer();
			int secretPldSize = obj.at(U("sgxSecretPldSize")).as_integer();
			ra_samp_response_header_t *p_resp_msg = (ra_samp_response_header_t*)malloc(msg_size
				+ sizeof(ra_samp_response_header_t)+ secretPldSize);
			if (!p_resp_msg)
			{
				fprintf(stderr, "\nError, out of memory in [%s].", __FUNCTION__);
				ret = SGX_ERROR_UNEXPECTED;
				break;
			}
			p_resp_msg->type = (uint8_t)obj.at(U("msgType")).as_integer();
			p_resp_msg->status[0] = (uint8_t)obj.at(U("msgStatus0")).as_integer();
			p_resp_msg->status[1] = (uint8_t)obj.at(U("msgStatus1")).as_integer();
			p_resp_msg->size = msg_size;
			sample_ra_att_result_msg_t *p_att_result_msg = (sample_ra_att_result_msg_t *)p_resp_msg->body;
			myHex2Bin(obj.at(U("sgxPlatformInfo")).as_string(), SAMPLE_PLATFORM_INFO_SIZE, 
				p_att_result_msg->platform_info_blob.platform_info);
			myHex2Bin(obj.at(U("sgxMac")).as_string(), SAMPLE_MAC_SIZE, p_att_result_msg->mac);
			p_att_result_msg->secret.payload_size = secretPldSize;
			myHex2Bin(obj.at(U("sgxSecretPldTag")).as_string(), SAMPLE_SP_TAG_SIZE, p_att_result_msg->secret.payload_tag);
			myHex2Bin(obj.at(U("sgxSecretPld")).as_string(), 
				p_att_result_msg->secret.payload_size, 
				p_att_result_msg->secret.payload);

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
