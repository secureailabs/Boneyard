#include "../include/stdafx.h"
#include "../include/enclave_control.h"
#include "../include/network_listener.h"
#include "../include/query.h"
#include "../gt_enclave_u.h"
#include "ai.h"
#include "stdlib.h"

int access_code = 100;

NetworkListener::NetworkListener(utility::string_t url) : m_listener(url)
{
	m_listener.support(methods::GET, std::bind(&NetworkListener::handle_get, this, std::placeholders::_1));
	m_listener.support(methods::PUT, std::bind(&NetworkListener::handle_put, this, std::placeholders::_1));
	m_listener.support(methods::POST, std::bind(&NetworkListener::handle_post, this, std::placeholders::_1));
	m_listener.support(methods::DEL, std::bind(&NetworkListener::handle_delete, this, std::placeholders::_1));
	m_listener.support(methods::OPTIONS, std::bind(&NetworkListener::handle_options, this, std::placeholders::_1));
}

void NetworkListener::handle_options(http_request message)
{
	ucout << "Options handler:" << message.to_string() << endl;

	http_response response(status_codes::OK);
	response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
	//response.headers().add(U("Access-Control-Allow-Methods"), U("OPTIONS, GET, PUT, POST, DELETE"));
	response.headers().add(U("Access-Control-Allow-Headers"), U("X-PINGOTHER, Content-Type"));
	response.set_body(U("OK"));
	message.reply(response);
};

//
// .
// 
void NetworkListener::handle_get(http_request message)
{
	ucout << message.to_string() << endl;

	json::value obj = message.extract_json().get();

	ucout << U("JSON received:") << obj.serialize() << endl;

	if (obj.is_null()) {
		http_response response(status_codes::BadRequest);
		response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
		response.headers().add(U("Access-Control-Allow-Methods"), U("OPTIONS, GET, PUT, POST, DELETE"));
		//response.set_body(rply_obj);
		message.reply(response);
		return;
	}
	uint32_t type = obj.at(U("type")).as_integer();
	json::value rply_obj;

	switch (type) {
	case GET_ACCESS_CODE: {
		rply_obj[U("access_code")] = json::value::number(access_code++);
		rply_obj[U("prediction")] = json::value(U(""));
		break;
	}

	case GET_PREDICTION: {
		rply_obj[U("access_code")] = obj.at(U("access_code")).as_integer();
		rply_obj[U("prediction")] = json::value(U("Tempory prediction: 60% risk of the rare disease"));
		break;
	}
	default:
		http_response response(status_codes::BadRequest);
		response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
		response.headers().add(U("Access-Control-Allow-Methods"), U("OPTIONS, GET, PUT, POST, DELETE"));
		//response.set_body(rply_obj);
		message.reply(response);
		return;
		break;
	}
	http_response response(status_codes::OK);
	response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
	response.headers().add(U("Access-Control-Allow-Methods"), U("OPTIONS, GET, PUT, POST, DELETE"));
	response.set_body(rply_obj);
	message.reply(response);
};

