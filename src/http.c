#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

#include "error.h"
#include "http.h"

#define MAX_CONNECTIONS 1
#define BUFFER_SIZE 4096

err_code http_listen(err_code (*handler)(struct Request *request),
					 uint16_t port)
{
	err_code err = 0;

	(void)handler;

	int server_fd = 0;
	if ((server_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0) {
		perror("Failed to create socket");
		exit(EXIT_FAILURE);
	}

	// Allow socket address and port to be reused.
	// int opt = 1;
	// if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
	// 			   sizeof(opt))) {
	// 	perror("Failed to set socket options");
	// 	exit(EXIT_FAILURE);
	// }

	struct sockaddr_in address = {0};
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(port);
	socklen_t address_len = sizeof(address);
	if (bind(server_fd, (struct sockaddr *)&address, address_len) < 0) {
		perror("Failed to bind server");
		exit(EXIT_FAILURE);
	}

	if (listen(server_fd, MAX_CONNECTIONS) < 0) {
		perror("Failed to listen to socket");
		exit(EXIT_FAILURE);
	}

	printf("Listening on http://127.0.0.1:%d\n", port);

	// Loop forever to fulfill each request.
	for (;;) {
		char buffer[BUFFER_SIZE] = {0};

		int socket_fd;
		if ((socket_fd = accept(server_fd, (struct sockaddr *)&address,
								&address_len)) < 0) {
			perror("Failed to accept socket connection");
			close(server_fd);
			exit(EXIT_FAILURE);
		}

		if (read(socket_fd, buffer, BUFFER_SIZE) < 0) {
			perror("Failed to read from socket");
			close(socket_fd);
			close(server_fd);
			exit(EXIT_FAILURE);
		}

		printf("Request!\n");
		printf("%s\n", buffer);

		struct Request *request = NULL;
		if ((err = parse_request(buffer, BUFFER_SIZE, &request))) {
			// TODO 400 bad request response
			return err;
		}

		if ((err = handler(request))) {
			// TODO 500 response
			return err;
		}

		// shutdown(2): Closes a sockets read and writing
		if (shutdown(socket_fd, SHUT_RDWR) < 0) {
			perror("Failed to shutdown socket connection");
			close(socket_fd);
			close(server_fd);
			exit(EXIT_FAILURE);
		}

		// Close socket file descriptor
		close(socket_fd);
	}

	// Close socket server file descriptor
	close(server_fd);

	return 0;
}
