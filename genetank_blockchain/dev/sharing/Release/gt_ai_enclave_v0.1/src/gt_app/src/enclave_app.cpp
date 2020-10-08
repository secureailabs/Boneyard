#include "..\include\stdafx.h"
#include "..\include\enclave_control.h"



using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

class NetworkListener
{
public:
	NetworkListener() {}
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

std::unique_ptr<NetworkListener> g_httpListener;

void on_initialize(const string_t& address)
{
	// Build our listener's URI from the configured address and the hard-coded path "attestation"

	uri_builder uri(address);
	uri.append_path(U("enclave"));

	auto addr = uri.to_uri().to_string();
	g_httpListener = std::unique_ptr<NetworkListener>(new NetworkListener(addr));
	g_httpListener->open().wait();

	ucout << utility::string_t(U("Listening for requests at: ")) << addr << std::endl;

	return;
}

void on_shutdown()
{
	g_httpListener->close().wait();
	return;
}


//
// To start the enclave, run the below command with admin privileges:
// gt_app.exe <port>
// If port is not specified, will listen on 4300
//
#ifdef _MSC_VER
#include <TCHAR.H>
int _tmain(int argc, _TCHAR *argv[])
#else
#define _T(x) x
int main(int argc, char* argv[])
#endif
{

#if defined(_MSC_VER)
	if (query_sgx_status() < 0) {
		/* either SGX is disabled, or a reboot is required to enable SGX */
		printf("Enter a character before exit ...\n");
		getchar();
		return -1;
	}
#endif 

	/* Initialize the enclave */
	if (enclave.create_enclave() < 0) {
		printf("Enter a character before exit ...\n");
		getchar();
		return -1;
	}

	if (enclave.remote_attestation("0xB480c3233e1852Ea00Fd0EEc7149b2a6E67c23B2") != SGX_SUCCESS) {
		printf("Attestation failed. \nEnter a character before exit ...\n");
		getchar();
		return -1;
	}

	utility::string_t port = U("4300");
	if (argc == 2)
	{
		port = argv[1];
	}

	utility::string_t address = U("http://localhost:");
	address.append(port);

	on_initialize(address);
	std::cout << "Press ENTER to exit." << std::endl;

	std::string line;
	std::getline(std::cin, line);

	on_shutdown();
	return 0;
}