//
// .
// 
void NetworkListener::handle_post(http_request message)
{
	//ucout << message.to_string() << endl;

	json::value obj = message.extract_json().get();

	//ucout << U("JSON received:") << obj.serialize() << endl;

	if (obj.is_null()) {
		http_response response(status_codes::BadRequest);
		response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
		response.set_body(U("No JSON in the body"));
		message.reply(response);
		return;
	}

	auto cmd = obj.at(U("command")).as_string();

	ucout << U("Received command: ") << cmd << endl;
	json::value rply_obj;
	int ret = 0;
	if (!cmd.compare(U("access_code"))) {
		//utility::string_t param_s = obj.at(U("parameters")).as_string();
		rply_obj[U("access_code")] = json::value::number(access_code++);
		rply_obj[U("prediction")] = json::value(U(""));
		//result = rply_obj.serialize();
	}
	else if (!cmd.compare(U("get_prediction"))) {
		json::value params = obj.at(U("parameters"));
		string_t data = params.at(U("data")).as_string();
		double result;

		string_t model_id = params.at(U("model_id")).as_string();
		wchar_t wstr[200];
		char ret_str[200];// 1(U("Tempory prediction : "));

		if (U("0") == model_id) {
			//for outside enclave
			vector<unsigned char> char_vec = utility::conversions::from_base64(data);
			string_t data_str(char_vec.begin(), char_vec.end());
			json::value data_json = json::value::parse(data_str);
			result = ai_ad(data_json);

			//char * as parmeter
			//result = ai_ad((char *)&char_vec[0]);

			//for enclave
			//sgx_status_t status = SGX_SUCCESS;
			//ret = get_prediction(enclave.getEnclaveId(), &status,
			//	(char *)&char_vec[0], char_vec.size(), &result);
			//result = ai_ad(data);
			rply_obj[U("access_code")] = params.at(U("access_code")).as_integer();
			sprintf(ret_str, "The prediction result is: %1.2f %% risk of the disease", result * 100);
			//size_t retval;
			//mbstowcs_s(&retval, wstr, 100, ret_str, strlen(ret_str));
		} else if (U("1")==model_id){
			//Sleep(15 * 1000);
			//wmemcpy_s(wstr, 100, U("The prediction result is 45.32% risk of the disease."), 100);
			memcpy(ret_str, "The prediction result is 45.32% risk of the disease.", 100);
                        sleep(15);
		}
		else if (U("2") == model_id) {
			//Sleep(30 * 1000);
			//wmemcpy_s(wstr, 100, U("The prediction result is 63.84% risk of the disease."), 100);
			memcpy(ret_str, "The prediction result is 63.84% risk of the disease.", 100);
                        sleep(15);
		}
		else {
			//wmemcpy_s(wstr,100,U("Model unrecognized."), 100);
			memcpy(ret_str,"Model unrecognized.", 100);
		}
		//rply_obj[U("prediction")] = json::value(wstr);
		rply_obj[U("prediction")] = json::value(ret_str);
		//result = rply_obj.serialize();
	}
	else if (!cmd.compare(U("attestation"))) {
		utility::string_t mrenclave, pubkey, result;
		utility::string_t server_url = obj.at(U("server_url")).as_string();
		utility::string_t runner_address = obj.at(U("runner_address")).as_string();
		utility::string_t spid = obj.at(U("spid")).as_string();

		if (enclave.simple_ra(server_url, runner_address, spid, 
			  mrenclave, pubkey, result) != SGX_SUCCESS) {
			printf("Attestation fail. \n");
			//result = U("Attestation fail");
			rply_obj[U("Response")] = json::value(U("Attestation fail"));
			ret = -1;
		}
		else {
			printf("Attestation succeeded. \n");
			rply_obj = json::value::parse(result);
			rply_obj[U("MRENCLAVE")] = json::value(mrenclave);
			rply_obj[U("enclave_public_key")] = json::value(pubkey);
		}
	}
	else {
		ret = -1;
		rply_obj[U("Response")] = json::value(U("Unknow command"));
		//result = U("Unknow command");
	}

	http_response response(status_codes::OK);
	if (ret) {
		response.set_status_code(status_codes::InternalError);
	}
	response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
	//response.headers().add(U("Access-Control-Allow-Methods"), U("OPTIONS, GET, PUT, POST, DELETE"));
	response.set_body(rply_obj);
	message.reply(response);
};

//
// .
// 
void NetworkListener::handle_delete(http_request message)
{
	ucout << message.to_string() << endl;

	http_response response(status_codes::OK);
	response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
	response.set_body(U("OK"));
	message.reply(response);
};


//
// .
// 
void NetworkListener::handle_put(http_request message)
{
	ucout << message.to_string() << endl;


	http_response response(status_codes::OK);
	response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
	response.set_body(U("OK"));
	message.reply(response);
};
