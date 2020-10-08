#pragma once
#include "sgx_error.h"

sgx_status_t km_init();
sgx_status_t km_reload();
sgx_status_t store_runner_addr(char *runner_addr);
