#include <ctype.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "error.h"
#include "request.h"

#define METHOD_MAX_LENGTH 16

err_code parse_request(const void *request_data, size_t request_data_size,
					   struct Request **request_out)
{
	*request_out = malloc(sizeof(struct Request));
	if (*request_out == NULL) {
		debug_code(ERR_NOMEM);
		return ERR_NOMEM;
	}

	char *request_ptr = (char *)request_data;
	uintptr_t request_ptr_end = (uintptr_t)request_data + request_data_size;

	//
	// Parse Method
	//

	size_t method_size = 0;
	while (1) {
		if ((uintptr_t)request_ptr + method_size >= request_ptr_end) {
			debug_code(ERR_BOUNDS);
			return ERR_BOUNDS;
		}

		if (isspace(request_ptr[method_size])) {
			break;
		}

		method_size++;
	}

	(*request_out)->method = calloc(sizeof(char), method_size + 1);
	if ((*request_out)->method == NULL) {
		debug_code(ERR_NOMEM);
		return ERR_NOMEM;
	}

	strncpy((*request_out)->method, request_ptr, method_size);
	(*request_out)->method[method_size] = '\0';
	request_ptr += method_size + 1;

	//
	// Parse URI
	//

	size_t uri_size = 0;
	while (1) {
		if ((uintptr_t)request_ptr + uri_size >= request_ptr_end) {
			debug_code(ERR_BOUNDS);
			return ERR_BOUNDS;
		}

		if (isspace(request_ptr[uri_size])) {
			break;
		}

		uri_size++;
	}

	(*request_out)->uri = calloc(sizeof(char), uri_size + 1);
	if ((*request_out)->uri == NULL) {
		debug_code(ERR_NOMEM);
		return ERR_NOMEM;
	}

	strncpy((*request_out)->uri, request_ptr, uri_size);
	(*request_out)->uri[uri_size] = '\0';
	request_ptr += uri_size + 1;

	//
	// Parse Version
	//

	size_t version_size = 0;
	while (1) {
		if ((uintptr_t)request_ptr + version_size >= request_ptr_end) {
			debug_code(ERR_BOUNDS);
			return ERR_BOUNDS;
		}

		if (isspace(request_ptr[version_size])) {
			break;
		}

		version_size++;
	}

	(*request_out)->version = calloc(sizeof(char), version_size + 1);
	if ((*request_out)->version == NULL) {
		debug_code(ERR_NOMEM);
		return ERR_NOMEM;
	}

	strncpy((*request_out)->version, request_ptr, version_size);
	(*request_out)->version[version_size] = '\0';
	request_ptr += version_size;

	return 0;
}

err_code dispose_request(struct Request **request)
{
	if (request == NULL || *request == NULL) {
		debug_code(ERR_NULL);
		return ERR_NULL;
	}

	if ((*request)->method != NULL) {
		free((*request)->method);
	}

	if ((*request)->uri != NULL) {
		free((*request)->uri);
	}

	if ((*request)->version != NULL) {
		free((*request)->version);
	}

	free(*request);
	*request = NULL;

	return 0;
}
