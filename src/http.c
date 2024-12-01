#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "error.h"
#include "http.h"
#include "response.h"

#define MAX_CONNECTIONS 1
#define REQUEST_BUFFER_SIZE 4096

err_code send_bad_request(RESPONSE *response)
{
	err_code err = 0;
	if ((err = status(response, 400))) {
		debug_code(err);
		return err;
	}

	char *body = "<h1>Bad Request</h1><p>Your browser sent a request that this "
				 "server could not understand.</p>";

	if ((err = send_body(response, body, strlen(body)))) {
		debug_code(err);
		return err;
	}

	return 0;
}

err_code send_internal_error(RESPONSE *response)
{
	err_code err = 0;
	if ((err = status(response, 500))) {
		debug_code(err);
		return err;
	}

	char *body = "<h1>Internal Server Error</h1><p>The server encountered an "
				 "internal error or misconfiguration and was unable to "
				 "complete your request.</p>";

	if ((err = send_body(response, body, strlen(body)))) {
		debug_code(err);
		return err;
	}

	return 0;
}

err_code http_listen(err_code (*handler)(struct Request *request,
										 RESPONSE *response),
					 uint16_t port)
{
	err_code err = 0;

	int server_fd, socket_fd = -1;
	if ((server_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		perror("Failed to create socket");
		debug_code(ERR_SOCKET);
		return ERR_SOCKET;
	}

	// Allow socket address and port to be reused.
	// int opt = 1;
	// if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
	// 			   sizeof(opt))) {
	// 	perror("Failed to set socket options");
	// 	exit(EXIT_FAILURE);
	// }

	struct sockaddr_in address = {.sin_family = AF_INET,
								  .sin_addr.s_addr = INADDR_ANY,
								  .sin_port = htons(port)};
	socklen_t address_len = sizeof(address);
	if (bind(server_fd, (struct sockaddr *)&address, address_len) < 0) {
		close(server_fd);

		perror("Failed to bind server");
		debug_code(ERR_SOCKET);
		return ERR_SOCKET;
	}

	if (listen(server_fd, MAX_CONNECTIONS) < 0) {
		close(server_fd);

		perror("Failed to listen to socket");
		debug_code(ERR_SOCKET);
		return ERR_SOCKET;
	}

	printf("Listening on http://127.0.0.1:%d\n", port);

	// Loop forever to fulfill each request.
	for (;;) {
		char request_buffer[REQUEST_BUFFER_SIZE] = {0};

		if ((socket_fd = accept(server_fd, (struct sockaddr *)&address,
								&address_len)) < 0) {
			perror("Failed to accept socket connection");
			debug_code((err = ERR_SOCKET));
			break;
		}

		if (read(socket_fd, request_buffer, REQUEST_BUFFER_SIZE) < 0) {
			perror("Failed to read from socket");
			debug_code((err = ERR_SOCKET));
			break;
		}

		RESPONSE *response = NULL;
		if ((err = create_response(socket_fd, &response))) {
			debug_code(err);

		} else {
			struct Request *request = NULL;
			if ((err = parse_request(request_buffer, REQUEST_BUFFER_SIZE,
									 &request))) {
				send_bad_request(response);

				printf("400 Bad Request\n");
				debug_code(err);
			} else {
				if ((err = handler(request, response))) {
					send_internal_error(response);

					printf("500 Internal Server error\n");
					debug_code(err);
				}

				if ((err = dispose_request(&request))) {
					perror("Failed to dispose of request object");
					debug_code(err);
					break;
				}
			}

			if ((err = dispose_response(&response))) {
				perror("Failed to dispose of response object");
				debug_code(err);
				break;
			}
		}

		if (shutdown(socket_fd, SHUT_RDWR) < 0) {
			perror("Failed to shutdown socket connection");
			debug_code((err = ERR_SOCKET));
			break;
		}

		close(socket_fd);
		socket_fd = -1;
	}

	if (socket_fd >= 0)
		close(socket_fd);

	if (server_fd >= 0)
		close(server_fd);

	return err;
}
