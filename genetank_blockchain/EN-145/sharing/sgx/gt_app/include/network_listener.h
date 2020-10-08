#pragma once

#include "stdafx.h"
#include <bits/unique_ptr.h>

using namespace std;
using namespace web;
using namespace utility;
using namespace http;
using namespace web::http::experimental::listener;

#define RUNNER_PATH "runner"
#define QUERIER_PATH "querier"
#define DONOR_PATH "donor"
#define TRAINER_PATH "trainer"

class NetworkListener
{
public:
	NetworkListener() { }
	NetworkListener(utility::string_t url);
	void setPath(const string_t& path_in);

	pplx::task<void> open() { return m_listener.open(); }
	pplx::task<void> close() { return m_listener.close(); }

private:

	void handle_get(http_request message);
	void handle_put(http_request message);
	void handle_post(http_request message);
	void handle_delete(http_request message);
	void handle_options(http_request message);

	string_t path;
	http_listener m_listener;
};

void listener_initialize(const string_t& address);
void listener_shutdown();


