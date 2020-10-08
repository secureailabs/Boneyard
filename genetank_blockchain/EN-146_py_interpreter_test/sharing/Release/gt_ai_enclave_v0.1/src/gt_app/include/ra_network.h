
#ifndef _NETWORKIO_H
#define _NETWORKIO_H

#include "sgx_error.h"
#include "..\include\ra_msg.h"

#ifdef _WIN32
# define iequals(x, y) (_stricmp((x), (y))==0)
#else
# define iequals(x, y) boost::iequals((x), (y))
#endif

#ifdef  __cplusplus
extern "C" {
#endif

	sgx_status_t ra_network_send_receive(const ra_samp_request_header_t *req,
		ra_samp_response_header_t **p_resp);
	void ra_free_network_response_buffer(ra_samp_response_header_t *resp);

#ifdef  __cplusplus
}
#endif

#endif