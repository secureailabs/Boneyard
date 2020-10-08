/*
 * network_client.h
 *
 *  Created on: Jul 9, 2018
 *      Author: accuser
 */

#ifndef GT_APP_INCLUDE_NETWORK_CLIENT_H_
#define GT_APP_INCLUDE_NETWORK_CLIENT_H_

// Needed to query extended epid group id.
#include "sgx_uae_epid.h"
//#include "../gt_enclave_u.h"
#include "stdafx.h"
#include "tools.h"

using namespace std;
using namespace web;
using namespace utility;
using namespace http;

#define DAPP_URL_PATH "dapp_endpoint"

enum {
	HTTP_GET,
	HTTP_POST,
	HTTP_PUT,
	HTTP_DELETE,
} _http_method;

sgx_status_t rest_request(json::value& req_info, json::value &resp_info);
sgx_status_t http_send(utility::string_t server_url, json::value& obj);
sgx_status_t http_send_data(utility::string_t server_url, json::value& obj);
sgx_status_t http_send_2enclave(utility::string_t server_url, json::value& obj);

#endif /* GT_APP_INCLUDE_NETWORK_CLIENT_H_ */
