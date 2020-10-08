#include "network_dealer.h"
#include "service_provider.h"
#include "isa_service.h"
#include "../gt_app/include/tools.h"

// Needed to call untrusted key exchange library APIs, i.e. sgx_ra_proc_msg2.
#include "sgx_ukey_exchange.h"
#ifndef SAFE_FREE
#define SAFE_FREE(ptr) {if (NULL != (ptr)) {free(ptr); (ptr) = NULL;}}
#endif



NetworkDealer::NetworkDealer(utility::string_t url) : m_listener(url)
{
    m_listener.support(methods::GET, std::bind(&NetworkDealer::handle_get, this, std::placeholders::_1));
    m_listener.support(methods::PUT, std::bind(&NetworkDealer::handle_put, this, std::placeholders::_1));
    m_listener.support(methods::POST, std::bind(&NetworkDealer::handle_post, this, std::placeholders::_1));
    m_listener.support(methods::DEL, std::bind(&NetworkDealer::handle_delete, this, std::placeholders::_1));
	m_listener.support(methods::OPTIONS, std::bind(&NetworkDealer::handle_options, this, std::placeholders::_1));
}

void PRINT_BYTE_ARRAY(void *mem, uint32_t len)
{
	if (!mem || !len)
	{
		ucout << "\n( null )\n";
		return;
	}
	uint8_t *array = (uint8_t *)mem;
	ucout << len << " bytes:\n{\n";
	uint32_t i = 0;
	for (i = 0; i < len - 1; i++)
	{
		ucout << "0x" <<array[i] << ", ";
		if (i % 8 == 7) 
			ucout << endl;
	}
	ucout << "0x" << array[i] << " \n}\n";
}

//
// A GET of the dealer resource produces a list of existing tables.
// 
void NetworkDealer::handle_options(http_request message)
{
	ucout << "Options handler:"<< message.to_string() << endl;

	auto paths = http::uri::split_path(http::uri::decode(message.relative_uri().path()));
	if (paths.empty())
	{
		message.reply(status_codes::OK/*,some json*/);
		return;
	}
	message.reply(status_codes::OK, U("OK"));
};
/*
void details::http_listener_impl::handle_options(http_request message)
{
http_response response(status_codes::OK);
response.headers().add(U("Allow"), get_supported_methods());
message.reply(response);
}
*/
//
// A GET of the dealer resource produces a list of existing tables.
// 
void NetworkDealer::handle_get(http_request message)
{
    ucout << "Get handler:" << message.to_string() << endl;

	http_response response(status_codes::OK);
	response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
	response.set_body(U("OK"));
	message.reply(response);
	return;
};

