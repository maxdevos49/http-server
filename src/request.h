#ifndef __REQUEST_H
#define __REQUEST_H 1

#include <stddef.h>

#include "error.h"

struct Request {
	// enum Method method;
	char *url;
	// struct Header **headers;
	void *body;
};

err_code parse_request(const void *data, size_t data_size,
					   struct Request **request_out);
err_code dispose_request(struct Request **request);

#endif
