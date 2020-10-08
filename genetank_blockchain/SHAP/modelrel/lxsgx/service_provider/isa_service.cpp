#include "isa_service.h"
#include "../gt_app/include/tools.h"

using namespace std;
using namespace web;
using namespace utility;
using namespace http;
using namespace web::http::experimental::listener;

//static const attestationReportSigningCA = {};

static spid_t g_spid = { 0x7B,0xC0,0x33,0x8D,0x24,0x78,0x1F,0x74,
0x48,0x75,0x17,0x8F,0xE7,0x38,0x1F,0x76 };

#ifdef WINDOWS
HRESULT loadPFX(HCERTSTORE *phCertStore) {
	HRESULT  hr = S_OK;

	LPCWSTR certFile(U("ias_ca/genetankaiopen.pfx"));
	// read cert file
	BY_HANDLE_FILE_INFORMATION fileInfo;
	HANDLE hFile =
		CreateFile(certFile, GENERIC_READ, FILE_SHARE_READ, NULL, 
			OPEN_EXISTING, 0, NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		printf("Error: Couldn't open file (%lls)\n", certFile);
		return S_FALSE;
	}

	GetFileInformationByHandle(hFile, &fileInfo);
	long fileSize = fileInfo.nFileSizeLow;

	// make buffer for cert data
	PBYTE pbBuffer = (PBYTE)malloc(fileSize);
	if (!pbBuffer)
	{
		printf("Error: malloc failed (%d)\n", fileSize);
		return S_FALSE;;
	}

	unsigned long bytesRead;
	ReadFile(hFile, pbBuffer, fileSize, &bytesRead, NULL);

	// create pfx blob
	CRYPT_DATA_BLOB cryptBlob;
	cryptBlob.cbData = fileSize;
	cryptBlob.pbData = pbBuffer;

	// is it actually a pfx blob?
	if (FALSE == PFXIsPFXBlob(&cryptBlob))
	{
		printf("Error: PFXIsPFXBlob failed\n");
		free(pbBuffer);
		return S_FALSE;;
	}

	WCHAR pw[256]=U("");//no password

	*phCertStore = PFXImportCertStore(&cryptBlob, (LPCWSTR)pw, CRYPT_USER_KEYSET);
	if (*phCertStore == NULL)
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
		printf("Error: PFXImportCertStore failed\n");
		free(pbBuffer);
		return hr;
	}
	free(pbBuffer);
	return hr;
}
static HCERTSTORE hCertStore=NULL;
#endif
utility::string_t ias_address = U("https://test-as.sgx.trustedservices.intel.com:443/attestation/sgx/v2/");

#ifdef WINDOWS
int init_certificate() {
	HRESULT  hr = loadPFX(&hCertStore);
	return(S_OK == hr);
}

