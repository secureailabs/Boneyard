#include "network_dealer.h"
#include "isa_service.h"

std::unique_ptr<NetworkDealer> g_httpDealer;

void on_initialize(const string_t& address)
{
	// Build our listener's URI from the configured address and the hard-coded path "attestation"

	uri_builder uri(address);
	uri.append_path(U("attestation"));

	auto addr = uri.to_uri().to_string();
	g_httpDealer = std::unique_ptr<NetworkDealer>(new NetworkDealer(addr));
	g_httpDealer->open().wait();

	ucout << utility::string_t(U("Listening for requests at: ")) << addr << std::endl;

	return;
}

void on_shutdown()
{
	g_httpDealer->close().wait();
	return;
}

//
// To start the server, run the below command with admin privileges:
// service_provider.exe <port>
// If port is not specified, will listen on 4200
//
#ifdef _MSC_VER
#include <TCHAR.H>
int _tmain(int argc, _TCHAR *argv[])
#else
#define _T(x) x
int main(int argc, char* argv[])
#endif
{
#ifdef WINDOWS
	if (!init_certificate()) {
		std::cout << "Init IAS service provider certificate fail." << std::endl;
		return -1;
	}
#endif
	//network_send_receive(U("http://localhost:4000/enclave"));
	//utility::string_t sig_rl_s;
	//uint32_t sig_rl_size = 0;
	//epid_group_id_t      gid = { 0,0,0x0a,0xe3};
	//get_sigrl(gid, &sig_rl_size, sig_rl_s);

	utility::string_t port = U("4200");
	utility::string_t address = U("http://localhost:");

	if (argc == 2)
	{
		//port = argv[1];
		address = argv[1];
	}
	else {
		address.append(port);
	}

	on_initialize(address);

	std::cout << "Press ENTER to exit." << std::endl;

	std::string line;
	std::getline(std::cin, line);

	on_shutdown();
	return 0;
}