//
// A POST of the dealer resource creates a new table and returns a resource for
// that table.
// 
void NetworkDealer::handle_post(http_request message)
{
	int ret = 0;
	ra_samp_response_header_t* p_resp_msg;

    ucout << "Post handler:" << message.to_string() << endl;

	json::value obj = message.extract_json().get();

	ucout << U("JSON received:") << obj.serialize() << endl;

	uint32_t type = obj.at(U("msgType")).as_integer();

	switch (type) {
	case TYPE_RA_SIMPLE:{
		json::value a_report;
		ret = verify_attestation_evidence(obj.at(U("quote")).as_string(),a_report);
		if (ret) {
			ucout << U("Attestation quote fail.") << endl;
			message.reply(status_codes::InternalError);
			break;
		}

		json::value rply_obj;
		size_t report_size = a_report.serialize().length();
		rply_obj[U("msgType")] = json::value::number(TYPE_RA_SRESP);
		rply_obj[U("msgStatus0")] = json::value::number(0);
		rply_obj[U("msgStatus1")] = json::value::number(0);
		rply_obj[U("msgSize")] = json::value::number(report_size);
		rply_obj[U("attestationResult")] = a_report;

		ucout << U("JSON generated:") << rply_obj.serialize() << endl;

		message.reply(status_codes::OK, rply_obj);
		break;
	}
	case TYPE_RA_MSG0: {
		ra_samp_request_header_t *p_msg0_full = NULL;
		//ra_samp_response_header_t *p_msg0_resp_full = NULL;
		p_msg0_full = (ra_samp_request_header_t*)
			malloc(sizeof(ra_samp_request_header_t)
				+ sizeof(uint32_t));
		if (NULL == p_msg0_full)
		{
			ret = -1;
			message.reply(status_codes::InternalError);
			break;
		}
		p_msg0_full->type = TYPE_RA_MSG0;
		p_msg0_full->size = sizeof(uint32_t);

		*(uint32_t*)((uint8_t*)p_msg0_full + sizeof(ra_samp_request_header_t)) =
			obj.at(U("sgxExtendedEpidGroupId")).as_integer();
		ucout << "\nMSG0 body generated -\n";
		PRINT_BYTE_ARRAY(p_msg0_full->body, p_msg0_full->size);
		ret = sp_ra_proc_msg0_req((const sample_ra_msg0_t*)((uint8_t*)p_msg0_full
			+ sizeof(ra_samp_request_header_t)),
			p_msg0_full->size);
		if (0 != ret)
		{
			fprintf(stderr, "\nError, call sp_ra_proc_msg0_req fail [%s].",
				__FUNCTION__);
			message.reply(status_codes::Forbidden);
		}
		else {
			message.reply(status_codes::OK);
		}
		SAFE_FREE(p_msg0_full);
		break;
	}
	case TYPE_RA_MSG1: {
		ra_samp_request_header_t *p_msg1_full = NULL;
		p_msg1_full = (ra_samp_request_header_t*)
			malloc(sizeof(ra_samp_request_header_t)
				+ sizeof(sgx_ra_msg1_t));
		if (NULL == p_msg1_full)
		{
			ret = -1;
			break;
		}
		p_msg1_full->type = TYPE_RA_MSG1;
		p_msg1_full->size = sizeof(sgx_ra_msg1_t);

		sgx_ra_msg1_t *msg1 = (sgx_ra_msg1_t *)((uint8_t*)p_msg1_full + sizeof(ra_samp_request_header_t));

		myHex2Bin(obj.at(U("sgxGroupId")).as_string(), 4, msg1->gid);
		myHex2Bin(obj.at(U("sgxEcPubGx")).as_string(), SGX_ECP256_KEY_SIZE, msg1->g_a.gx);
		myHex2Bin(obj.at(U("sgxEcPubGy")).as_string(), SGX_ECP256_KEY_SIZE, msg1->g_a.gy);

		ucout << "\nMSG1 body received -\n";
		PRINT_BYTE_ARRAY(p_msg1_full->body, p_msg1_full->size);
		ret = sp_ra_proc_msg1_req((const sample_ra_msg1_t*)((uint8_t*)p_msg1_full
			+ sizeof(ra_samp_request_header_t)),
			p_msg1_full->size,
			&p_resp_msg);
		if (0 != ret)
		{
			fprintf(stderr, "\nError, call sp_ra_proc_msg1_req fail [%s].",
				__FUNCTION__);
			message.reply(status_codes::Forbidden);
			SAFE_FREE(p_msg1_full);
			break;
		}
		sample_ra_msg2_t *p_msg2 = (sample_ra_msg2_t *)p_resp_msg->body;
		json::value rply_obj;
		rply_obj[U("msgType")] = json::value::number(p_resp_msg->type);
		rply_obj[U("msgStatus0")] = json::value::number(p_resp_msg->status[0]);
		rply_obj[U("msgStatus1")] = json::value::number(p_resp_msg->status[1]);
		rply_obj[U("msgSize")] = json::value::number(p_resp_msg->size);
		rply_obj[U("sgxEcPubGx")] = json::value(mBin2Hex(p_msg2->g_b.gx, SAMPLE_ECP_KEY_SIZE));
		rply_obj[U("sgxEcPubGy")] = json::value(mBin2Hex(p_msg2->g_b.gy, SAMPLE_ECP_KEY_SIZE));
		rply_obj[U("sgxSpid")] = json::value(mBin2Hex(p_msg2->spid.id, 16));
		rply_obj[U("sgxQuoteType")] = json::value::number(p_msg2->quote_type);
		rply_obj[U("sgxKdfId")] = json::value::number(p_msg2->kdf_id);
		rply_obj[U("sgxSignGbGaX")] = json::value(mBin2Hex((uint8_t *)p_msg2->sign_gb_ga.x, SAMPLE_NISTP256_KEY_SIZE * sizeof(uint32_t)));
		rply_obj[U("sgxSignGbGaY")] = json::value(mBin2Hex((uint8_t *)p_msg2->sign_gb_ga.y, SAMPLE_NISTP256_KEY_SIZE * sizeof(uint32_t)));
		rply_obj[U("sgxMac")] = json::value(mBin2Hex(p_msg2->mac, SAMPLE_MAC_SIZE));
		rply_obj[U("sgxSigRl")] = json::value(mBin2Hex(p_msg2->sig_rl, p_msg2->sig_rl_size));
		rply_obj[U("sgxSigRlSize")] = json::value::number(p_msg2->sig_rl_size);

		ucout << U("JSON generated:") << rply_obj.serialize() << endl;
		SAFE_FREE(p_resp_msg);
		SAFE_FREE(p_msg1_full);
		message.reply(status_codes::OK,rply_obj);
		break;
	  }
	case TYPE_RA_MSG3: {
		ra_samp_request_header_t *p_msg3_full = NULL;
		uint32_t msg3_size = obj.at(U("msgSize")).as_integer();
		p_msg3_full = (ra_samp_request_header_t*)
			malloc(sizeof(ra_samp_request_header_t)
				+ msg3_size);
		if (NULL == p_msg3_full)
		{
			ret = -1;
			break;
		}
		p_msg3_full->type = TYPE_RA_MSG1;
		p_msg3_full->size = msg3_size;

		sgx_ra_msg3_t *msg3 = (sgx_ra_msg3_t *)((uint8_t*)p_msg3_full + sizeof(ra_samp_request_header_t));

		myHex2Bin(obj.at(U("sgxMac")).as_string(), SAMPLE_MAC_SIZE, msg3->mac);
		myHex2Bin(obj.at(U("sgxGaGx")).as_string(), SAMPLE_ECP_KEY_SIZE, msg3->g_a.gx);
		myHex2Bin(obj.at(U("sgxGaGy")).as_string(), SAMPLE_ECP_KEY_SIZE, msg3->g_a.gy);
		myHex2Bin(obj.at(U("sgxPropDesc")).as_string(), 256, msg3->ps_sec_prop.sgx_ps_sec_prop_desc);
		myHex2Bin(obj.at(U("sgxQuote")).as_string(), msg3_size - SAMPLE_MAC_SIZE - 2 * SAMPLE_ECP_KEY_SIZE - 256, msg3->quote);

		ucout << "\nMSG3 body received -\n";
		PRINT_BYTE_ARRAY(p_msg3_full->body, p_msg3_full->size);
		ret = sp_ra_proc_msg3_req((const sample_ra_msg3_t*)((uint8_t*)p_msg3_full
			+ sizeof(ra_samp_request_header_t)),
			p_msg3_full->size,
			&p_resp_msg);
		if (0 != ret)
		{
			fprintf(stderr, "\nError, call sp_ra_proc_msg1_req fail [%s].",
				__FUNCTION__);
			message.reply(status_codes::Forbidden);
			SAFE_FREE(p_msg3_full);
			break;
		}
		sample_ra_att_result_msg_t *p_att_result_msg = NULL;

		p_att_result_msg = (sample_ra_att_result_msg_t *)p_resp_msg->body;
		json::value rply_obj;
		rply_obj[U("msgType")] = json::value::number(p_resp_msg->type);
		rply_obj[U("msgStatus0")] = json::value::number(p_resp_msg->status[0]);
		rply_obj[U("msgStatus1")] = json::value::number(p_resp_msg->status[1]);
		rply_obj[U("msgSize")] = json::value::number(p_resp_msg->size);
		rply_obj[U("sgxPlatformInfo")] = json::value(mBin2Hex(p_att_result_msg->platform_info_blob.platform_info, SAMPLE_PLATFORM_INFO_SIZE));
		rply_obj[U("sgxMac")] = json::value(mBin2Hex(p_att_result_msg->mac, SAMPLE_MAC_SIZE));
		rply_obj[U("sgxSecretPldSize")] = json::value(p_att_result_msg->secret.payload_size);
		rply_obj[U("sgxSecretPldTag")] = json::value(mBin2Hex(p_att_result_msg->secret.payload_tag, SAMPLE_SP_TAG_SIZE));
		rply_obj[U("sgxSecretPld")] = json::value(mBin2Hex(p_att_result_msg->secret.payload,
			p_att_result_msg->secret.payload_size));

		ucout << U("JSON generated:") << rply_obj.serialize() << endl;
		SAFE_FREE(p_resp_msg);
		SAFE_FREE(p_msg3_full);
		message.reply(status_codes::OK, rply_obj);
		break;
	  }
	default:
		ret = -1;
		fprintf(stderr, "\nError, unknown ra message type. Type = %d [%s].", 
			type, __FUNCTION__);
		break;
	}
};

//
// A DELETE of the player resource leaves the table.
// 
void NetworkDealer::handle_delete(http_request message)
{
    ucout << "Delete handler:" << message.to_string() << endl;

    message.reply(status_codes::Forbidden, U("Player name is required in query"));
};


//
// A PUT to a table resource makes a card request (hit / stay).
// 
void NetworkDealer::handle_put(http_request message)
{
    ucout << "Put handler:" << message.to_string() << endl;

    message.reply(status_codes::Forbidden, U("Unrecognized request"));
};
