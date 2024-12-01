#include <stdio.h>
#include <string.h>

#include "error.h"
#include "http.h"
#include "request.h"
#include "response.h"

#define PORT 8080

err_code handler(struct Request *req, RESPONSE *res)
{
	err_code err = 0;
	printf("<-- %s %s\n", req->method, req->uri);

	// Only Accept GET requests
	if (strcmp(req->method, "GET") != 0) {
		if ((err = status(res, 405))) {
			debug_code(err);
			return err;
		}

		return 0;
	}

	if (strcmp(req->uri, "/index.html") == 0) {
		if ((err = send_file(res, "./wwwroot/index.html"))) {
			debug_code(err);
			return err;
		}
	} else if (strcmp(req->uri, "/index.css") == 0) {
		if ((err = send_file(res, "./wwwroot/index.css"))) {
			debug_code(err);
			return err;
		}
	} else if (strcmp(req->uri, "/index.js") == 0) {
		if ((err = send_file(res, "./wwwroot/index.js"))) {
			debug_code(err);
			return err;
		}
	} else if (strcmp(req->uri, "/favicon.png") == 0) {
		if ((err = send_file(res, "./wwwroot/favicon.png"))) {
			debug_code(err);
			return err;
		}
	} else {
		if ((err = status(res, 404))) {
			debug_code(err);
			return err;
		}

		char *body = "<h1>404 - Not found</h1>";
		if ((err = send_body(res, body, strlen(body)))) {
			debug_code(err);
			return err;
		}
	}

	return 0;
}

int main(void)
{
	err_code err = 0;
	if ((err = http_listen(&handler, PORT))) {
		debug_code(err);
		return err;
	}

	return 0;
}
