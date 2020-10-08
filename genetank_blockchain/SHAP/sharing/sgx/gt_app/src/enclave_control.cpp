#include "enclave_control.h"
#include "stdafx.h"

# define TOKEN_FILENAME   "enclave.token"
# define ENCLAVE_FILENAME "./build/gt_enclave.signed.so"

#include "../gt_enclave_u.h"
//#include "ecalls.h"

#define FILE_DIR_NAME "files"

#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/select.h>
#include <termios.h>
#include "tools.h"
#include <unistd.h>

struct termios orig_termios;

void reset_terminal_mode()
{
    tcsetattr(0, TCSANOW, &orig_termios);
}

void set_conio_terminal_mode()
{
    struct termios new_termios;

    /* take two copies - one for now, one for later */
    tcgetattr(0, &orig_termios);
    memcpy(&new_termios, &orig_termios, sizeof(new_termios));

    /* register cleanup handler, and set the new terminal mode */
    atexit(reset_terminal_mode);
    cfmakeraw(&new_termios);
    tcsetattr(0, TCSANOW, &new_termios);
}

int getch()
{
    int r;
    unsigned char c;
    if ((r = read(0, &c, sizeof(c))) < 0) {
        return r;
    } else {
        return c;
    }
}


typedef struct _sgx_errlist_t {
	sgx_status_t err;
	const char *msg;
	const char *sug; // Suggestion
} sgx_errlist_t;

// Error code returned by sgx_create_enclave
static sgx_errlist_t sgx_errlist[] = {
	{
		SGX_ERROR_UNEXPECTED,
		"Unexpected error occurred.",
		NULL
	},
	{
		SGX_ERROR_INVALID_PARAMETER,
		"Invalid parameter.",
		NULL
	},
	{
		SGX_ERROR_OUT_OF_MEMORY,
		"Out of memory.",
		NULL
	},
	{
		SGX_ERROR_ENCLAVE_LOST,
		"Power transition occurred.",
		"Please refer to the sample \"PowerTransition\" for details."
	},
	{
		SGX_ERROR_INVALID_ENCLAVE,
		"Invalid enclave image.",
		NULL
	},
	{
		SGX_ERROR_INVALID_ENCLAVE_ID,
		"Invalid enclave identification.",
		NULL
	},
	{
		SGX_ERROR_INVALID_SIGNATURE,
		"Invalid enclave signature.",
		NULL
	},
	{
		SGX_ERROR_OUT_OF_EPC,
		"Out of EPC memory.",
		NULL
	},
	{
		SGX_ERROR_NO_DEVICE,
		"Invalid SGX device.",
		"Please make sure SGX module is enabled in the BIOS, and install SGX driver afterwards."
	},
	{
		SGX_ERROR_MEMORY_MAP_CONFLICT,
		"Memory map conflicted.",
		NULL
	},
	{
		SGX_ERROR_INVALID_METADATA,
		"Invalid enclave metadata.",
		NULL
	},
	{
		SGX_ERROR_DEVICE_BUSY,
		"SGX device was busy.",
		NULL
	},
	{
		SGX_ERROR_INVALID_VERSION,
		"Enclave version was invalid.",
		NULL
	},
	{
		SGX_ERROR_INVALID_ATTRIBUTE,
		"Enclave was not authorized.",
		NULL
	},
	{
		SGX_ERROR_ENCLAVE_FILE_ACCESS,
		"Can't open enclave file.",
		NULL
	},
	{
		SGX_ERROR_NDEBUG_ENCLAVE,
		"The enclave is signed as product enclave, and can not be created as debuggable enclave.",
		NULL
	},
};

// Check error conditions for loading enclave
void print_error_message(sgx_status_t ret)
{
	size_t idx = 0;
	size_t ttl = sizeof sgx_errlist / sizeof sgx_errlist[0];

	for (idx = 0; idx < ttl; idx++) {
		if (ret == sgx_errlist[idx].err) {
			if (NULL != sgx_errlist[idx].sug)
				std::cout<<"Info: "<<sgx_errlist[idx].sug<<std::endl;
			std::cout<<"Error: "<<sgx_errlist[idx].msg<<std::endl;
			break;
		}
	}

	if (idx == ttl)
		std::cout<<"Error code is "<<std::hex<<ret<<". Please refer to the \"Intel SGX SDK Developer Reference\" for more details.\n"<<std::endl;
}

