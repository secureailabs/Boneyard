#include <sys/types.h>
#include <pthread.h>
#include <unistd.h>
#include "sgx_urts.h"
#include "sgx_tcrypto.h"
#include "ecalls.h"


sgx_status_t task_manager_new_threads(int index, uint32_t stId);

sgx_status_t task_manager_app(uint32_t index,uint64_t tid, uint32_t stId);

sgx_status_t task_manager_kill_thread(pthread_t tid_app);
