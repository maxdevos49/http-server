#include <stddef.h>

#include "error.h"
#include "request.h"

err_code parse_request(const void *data, size_t data_size,
					   struct Request **request_out)
{
	*request_out = NULL;

	(void)data;
	(void)data_size;
	(void)request_out;

	return ERR_NOT_IMPLEMENTED;
}

err_code dispose_request(struct Request **request)
{
	(void)request;

	return ERR_NOT_IMPLEMENTED;
}
