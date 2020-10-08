#include "stdafx.h"
#include "enclave_control.h"
#include "network_listener.h"
#include "conversion.h"
#include "stdlib.h"

#include "msg_def.h"
#include "ecalls.h"
#include "conversion.h"
#include "enclave_tab.h"

using namespace std;
using namespace web;
using namespace utility;
using namespace http;
using namespace web::http::experimental::listener;

std::unique_ptr<NetworkListener> manage_httpListener;
std::unique_ptr<NetworkListener> service_httpListener;
std::unique_ptr<NetworkListener> data_httpListener;

void listener_initialize(const string_t& address)
{
	// Build our listener's URI from the configured address and the hard-coded path "..."

	uri_builder uri(address);
	uri.append_path(U(RUNNER_PATH));

	auto addr = uri.to_uri().to_string();
	manage_httpListener = std::unique_ptr<NetworkListener>(new NetworkListener(addr));
	manage_httpListener->setPath(U(RUNNER_PATH));
	manage_httpListener->open().wait();

	std:cout << utility::string_t(U("Listening for runner manage requests at: ")) << addr << std::endl;

	return;
}

void listener_shutdown()
{
	manage_httpListener->close().wait();
	return;
}

int access_code = 100;

NetworkListener::NetworkListener(utility::string_t url) : m_listener(url)
{
	m_listener.support(methods::GET, std::bind(&NetworkListener::handle_get, this, std::placeholders::_1));
	m_listener.support(methods::PUT, std::bind(&NetworkListener::handle_put, this, std::placeholders::_1));
	m_listener.support(methods::POST, std::bind(&NetworkListener::handle_post, this, std::placeholders::_1));
	m_listener.support(methods::DEL, std::bind(&NetworkListener::handle_delete, this, std::placeholders::_1));
	m_listener.support(methods::OPTIONS, std::bind(&NetworkListener::handle_options, this, std::placeholders::_1));
}

void NetworkListener::setPath(const string_t &path_in){
	path = path_in;
	return;
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

	ucout << U("JSON received: ") << obj.serialize() << endl;

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

	ucout << U("Endpoint path:") << path << endl;

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

sgx_status_t call_ec_session(sgx_status_t* retval, const char* reqMsg, char *rspMsg, size_t rspSize);

//
// .
//
void NetworkListener::handle_post(http_request message)
{
	//int ret = 0, role=ROLE_CREATOR;

	//ucout << message.to_string() << endl;

	json::value obj = message.extract_json().get();

	//ucout << U("JSON received: ") << obj.serialize() << endl;

	http::http_headers header = message.headers();
	utility::string_t sessionId_str = header[U("session_id")];
	uint32_t session_id;
	sscanf(sessionId_str.c_str(), "%d", &session_id);
	//pplx::task<utility::string_t> req = message.extract_string();

	utility::string_t rcv_iv = header[U("aes_iv")];

	size_t maxSize=10240;
	vector <unsigned char> rspMsg(10240);
	sgx_status_t sgxRet, retVal=SGX_SUCCESS;
	uint8_t iv[16], iv_hex[33];
	iv_hex[0]=iv_hex[32]=0;
	if (0 == session_id){
		utility::string_t reqMsg = obj.serialize();
		utility::string_t pubkey = obj.at(U("ethPubKey")).as_string();

		sgxRet = enclaveTab->newSession(pubkey.c_str(), &session_id, reqMsg.c_str(),
				(void *)&rspMsg, iv);
		bin2hex((const unsigned char *)iv, 16, (char *)iv_hex);
	} else {
		uint8_t r_iv[16];
		hex2bin(rcv_iv.c_str(), r_iv, 16);
		vector <unsigned char> msg =  utility::conversions::from_base64(obj.at(U("content")).as_string());

		size_t msgLen = msg.size();
		sgxRet = enclaveTab->session(&retVal, session_id, (const uint8_t *)&msg[0], msgLen,
				&rspMsg, r_iv);
		if ((sgx_status_t)-2 == retVal){
			session_id = 0;
			retVal = SGX_SUCCESS;
		}
		bin2hex((const unsigned char *)r_iv, 16, (char *)iv_hex);
	}

	http_response response(status_codes::OK);
	if (SGX_SUCCESS != sgxRet || SGX_SUCCESS != retVal) {
		//response.set_status_code(status_codes::InternalError);
		char rsp[]="Error while calling the Enclave function";
		rspMsg.assign((size_t)strnlen(rsp, 100), (const uint8_t &)rsp);
	}

	response.headers().add(U("aes_iv"), iv_hex);
	response.set_body(utility::conversions::to_base64(rspMsg));

	response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
	//response.headers().add(U("Access-Control-Allow-Methods"), U("OPTIONS, GET, PUT, POST, DELETE"));
	response.headers().add(U("session_id"), session_id);

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
	http_response response(status_codes::OK);
	response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
	response.set_body(U("OK"));
	message.reply(response);
};

