#pragma once
#include "sgx_error.h"

sgx_status_t km_init();
sgx_status_t km_reload();
sgx_status_t store_runner_addr(const char *runner_addr, size_t message_size);
sgx_status_t get_public_key(sgx_ec256_public_t **pubkey);
