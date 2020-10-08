//file: key_management.cpp Author: Shifa Version:0.1
#include "sgx_tcrypto.h"
#include "string.h"
#include "sgx_tprotected_fs.h"
#include "sgx_tseal.h"
#include "key_management.h"

sgx_ec256_private_t enclave_private; //The private key of the enclave generated when it is firstly lauched
sgx_ec256_public_t enclave_public; //public key
char keyfilename[] = "key_management.seal";
char addrfilename[] = "key_management.addr";

sgx_status_t km_init() {
	sgx_ecc_state_handle_t ecc_handle;
	sgx_status_t ret = sgx_ecc256_open_context(&ecc_handle);
	if (ret != SGX_SUCCESS)
		return ret;
	ret = sgx_ecc256_create_key_pair(&enclave_private, &enclave_public, ecc_handle);
	if (ret != SGX_SUCCESS) {
		sgx_ecc256_close_context(ecc_handle);
		return ret;
	}
	ret = sgx_ecc256_close_context(ecc_handle);
	if (ret != SGX_SUCCESS)
		return ret;

	//Seal the keys before storing them to a file.
	//1. Use sgx_calc_sealed_data_size to calculate the number of bytes to allocate for the sgx_sealed_data_t structure.
	//2. Allocate memory for the sgx_sealed_data_t structure.
	//3. Call sgx_seal_data to perform sealing operation
#define BUFFER_SIZE (1024*10)
	uint8_t data_buffer[BUFFER_SIZE];
	uint32_t sealed_data_size;
	SGX_FILE* file = sgx_fopen_auto_key(keyfilename, "wb");
	if (NULL == file)
		return (sgx_status_t)(-1);

	sealed_data_size = sgx_calc_sealed_data_size(0, sizeof(sgx_ec256_private_t));
	if (sealed_data_size > BUFFER_SIZE) {
		sgx_fclose(file);
		return (sgx_status_t)(-1);
	}

	ret = sgx_seal_data(0, NULL, sizeof(sgx_ec256_private_t),
		(const uint8_t *)&enclave_private, sealed_data_size, (sgx_sealed_data_t *)data_buffer);
	if (SGX_SUCCESS != ret) {
		sgx_fclose(file);
		return (sgx_status_t)(-1);
	}

	size_t  count = sgx_fwrite(data_buffer, sealed_data_size, 1, file);
	if (1 != count) {
		sgx_fclose(file);
		return (sgx_status_t)(-1);
	}
	
	//Seal and write public key
	sealed_data_size = sgx_calc_sealed_data_size(0, sizeof(sgx_ec256_public_t));
	if (sealed_data_size > BUFFER_SIZE) {
		sgx_fclose(file);
		return (sgx_status_t)(-1);
	}

	ret = sgx_seal_data(0, NULL, sizeof(sgx_ec256_public_t),
		(const uint8_t *)&enclave_public, sealed_data_size, (sgx_sealed_data_t *)data_buffer);

	count = sgx_fwrite(data_buffer, sealed_data_size, 1, file);
	if (1 != count) {
		sgx_fclose(file);
		return (sgx_status_t)(-1);
	}
	sgx_fclose(file);
	return SGX_SUCCESS;
}

