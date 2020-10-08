#include "..\include\stdafx.h"

using namespace std;
using namespace web;
using namespace utility;
using namespace http;
using namespace web::http::experimental::listener;

class NetworkListener
{
public:
	NetworkListener() { }
	NetworkListener(utility::string_t url);

	pplx::task<void> open() { return m_listener.open(); }
	pplx::task<void> close() { return m_listener.close(); }

private:

	void handle_get(http_request message);
	void handle_put(http_request message);
	void handle_post(http_request message);
	void handle_delete(http_request message);

	http_listener m_listener;
};

NetworkListener::NetworkListener(utility::string_t url) : m_listener(url)
{
	m_listener.support(methods::GET, std::bind(&NetworkListener::handle_get, this, std::placeholders::_1));
	m_listener.support(methods::PUT, std::bind(&NetworkListener::handle_put, this, std::placeholders::_1));
	m_listener.support(methods::POST, std::bind(&NetworkListener::handle_post, this, std::placeholders::_1));
	m_listener.support(methods::DEL, std::bind(&NetworkListener::handle_delete, this, std::placeholders::_1));
}

//
// .
// 
void NetworkListener::handle_get(http_request message)
{
	ucout << message.to_string() << endl;

	auto paths = http::uri::split_path(http::uri::decode(message.relative_uri().path()));
	if (paths.empty())
	{
		message.reply(status_codes::OK/*,some json*/);
		return;
	}
	message.reply(status_codes::OK, U("OK"));
};

//
// .
// 
void NetworkListener::handle_post(http_request message)
{
	//int ret = 0;

	ucout << message.to_string() << endl;

	json::value obj = message.extract_json().get();

	ucout << U("JSON received:") << obj.serialize() << endl;

	uint32_t type = obj.at(U("msgType")).as_integer();

	switch (type) {
	case 0: {
//		register_runner(&obj);
		break;
		}

	}
	message.reply(status_codes::OK);
};

//
// .
// 
void NetworkListener::handle_delete(http_request message)
{
	ucout << message.to_string() << endl;

	message.reply(status_codes::Forbidden, U("Player name is required in query"));
};


//
// .
// 
void NetworkListener::handle_put(http_request message)
{
	ucout << message.to_string() << endl;

	message.reply(status_codes::Forbidden, U("Unrecognized request"));
};
