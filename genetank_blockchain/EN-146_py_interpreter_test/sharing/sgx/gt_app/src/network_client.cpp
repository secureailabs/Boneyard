/*
 * network_client.cpp
 *
 *  Created on: Jul 9, 2018
 *      Author: accuser
 */

#include "network_client.h"
#include "conversion.h"
#include "msg_def.h"
#include "stdafx.h"

using namespace std;
using namespace web;
using namespace utility;
using namespace http;

http::method getMethod(string_t mtdStr){
	if (!mtdStr.compare("GET"))
		return methods::GET;
	if (!mtdStr.compare("POST"))
		return methods::POST;
	if (!mtdStr.compare("PUT"))
		return methods::PUT;
	if (!mtdStr.compare("OPTIONS"))
		return methods::OPTIONS;
	if (!mtdStr.compare("DELETE"))
		return methods::DEL;
	if (!mtdStr.compare("PATCH"))
		return methods::PATCH;
	if (!mtdStr.compare("HEAD"))
		return methods::HEAD;
	return methods::GET;
}
/*
 * req_info JSON:
 *   url: The complete URL includes "http://" or "https://", domain name, port number, resources, queries, etc.
 *   headers: JSON object with attributes of headers
 *   method: method number of 0~3: 0 - "GET", 1 - "POST", 2 - "PUT", 3 - "DELETE"
 *   body: the body data ready to be sent
 *
 * return JSON:
 *   status: response status
 *   headers: JSON object with all response headers
 *   content: the response content
 *   content_length: the length of the response content
 */
sgx_status_t rest_request(json::value& req_info, json::value &resp_info){
	sgx_status_t ret = SGX_SUCCESS;
	if (!req_info.has_field(U("url")))
		return (sgx_status_t)-1;
	http::uri uri = http::uri(req_info[U("url")].as_string());
	web::http::client::http_client client(uri);
	http::method mtd = getMethod(req_info[U("method")].as_string());
	http_request req(mtd);
	if (req_info.has_field(U("headers"))){
		json::array headers = req_info[U("headers")].as_array();
		int len = headers.size();
		for (int i=0; i<len; i++){
			json::value header = headers[i];
			if (header.is_object() && header.has_field(U("key")) && header.has_field(U("value")))
				req.headers().add(header[U("key")].as_string(),header[U("value")].as_string());
		}
	}

	utility::string_t body=U("");
	if (req_info.has_field(U("data"))){
		body = req_info[U("data")].as_string();
	}

	req.set_body(body);

	http_response response;
    try
    {
		response = client.request(req).get();
    }
    catch (const std::exception &e)
    {
        std::cout<<"Error exception: "<<e.what()<<std::endl;
		ret = SGX_ERROR_UNEXPECTED;
    }

    if (SGX_SUCCESS != ret)
    	return ret;

    http::status_code status_c = response.status_code();

    resp_info[U("status")] = json::value::number(status_c);
	http::http_headers rsp_headers = response.headers();
	http::http_headers::iterator itt = rsp_headers.begin();
	json::value out_header;
	int header_cnt=0;
	while (itt!=rsp_headers.end()){
		out_header[header_cnt][U("key")] = json::value::string(itt->first);
		out_header[header_cnt][U("value")] = json::value::string(itt->second);
		itt++;
		header_cnt++;
	}
	resp_info[U("headers")] = out_header;
	utility::string_t contentLen = rsp_headers[U("Content-Length")];
	uint32_t cLen;
	sscanf(contentLen.c_str(), "%d", &cLen);
	resp_info[U("content_length")] = json::value::number(cLen);
	if (cLen > 0){
		resp_info[U("content")] = json::value::string(string_t(response.extract_string(true).get()));

	} else {
		resp_info[U("content")] = json::value::string(U(""));
	}
	return ret;
}

