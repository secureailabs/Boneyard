#ifndef ENCLAVE_CONTROL_H
#define ENCLAVE_CONTROL_H

// Needed to create enclave and do ecall.
#include "sgx_urts.h"
#include "sgx.h"
// Needed to query extended epid group id.
#include "sgx_uae_epid.h"
#include <stdlib.h> /* for size_t */
#include <stdio.h>

#include "msg_def.h"

# include <unistd.h>
# include <pwd.h>
# define MAX_PATH FILENAME_MAX
#define MAX_ENCLAVE 16

//#define PASS_MAX 32

#define _T(x) x

class EnclaveControl {
public:
	EnclaveControl(){
		enclave_id = 0;
		enclaveHome = NULL;
		b_pse = false;
	}

	~EnclaveControl();

	sgx_enclave_id_t getEnclaveId() { return enclave_id; };
	sgx_status_t reg_dapp(char * dapp_url, char * enclave_url, char * username);
	int getbPse() { return b_pse; }
	sgx_status_t create_enclave(const char *home);
	sgx_status_t init_enclave(uint32_t stakeholderId, const uint8_t *message, size_t msgLen, uint8_t *iv,
			uint8_t *respBuff, size_t respBuffSize, size_t *respSize);
	sgx_status_t reset_enclave();
	sgx_status_t new_session(uint32_t *session_id, const char *reqMsg, void *resultVecPtr, uint8_t *iv);
	sgx_status_t session(sgx_status_t* retval, uint32_t session_id, const uint8_t *reqMsg, size_t reqLen,
			void *resultVecPtr, uint8_t *iv);
	int getStakeholder(const char *ethPubkey);
	sgx_status_t run_command(uint8_t *encBuff, size_t bufLen, void *resVecPtr, uint8_t *iv);
	sgx_status_t send_in_data(const uint8_t *inBuf, size_t inSize, uint8_t **destBuf);

private:
	sgx_enclave_id_t enclave_id;
	char *enclaveHome;
	//sgx_ec256_public_t *p_public_key;
	int b_pse;
};

#endif

