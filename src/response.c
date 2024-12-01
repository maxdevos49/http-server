#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "response.h"

struct Response {
	int socket_fd;
	bool headers_sent;
};

err_code create_response(int fd, RESPONSE **res_out)
{
	*res_out = NULL;

	*res_out = malloc(sizeof(RESPONSE));
	if (*res_out == NULL) {
		debug_code(ERR_NOMEM);
		return ERR_NOMEM;
	}

	(*res_out)->socket_fd = fd;
	(*res_out)->headers_sent = false;

	return 0;
}

err_code dispose_response(RESPONSE **res)
{
	if (*res == NULL) {
		debug_code(ERR_NULL);
		return ERR_NULL;
	}

	free(*res);
	*res = NULL;

	return 0;
}

const char *status_text(uint16_t status)
{
	switch (status) {
	case 200:
		return "OK";
	case 400:
		return "Bad Request";
	case 405:
		return "Method Not Allowed";
	case 500:
		return "Internal Server Error";
	default:
		return "";
	}
}

err_code status(RESPONSE *res, uint16_t status)
{
	if (res->headers_sent) {
		perror("Headers already sent.");
		return ERR_UNSUPPORTED;
	}

	printf("--> %d\n", status);

	char buffer[1028];
	size_t written = snprintf(buffer, 1028, "HTTP/1.1 %d %s\r\n", status,
							  status_text(status));
	if (written <= 0) {
		perror("Response status line truncated");
		debug_code(ERR_NOMEM);
		return ERR_NOMEM;
	}

	if (write(res->socket_fd, buffer, written) < 0) {
		perror("Failed to write status line and headers.");
		debug_code(ERR_SOCKET);
		return ERR_SOCKET;
	}

	// TODO support writing response headers.

	res->headers_sent = true;

	return 0;
}

err_code send_body(RESPONSE *res, void *buffer, size_t buffer_size)
{
	if (!res->headers_sent) {
		err_code err = 0;
		if ((err = status(res, 200))) {
			debug_code(err);
			return err;
		}
	}

	if (write(res->socket_fd, "\r\n", 2) < 0) {
		perror("Failed to write separator");
		debug_code(ERR_SOCKET);
		return ERR_SOCKET;
	}

	if (write(res->socket_fd, buffer, buffer_size) < 0) {
		perror("Failed to write body");
		debug_code(ERR_SOCKET);
		return ERR_SOCKET;
	}

	return 0;
}

#define MAX_BUFFER_SIZE 4096

err_code send_file(RESPONSE *res, char *path)
{
	char body[MAX_BUFFER_SIZE + 1];
	FILE *fp = fopen(path, "r");
	if (fp == NULL) {
		debug_code(ERR_FILE);
		return ERR_FILE;
	}

	size_t len = fread(body, sizeof(char), MAX_BUFFER_SIZE, fp);
	if (ferror(fp) != 0) {
		fputs("Error reading file", stderr);
		debug_code(ERR_FILE);
		return ERR_FILE;
	}

	fclose(fp);
	err_code err = 0;
	if ((err = send_body(res, body, len))) {
		debug_code(err);
		return err;
	}

	return 0;
}
