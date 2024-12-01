#ifndef __HTTP_H
#define __HTTP_H 1

#include <stdint.h>

#include "error.h"
#include "request.h"
#include "response.h"

err_code http_listen(err_code (*handler)(struct Request *request,
										 RESPONSE *response),
					 uint16_t port);

#endif