sgx_status_t km_reload() {

	//Unseal the keys read from the file
	//1. Use sgx_get_encrypt_txt_len and sgx_get_add_mac_txt_len to determine the size of the buffers to allocate in terms of bytes.
	//2. Allocate memory for the decrypted text and additional text buffers.
	//3. Call sgx_unseal_data to perform the unsealing operation.
#define BUFFER_SIZE (1024*10)
	uint8_t data_buffer[BUFFER_SIZE];
	uint32_t sealed_data_size;
	SGX_FILE* file = sgx_fopen_auto_key(keyfilename, "rb");
	if (NULL == file)
		return (sgx_status_t)(-1);

	//read private key from file.
	sealed_data_size = sgx_calc_sealed_data_size(0, sizeof(sgx_ec256_private_t));
	if (sealed_data_size > BUFFER_SIZE) {
		sgx_fclose(file);
		return (sgx_status_t)(-1);
	}
	size_t count = sgx_fread(data_buffer, sealed_data_size, 1, file);
	if (1 != count) {
		sgx_fclose(file);
		return (sgx_status_t)(-1);
	}

	uint32_t data_size = sgx_get_encrypt_txt_len((sgx_sealed_data_t *)data_buffer);
	if (sizeof(sgx_ec256_private_t) != data_size) {
		sgx_fclose(file);
		return (sgx_status_t)(-1);
	}

	uint32_t additional_MACtext_length = 0, decrypted_text_length=data_size;
	sgx_status_t ret = sgx_unseal_data((sgx_sealed_data_t *)data_buffer, NULL,
		&additional_MACtext_length, (uint8_t *)&enclave_private, &decrypted_text_length);
	if (SGX_SUCCESS != ret || decrypted_text_length != data_size) {
		sgx_fclose(file);
		return (sgx_status_t)(-1);
	}

	//read public key from the file
	sealed_data_size = sgx_calc_sealed_data_size(0, sizeof(sgx_ec256_public_t));
	if (sealed_data_size > BUFFER_SIZE) {
		sgx_fclose(file);
		return (sgx_status_t)(-1);
	}
	count = sgx_fread(data_buffer, sealed_data_size, 1, file);
	if (1 != count) {
		sgx_fclose(file);
		return (sgx_status_t)(-1);
	}

	data_size = sgx_get_encrypt_txt_len((sgx_sealed_data_t *)data_buffer);
	if (sizeof(sgx_ec256_public_t) != data_size) {
		sgx_fclose(file);
		return (sgx_status_t)(-1);
	}

	additional_MACtext_length = 0;
	decrypted_text_length = data_size;
	ret = sgx_unseal_data((sgx_sealed_data_t *)data_buffer, NULL,
		&additional_MACtext_length, (uint8_t *)&enclave_public, &decrypted_text_length);
	if (SGX_SUCCESS != ret || decrypted_text_length != data_size) {
		sgx_fclose(file);
		return (sgx_status_t)(-1);
	}

	sgx_fclose(file);
	return SGX_SUCCESS;
}

#define MAX_ADDR_SIZE 64
char runner_address[MAX_ADDR_SIZE] = "";

sgx_status_t store_runner_addr(char *runner_addr) {
#define BUFFER_SIZE (1024*10)
	uint8_t data_buffer[BUFFER_SIZE];
	uint32_t sealed_data_size;
	sgx_status_t ret;

	if (0 != strlen(runner_address)) {
		return SGX_ERROR_UNEXPECTED; //runner address exist, failed to store new address.
	}
	SGX_FILE* file = sgx_fopen_auto_key(addrfilename, "rb");
	if (NULL != file) {
		size_t count = sgx_fread(data_buffer, 1, BUFFER_SIZE, file);
		if (0 != count) {
			uint32_t data_size = sgx_get_encrypt_txt_len((sgx_sealed_data_t *)data_buffer);
			if (data_size < MAX_ADDR_SIZE) {
				uint32_t additional_MACtext_length = 0, decrypted_text_length = data_size;
				ret = sgx_unseal_data((sgx_sealed_data_t *)data_buffer, NULL,
					&additional_MACtext_length, (uint8_t *)runner_address, &decrypted_text_length);
				if (SGX_SUCCESS == ret && decrypted_text_length == data_size) {
					sgx_fclose(file);
					return SGX_ERROR_UNEXPECTED; //runner address exist in file, failed to store new address
				} 
			}
		}
		sgx_fclose(file);
	}
	strncpy(runner_address, runner_addr, MAX_ADDR_SIZE);

	file = sgx_fopen_auto_key(addrfilename, "wb");
	if (NULL == file)
		return (sgx_status_t)(-1);

	sealed_data_size = sgx_calc_sealed_data_size(0, (uint32_t)strnlen(runner_address,MAX_ADDR_SIZE));
	if (sealed_data_size > BUFFER_SIZE) {
		sgx_fclose(file);
		return (sgx_status_t)(-1);
	}

	ret = sgx_seal_data(0, NULL, (uint32_t)strnlen(runner_address, MAX_ADDR_SIZE),
		(const uint8_t *)&runner_addr, sealed_data_size, (sgx_sealed_data_t *)data_buffer);
	if (SGX_SUCCESS != ret) {
		sgx_fclose(file);
		return (sgx_status_t)(-1);
	}

	size_t count = sgx_fwrite(data_buffer, sealed_data_size, 1, file);
	if (1 != count) {
		sgx_fclose(file);
		return (sgx_status_t)(-1);
	}
	sgx_fclose(file);
	return SGX_SUCCESS;
}