sgx_status_t http_send_2enclave(utility::string_t server_url, json::value& obj) {
	utility::string_t address = server_url;
	utility::string_t path = U("runner");
	//utility::string_t content_type = U("application/json");
	sgx_status_t ret = SGX_SUCCESS;
	http::uri uri = http::uri(address);
	json::value tempObj;
	web::http::client::http_client gtDappr(uri);

	http_request req(methods::POST);
	utility::string_t session_id = "session_id";
	if (obj.has_field(session_id))
		req.headers().add(session_id,obj[U(session_id)]);

	utility::string_t aes_iv = "aes_iv";
	if (obj.has_field(aes_iv)){
		req.headers().add(U("aes_iv"), obj[U(aes_iv)].as_string());
		http::http_headers headers = req.headers();
	}

	req.set_body(obj);
	req.set_request_uri(path);
    // Wait for all the outstanding I/O to complete and handle any exceptions
	http_response response;
    try
    {
		response = gtDappr.request(req).get();
    }
    catch (const std::exception &e)
    {
        std::cout<<"Error exception: "<<e.what()<<" ("<<server_url.c_str()<<")"<<std::endl;
		ret = SGX_ERROR_UNEXPECTED;
    }

    if (SGX_SUCCESS != ret)
    	return ret;

    http::status_code status_c = response.status_code();

	if (status_c != 200)
	{
		fprintf(stderr, "\nError, HTTP send message to %s failed [%s].\n", server_url.c_str(),
			__FUNCTION__);
		ret = SGX_ERROR_UNEXPECTED;
	}
	else {
		http::http_headers headers = response.headers();

		utility::string_t iv_str = headers[U("aes_iv")];
		utility::string_t sessionIdStr = headers[U("session_id")];
		uint32_t sessionId;
		sscanf(sessionIdStr.c_str(), "%d", &sessionId);
		utility::string_t contentLen = headers[U("Content-Length")];
		obj[U("aes_iv")] = json::value::string(iv_str);
		obj[U("session_id")] = json::value::number(sessionId);
		uint32_t cLen;
		sscanf(contentLen.c_str(), "%d", &cLen);
		obj[U("contentLen")] = json::value::number(cLen);
		obj[U("content")] = json::value::string(string_t(response.extract_string().get()));
	}
	return ret;
}



sgx_status_t http_send(utility::string_t server_url, json::value& obj){
	utility::string_t address = server_url;
	utility::string_t path = U(DAPP_URL_PATH);
	//utility::string_t content_type = U("application/json");
	sgx_status_t ret=SGX_SUCCESS;

	http::uri uri = http::uri(address);

	web::http::client::http_client gtDappr(uri);

	http_response response;
    try
    {
    	response = gtDappr.request(methods::POST,path,obj).get();
    }
    catch (const std::exception &e)
    {
        std::cout<<"Error exception: "<< e.what()<<std::endl;
    }

	ucout << response.to_string() << endl;

	http::status_code status_c = response.status_code();

	if (status_c != 200)
	{
		fprintf(stderr, "\nError, Dapp register  fail [%s].\n",
			__FUNCTION__);
		ret = SGX_ERROR_UNEXPECTED;
	}
	else {
		obj = response.extract_json().get();
		//ucout << U("JSON received:") << obj.serialize() << endl;
	}

	return ret;
}

sgx_status_t http_send_data(utility::string_t server_url, json::value& obj){
	//utility::string_t address = server_url;
	utility::string_t path = U("");
	//utility::string_t content_type = U("application/json");
	sgx_status_t ret;

	http::uri uri = http::uri(server_url);

	web::http::client::http_client client(http::uri_builder(uri).to_uri());

	http_response response = client.request(methods::POST,path,obj).get();

	ucout << response.to_string() << endl;

	http::status_code status_c = response.status_code();

	if (status_c != 200)
	{
		return((sgx_status_t)-1);
	}
	return SGX_SUCCESS;
}