sgx_status_t EnclaveControl::init_enclave(uint32_t stakeholderId, const uint8_t *message, size_t msgLen, uint8_t *iv,
		uint8_t *respBuff, size_t respBuffSize, size_t *respSize){
	sgx_status_t init_ret, retval;
	//uint32_t forceInit=0;
	std::cout<<"Ecall enclave initializing ..."<<std::endl;

	init_ret = ec_enclave_init(enclave_id, &retval, enclaveHome, strnlen(enclaveHome, MAX_PATH-1)+1, stakeholderId,
			enclave_id, message, msgLen, iv, 16, respBuff, respBuffSize, respSize);
	if (SGX_SUCCESS == init_ret) {
		if (SGX_SUCCESS == retval)
			std::cout<<"Enclave data is loaded from "<<enclaveHome<<"."<<std::endl;
		else {
			std::cout<<"Enclave data error."<<std::endl;
			return (sgx_status_t)-1;
		}
    }

    return init_ret;
}

/* Initialize the enclave:
*   Step 1: try to retrieve the launch token saved by last transaction
*   Step 2: call sgx_create_enclave to initialize an enclave instance
*   Step 3: save the launch token if it is updated
*/
sgx_status_t EnclaveControl::create_enclave(const char *home)
{
	enclaveHome = (char*)malloc(strnlen(home, MAX_PATH-1)+1);
	if (NULL == enclaveHome)
		return (sgx_status_t)-1;
	snprintf(enclaveHome, strnlen(home, MAX_PATH-1)+1, "%s", home);
	char token_path[MAX_PATH] = { '\0' };
	sgx_launch_token_t token = { 0 };
	sgx_status_t ret = SGX_ERROR_UNEXPECTED;
	int updated = 0;

	/* Step 1: try to retrieve the launch token saved by last transaction
	*         if there is no token, then create a new one.
	*/

	/* try to get the token saved in $HOME */
	//const char *home_dir = getpwuid(getuid())->pw_dir;

	if (home != NULL &&
		(strlen(home) + strlen("/") + sizeof(TOKEN_FILENAME) + 1) <= MAX_PATH) {
		/* compose the token path */
		strncpy(token_path, home, strlen(home));
		strncat(token_path, "/", strlen("/"));
		strncat(token_path, TOKEN_FILENAME, sizeof(TOKEN_FILENAME) + 1);
	}
	else {
		/* if token path is too long or $HOME is NULL */
		//strncpy(token_path, TOKEN_FILENAME, sizeof(TOKEN_FILENAME));
		return (sgx_status_t)-1;
	}

	char file_path[MAX_PATH] = {'\0'};

	if (home != NULL &&
		(strlen(home) + strlen("/") + sizeof(FILE_DIR_NAME) + 1) <= MAX_PATH) {
		struct stat st = {0};

		/* compose the files directory path */
		strncpy(file_path, home, strlen(home));
		strncat(file_path, "/", strlen("/"));
		strncat(file_path, FILE_DIR_NAME, sizeof(FILE_DIR_NAME) + 1);
		if (stat(file_path, &st) == -1) {
			mkdir(file_path, 0700);
		}
	}
	else return (sgx_status_t)-1;

	FILE *fp = fopen(token_path, "rb");
	if (fp == NULL && (fp = fopen(token_path, "wb")) == NULL) {
		std::cout<<"Warning: Failed to create/open the launch token file \""<<token_path<<"\"."<<std::endl;
	}

	if (fp != NULL) {
		/* read the token from saved file */
		size_t read_num = fread(token, 1, sizeof(sgx_launch_token_t), fp);
		if (read_num != 0 && read_num != sizeof(sgx_launch_token_t)) {
			/* if token is invalid, clear the buffer */
			memset(&token, 0x0, sizeof(sgx_launch_token_t));
			std::cout<<"Warning: Invalid launch token read from \""<<token_path<<"\"."<<std::endl;
		}
	}

	/* Step 2: call sgx_create_enclave to initialize an enclave instance */
	/* Debug Support: set 2nd parameter to 1 */
	std::cout<<"Creating enclave..."<<std::endl;
	ret = sgx_create_enclave(_T(ENCLAVE_FILENAME), SGX_DEBUG_FLAG, &token, &updated, &enclave_id, NULL);
	if (ret != SGX_SUCCESS) {
		print_error_message(ret);
		if (fp != NULL) fclose(fp);
		return (sgx_status_t)-1;
	}

	/* Step 3: save the launch token if it is updated */
	if (fp != NULL)
		fclose(fp);

	if (updated){
		/* open the file with write capablity */
		fp = fopen(token_path, "wb");
		if (fp == NULL)
			return (sgx_status_t)-1;
		size_t write_num = fwrite(token, 1, sizeof(sgx_launch_token_t), fp);
		if (write_num != sizeof(sgx_launch_token_t))
			std::cout<<"Warning: Failed to save launch token to \""<<token_path<<"\"."<<std::endl;
		fclose(fp);
	}

	return SGX_SUCCESS;
}

