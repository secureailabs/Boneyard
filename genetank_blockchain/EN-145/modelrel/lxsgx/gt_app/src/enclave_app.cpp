#include "../include/stdafx.h"
#include "../include/enclave_control.h"
#include "../include/network_listener.h"

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
/*
	if (enclave.remote_attestation("0xB480c3233e1852Ea00Fd0EEc7149b2a6E67c23B2") != SGX_SUCCESS) {
		printf("Attestation failed. \nEnter a character before exit ...\n");
		getchar();
		return -1;
	}
*/
	utility::string_t port = U("4300");
	if (argc == 2)
	{
		port = argv[1];
	}
	else
	{
		printf("Usage: app.exe http://domain:port_number\n");
		port = U("http://0.0.0.0:4300");
	}

	utility::string_t address = port; // U("http://localhost:");
	//address.append(port);
	//printf("testing: u%04x", L'a');
	on_initialize(address);
	std::cout << "Press Control-C to exit." << std::endl;

	std::string line;
	//std::getline(std::cin, line);
        pause();

	on_shutdown();
	return 0;
}
