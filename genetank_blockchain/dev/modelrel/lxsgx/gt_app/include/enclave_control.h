#ifndef ENCLAVE_CONTROL_H
#define ENCLAVE_CONTROL_H

#include "../include/stdafx.h"
#include <stdio.h>
#include <limits.h>

#include "../gt_enclave_u.h"

// Needed to create enclave and do ecall.
#include "sgx_urts.h"

// Needed to query extended epid group id.
#include "sgx_uae_service.h"

#ifdef _MSC_VER
#include <TCHAR.H>
//#define ENCLAVE_PATH "gt_enclave.signed.dll"
#else
#define _T(x) x
//#define ENCLAVE_PATH "gt_enclave.signed.so"
#endif

class EnclaveControl {
public:
	EnclaveControl(){
		enclave_id = 0;
		p_public_key = NULL;
		b_pse = false;
	}

	~EnclaveControl() {
		/* Destroy the enclave */
		sgx_destroy_enclave(enclave_id);
	}

	sgx_enclave_id_t getEnclaveId() { return enclave_id; };
	sgx_ec256_public_t *getPublicKey() { return p_public_key; };
	sgx_status_t remote_attestation(utility::string_t server_url, utility::string_t runner_addr);
	sgx_status_t simple_ra(const utility::string_t & server_url, 
		const utility::string_t & runner_addr,
		const utility::string_t & spid,
		utility::string_t & mrenclave,
		utility::string_t & pubkey, 
		utility::string_t & attestation_report);//simple remote attestation
	int getbPse() { return b_pse; }
	int create_enclave();

private:
	sgx_enclave_id_t enclave_id;
	sgx_ec256_public_t *p_public_key;
	int b_pse;
};

extern EnclaveControl enclave;

int query_sgx_status();

#endif

