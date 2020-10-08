
#include "reg_dapp.h"
#include "network_client.h"
#include "conversion.h"

#include "sgx_tcrypto.h"
#include "ecalls.h"

using namespace utility;
using namespace web;
using namespace http;

/*
 * Register to Dapp steps: 1)send public key to Dapp 2)Dapp check if the public key registered, 3) if yes send back the Ethereum address,
 * 4) If no enclave owner Ethereum address is stored, stored the Ethereum address, if there is a Ethereum address stored, verify the address,
 * if the address matches, finish successfully, if doesn't match, finish fail.
 * 5) if no in step 2 send back fail, the enclave display the public key to the console and remind operators to add the public key to the Dapp.
 */
sgx_status_t EnclaveControl::reg_dapp(char * dappUrl, char * enclaveUrl, char * username){
	char data[100] = "Genetank Enclave";
	char enclavePublic[64];
	char sPubKey[2*sizeof(sgx_ec256_public_t)+1];
	//sgx_ec256_signature_t signOut;
	uint8_t hash[32];
	sgx_status_t ret, retval;
	json::value obj;

	size_t len = (size_t)strnlen(data, 100);
	//Get signature and public key
	//ret = this->ec_enclave_sign(&retval, (uint8_t *)data, len, hash,
	//		(uint8_t *)&enclavePublic, (size_t)sizeof(sgx_ec256_public_t),
	//		(uint8_t *)&signOut, (size_t)sizeof(sgx_ec256_signature_t));

	ret = call_ec_get_pubkey(&retval, enclavePublic);

	if (SGX_SUCCESS != ret || SGX_SUCCESS != retval )
		return (sgx_status_t)(-1);

	bin2hex((uint8_t *)&enclavePublic, 64, (char *)sPubKey);
	sPubKey[2*sizeof(sgx_ec256_public_t)] ='\0';
	obj[U("msgType")] = json::value::number(TYPE_DAPP_REG);
	obj[U("pubKey")] = json::value::string(string_t(sPubKey));
	obj[U("enclaveUrl")] = json::value::string(string_t(enclaveUrl));
	obj[U("username")] = json::value::string(string_t(username));

	std::cout<<"Public key:"<<sPubKey<<std::endl;

	ret = http_send(string_t(dappUrl), (json::value &) obj);
	if (SGX_SUCCESS != ret){
		std::cout<<"Can't connect to the DAPP at "<<dappUrl<<std::endl;
		std::cout<<"If necessary please create an enclave record on the Dapp with a public key:"<<std::endl<<sPubKey<<std::endl;
		return (sgx_status_t)-1;
	}

	ucout << U("JSON received:") << obj.serialize() << endl;
	int result = obj.at(U("result")).as_integer();
	if (0 == result){
		string_t runnerEth = obj.at(U("runnerEth")).as_string();
		string_t trainerEth = obj.at(U("trainerEth")).as_string();
		eth_addr_t runnerEthAddr, trainerEthAddr;
		//strncpy((char *)(ethAddr.addr),ethAccount.c_str(),ETH_ADDR_LEN);
		hex2bin(runnerEth.c_str(), runnerEthAddr.addr, ETH_ADDR_LEN);
		hex2bin(trainerEth.c_str(), trainerEthAddr.addr, ETH_ADDR_LEN);
		ret = call_enclave_set_eth(&retval, &runnerEthAddr, &trainerEthAddr);
		if (SGX_SUCCESS != ret)
			return (sgx_status_t)(-1);
		else if (SGX_SUCCESS == retval) {
			std::cout<<"New Ethereum address has been added to the enclave."<<std::endl;
			return SGX_SUCCESS;
		}
		else if (1 == retval){
			std::cout<<"Ethereum address matches, register success."<<std::endl;
			return SGX_SUCCESS;
		} else {
			std::cout<<"Ethereum address doesn't match, register fail."<<std::endl;
			return (sgx_status_t)(-1);
		}
	}

	std::cout<<"Please create an enclave record on the Dapp with a public key:"<<std::endl<<sPubKey<<std::endl;

	return (sgx_status_t)(-1);
}
