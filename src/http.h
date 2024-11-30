#ifndef __HTTP_H
#define __HTTP_H 1

#include <stdint.h>

#include "error.h"
#include "request.h"

err_code http_listen(err_code (*handler)(struct Request *request),
				 uint16_t port);

#endif