void rest_rest(){
	json::value req_info, resp_info;
	req_info[U("url")] = json::value::string(U("https://stu3.test.pyrohealth.net/fhir/Patient/7d13c1e8-b603-4ec6-98cc-b4af32fdd5d4"));
	req_info[U("method")] = json::value::number(HTTP_GET);
	json::value headers;
	json::value header;
	header[U("key")] = json::value::string(U("Accept"));
	header[U("value")] = json::value::string(U("application/fhir+json"));
	headers[0] = header;
	req_info[U("headers")] = headers;
	sgx_status_t ret = rest_request(req_info, resp_info);
	if (SGX_SUCCESS == ret){
		std::cout << "Get result: "<<resp_info[U("content")] << std::endl;
	}

	req_info[U("method")] = json::value::number(HTTP_PUT);
	utility::string_t content = resp_info[U("content")].as_string();
	json::value data = json::value::parse(content);
	data[U("gender")] = json::value::string(U("female"));
	utility::string_t put_body = data.serialize();
	req_info[U("body")] = json::value::string(put_body);
	header[U("key")] = json::value::string(U("Content-Type"));
	header[U("value")] = json::value::string(U("application/fhir+json"));
	headers[1] = header;
	req_info[U("headers")] = headers;
	ret = rest_request(req_info, resp_info);
	if (SGX_SUCCESS == ret){
		std::cout << "Put result: "<<resp_info[U("content")] << std::endl;
	}

	req_info[U("url")] = json::value::string(U("https://stu3.test.pyrohealth.net/fhir/Patient/"));
	req_info[U("method")] = json::value::number(HTTP_POST);
	content = resp_info[U("content")].as_string();
	data = json::value::parse(content);
	data[U("gender")] = json::value::string(U("male"));
	json::value names = data[U("name")];
	names[0][U("given")] = json::value::string(U("Thomas"));
	data[U("name")] = names;
	data.erase(U("id"));
	put_body = data.serialize();
	req_info[U("body")] = json::value::string(put_body);
	header[U("key")] = json::value::string(U("Content-Type"));
	header[U("value")] = json::value::string(U("application/fhir+json"));
	headers[1] = header;
	req_info[U("headers")] = headers;
	ret = rest_request(req_info, resp_info);
	if (SGX_SUCCESS == ret){
		std::cout << "Post result: "<<resp_info[U("content")] << std::endl;
	}

	content = resp_info[U("content")].as_string();
	data = json::value::parse(content);
	utility::string_t url_id = U("https://stu3.test.pyrohealth.net/fhir/Patient/")+data[U("id")].as_string();
	req_info[U("url")] = json::value::string(url_id);
	req_info[U("method")] = json::value::number(HTTP_DELETE);
	header[U("key")] = json::value::string(U("Accept"));
	header[U("value")] = json::value::string(U("application/fhir+json"));
	json::value hdrs;
	hdrs[0] = header;
	req_info[U("headers")] = hdrs;
	req_info[U("body")] = json::value::string(U(""));
	ret = rest_request(req_info, resp_info);
	if (SGX_SUCCESS == ret){
		std::cout << "Post result: "<<resp_info[U("content")] << std::endl;
	}

	req_info[U("url")] = json::value::string(url_id+U("/_history/1"));
	req_info[U("method")] = json::value::number(HTTP_GET);
	req_info[U("body")] = json::value::string(U(""));
	ret = rest_request(req_info, resp_info);
	if (SGX_SUCCESS == ret){
		std::cout << "Post result: "<<resp_info[U("content")] << std::endl;
	}


	req_info[U("url")] = json::value::string(U("https://stu3.test.pyrohealth.net/fhir/Patient?family=fhirman"));
	req_info[U("method")] = json::value::number(HTTP_GET);
	req_info[U("body")] = json::value::string(U(""));
	ret = rest_request(req_info, resp_info);
	if (SGX_SUCCESS == ret){
		std::cout << "Post result: "<<resp_info[U("content")] << std::endl;
	}

}