HRESULT loadCert(PCCERT_CONTEXT *cert) {
#define CERTIFICATE_BUFFER_SIZE 10240

	HRESULT  hr = S_OK;
	BYTE     certEncoded[CERTIFICATE_BUFFER_SIZE] = { 0 };
	DWORD    certEncodedSize = 0L;
	HANDLE   certFileHandle = NULL;
	BOOL     result = FALSE;
	LPCWSTR certFile(U("c:\\zsf\\genetank_blockchain\\modelrel\\sgx\\x64\\debug\\genetankai.der"));

	// open the certificate file
	certFileHandle = CreateFile(certFile,
		GENERIC_READ,
		0,
		NULL,
		OPEN_EXISTING,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (INVALID_HANDLE_VALUE == certFileHandle) {
		hr = HRESULT_FROM_WIN32(GetLastError());
	}

	if (SUCCEEDED(hr)) {
		// if the buffer is large enough
		//  read the certificate file into the buffer
		if (GetFileSize(certFileHandle, NULL) <= CERTIFICATE_BUFFER_SIZE) {
			result = ReadFile(certFileHandle,
				certEncoded,
				CERTIFICATE_BUFFER_SIZE,
				&certEncodedSize,
				NULL);
			if (!result) {
				// the read failed, return the error as an HRESULT
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			else {
				hr = S_OK;
			}
		}
		else {
			// The certificate file is larger than the allocated buffer.
			//  To handle this error, you could dynamically allocate
			//  the certificate buffer based on the file size returned or 
			//  use a larger static buffer.
			hr = HRESULT_FROM_WIN32(ERROR_MORE_DATA);
		}
	}
	if (SUCCEEDED(hr))
	{
		// create a certificate from the contents of the buffer
		*cert = CertCreateCertificateContext(X509_ASN_ENCODING,
			certEncoded,
			certEncodedSize);
		if (!(*cert)) {
			hr = HRESULT_FROM_WIN32(GetLastError());
			CloseHandle(certFileHandle);
			hr = E_FAIL;
		}
		else {
			hr = S_OK;
		}
	}
	// close the certificate file
	if (NULL != certFileHandle) CloseHandle(certFileHandle);
	return hr;
}
#endif

int verify_attestation_evidence(
	const utility::string_t &quote_s,
	json::value & attestation_report)
{
#ifdef WINDOWS
	if (NULL == hCertStore)
		return -1;
#endif

	utility::ostringstream_t buf;
	http_response response;
	json::value obj;

	http::client::http_client_config config;
	//config.set_validate_certificates(false);

#ifdef WINDOWS
	auto func = [&](web::http::client::native_handle handle) {
		PCCERT_CONTEXT pCertContext = NULL;
		pCertContext =
			CertEnumCertificatesInStore(hCertStore, pCertContext);
		if (pCertContext != NULL) //(S_OK == loadCert(&hContext))
			WinHttpSetOption(handle, WINHTTP_OPTION_CLIENT_CERT_CONTEXT,
			(LPVOID)pCertContext, sizeof(CERT_CONTEXT));
		else {
			HRESULT  hr = HRESULT_FROM_WIN32(GetLastError());
			ucout << "CertCreateCertificateContext Failed." << hr << endl;
		}
	};
#endif
/*
        auto func = [&](boost::asio::ssl::context& ctx)
        {
                ctx.set_options(boost::asio::ssl::context::default_workarounds);
                ctx.use_certificate_chain(cert);
                ctx.use_private_key(key, boost::asio::ssl::context::pem);
        });
*/
	//auto func = [&](web::http::client::native_handle handle) {

	//config.set_nativehandle_options(func);
        //config.set_ssl_context_callback([]((boost::asio::ssl::context &ctx) {
        //  ctx.load_verify_file("PATH_TO_CERTIFICATE_FILE");
        //});

	utility::string_t address = ias_address;
	utility::string_t path = U("report");

	http::uri uri = http::uri(address);

	web::http::client::http_client isa_server(http::uri_builder(uri).to_uri(), config);

	obj[U("isvEnclaveQuote")] = json::value::string(quote_s);
	
	//isa_server.headers().add(U("Conten-Type"), U("application/json"));

	try
	{
		response = isa_server.request(methods::POST, path, obj).get();

		http::status_code st = response.status_code();

		http::http_headers headers = response.headers();
		web::http::http_headers::key_type key(U("Request-ID"));
		utility::string_t req_id = headers[key];

		if (status_codes::OK == st)
		{
			key = U("Content-Length");
			utility::string_t size_str = headers[key];
			key = U("X-IASReport-Signature");
			utility::string_t IAS_signature = headers[key];
			key = U("X-IASReport-Signing-Certificate");
			utility::string_t IAS_certificate = headers[key];

			attestation_report[U("Content-Length")] = json::value::string(size_str);
			attestation_report[U("X-IASReport-Signature")] = json::value::string(IAS_signature);
			attestation_report[U("X-IASReport-Signing-Certificate")] = json::value::string(IAS_certificate);
			attestation_report[U("attestation_report")] = response.extract_json().get();

			ucout << U("Report JSON generated:") << attestation_report.serialize() << endl;
			return 0;
		}
		else {
			ucout << "IAS response status:" << st << endl <<
				U("received: req_id:") << req_id << endl;
			return -1;
		}
	}
	catch (const std::exception &e)
	{
		ucout << "Error exception:" << e.what() << endl;
		return -1;
	}
	//Decrypt the Quote signature and verify.
}

// This function interface with IAS. It retrieves of the SigRL.
//
// @param gid Group ID for the EPID key.
// @param p_sig_rl_size Pointer to the output value of the full
//                      SIGRL size in bytes. (including the
//                      signature).
// @param p_sig_rl Pointer to the output of the SIGRL.
//
// @return int
//#include <cpprest/filestream.h>
//using namespace Concurrency;

int get_sigrl(
	const epid_group_id_t gid,
	uint32_t *p_sig_rl_size,
	utility::string_t & sig_rl_s)
{
#ifdef WINDOWS
	if (NULL == hCertStore)
		return -1;
#endif

	utility::ostringstream_t buf;
	http_response response;
	json::value obj;
	utility::string_t gid_s = mBin2Hex(gid, sizeof(epid_group_id_t));
	gid_s.erase(0, 2);

	http::client::http_client_config config;
	//config.set_validate_certificates(false);

#ifdef WINDOWS
	auto func = [&](web::http::client::native_handle handle) {
		PCCERT_CONTEXT pCertContext = NULL;
		pCertContext =
			CertEnumCertificatesInStore(hCertStore, pCertContext);
		if (pCertContext != NULL) //(S_OK == loadCert(&hContext))
			WinHttpSetOption(handle, WINHTTP_OPTION_CLIENT_CERT_CONTEXT,
			(LPVOID)pCertContext, sizeof(CERT_CONTEXT));
		else {
			HRESULT  hr = HRESULT_FROM_WIN32(GetLastError());
			ucout << "CertCreateCertificateContext Failed." << hr << endl;
		}
	};

	config.set_nativehandle_options(func);
#endif

	//utility::string_t address = U("https://test-as.sgx.trustedservices.intel.com:443/attestation/sgx/v2/sigrl/")+gid_s;
	utility::string_t address = ias_address + U("sigrl/") + gid_s;

	http::uri uri = http::uri(address);

	web::http::client::http_client isa_server(http::uri_builder(uri).to_uri(),config);

	try
	{
		response = isa_server.request(methods::GET, buf.str()).get();

		http::status_code st = response.status_code();

		http::http_headers headers = response.headers();
		web::http::http_headers::key_type key(U("Request-ID"));
		utility::string_t req_id = headers[key];

		if (status_codes::OK == st)
		{
			key = U("Content-Length");
			utility::string_t size_str = headers[key];
			*p_sig_rl_size = stoi(size_str);
			sig_rl_s = response.extract_string().get();
			ucout << U("received: req_id:") << req_id << endl <<
				U("content length:") << size_str << endl <<
				U("SigRl:\'") << sig_rl_s << "\'"<<endl;
			return 0;
		}
		else {
			ucout << "Response status:" << st << endl <<
				U("received: req_id:") << req_id << endl;
			return -1;
		}
	}
	catch (const std::exception &e)
	{
		ucout << "Error exception:" << e.what() << endl;
		return -1;
	}

	//return(ret);
}

int network_send_receive(utility::string_t server_url)
{
	int ret = 0;

	utility::ostringstream_t buf;

	std::string strEpidGrp;
	http_response response;
	http::status_code status_c;

	json::value obj;

	utility::string_t address = server_url;
	//utility::string_t address = U("http://localhost:4200/attestation"); //server_url

	http::uri uri = http::uri(address);

	web::http::client::http_client gtServiceProvider(http::uri_builder(uri).to_uri());

	obj[U("msgType")] = json::value::number(TYPE_RA_SIMPLE);

	buf.clear();

	response = gtServiceProvider.request(methods::POST, buf.str(), obj).get();

	ucout << response.to_string() << endl;

	status_c = response.status_code();

	if (status_c != 200)
	{
		fprintf(stderr, "\nError, simple_ra request fail [%s].",
			__FUNCTION__);
		ret = -1;
	}
	else {
		obj = response.extract_json().get();
		ucout << U("JSON received:") << obj.serialize() << endl;
		//int msg_size = obj.at(U("msgSize")).as_integer();
		//utility::string_t report = obj.at(U("attestationReport")).as_string();
	}

	return ret;
}

