#ifndef __RESPONSE_H
#define __REQUEST_H 1

#include <stdbool.h>
#include <stdint.h>

#include "error.h"

typedef struct Response RESPONSE;

err_code create_response(int fd, RESPONSE **res_out);
err_code dispose_response(RESPONSE **res);

err_code status(RESPONSE *res, uint16_t status);
err_code send_body(RESPONSE *res, void *buffer, size_t buffer_size);
err_code send_file(RESPONSE *res, char *path);

#endif