EnclaveControl::~EnclaveControl() {
	/* Destroy the enclave */
	if (0 == enclave_id)
		return;
	sgx_status_t ret;
	ec_Py_clean(enclave_id, &ret);
	if (SGX_SUCCESS == sgx_destroy_enclave(enclave_id))
		std::cout<<"Enclave destroyed "<<enclave_id<<std::endl;
	else
		std::cout<<"Enclave "<<enclave_id<<" destroying failed."<<std::endl;
}

sgx_status_t EnclaveControl::reset_enclave(){
	if (0 == enclave_id)
		return (sgx_status_t)-1;
	sgx_status_t retval;
	return(ec_enclave_reset(enclave_id, &retval));
}

sgx_status_t EnclaveControl::new_session(uint32_t *session_id, const char *reqMsg, void *resultVecPtr, uint8_t *iv){
	sgx_status_t ret;
	sgx_status_t retval;
	ret = ec_new_session(enclave_id, &retval, session_id, reqMsg, strlen(reqMsg)+1, resultVecPtr, iv, 16, currentSecond());
	if (SGX_SUCCESS == ret && SGX_SUCCESS == retval){
		return SGX_SUCCESS;
	}
	return (sgx_status_t)-1;
}

sgx_status_t EnclaveControl::session(sgx_status_t* retval, uint32_t session_id, const uint8_t *reqMsg, size_t reqLen,
		void *resVecPtr, uint8_t *iv){
	return ec_session(enclave_id, retval, session_id, reqMsg, reqLen,
			resVecPtr, iv, 16, currentSecond());
}

int EnclaveControl::getStakeholder(const char *ethPubkey){
	sgx_status_t ret, retval;
	uint32_t stakeholderId;
	ret = ec_getStakeholder(enclave_id, &retval, ethPubkey, &stakeholderId);
	if (SGX_SUCCESS == ret && SGX_SUCCESS == retval)
		return stakeholderId;
	return -1;
}

sgx_status_t EnclaveControl::send_in_data(const uint8_t *inBuf, size_t inSize, uint8_t **destBuf){
	sgx_status_t ret, retval;
	uint8_t *ptr;
	ret = ec_send_in_data(enclave_id, &retval, inBuf, inSize, &ptr);
	*destBuf = ptr;
	if (SGX_SUCCESS == ret && SGX_SUCCESS == retval)
		return SGX_SUCCESS;
	return SGX_SUCCESS;
}

sgx_status_t EnclaveControl::run_command(uint8_t *encBuff, size_t bufLen, void *resVecPtr, uint8_t *iv){
	sgx_status_t ret, retval;
	ret = ec_run_command(enclave_id, &retval, encBuff, bufLen, resVecPtr, iv, 16);
	if (SGX_SUCCESS == ret && SGX_SUCCESS == retval)
		return SGX_SUCCESS;
	return SGX_SUCCESS;
}

