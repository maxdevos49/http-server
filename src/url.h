#ifndef __URL_H
#define __URL_H 1

#include <stdint.h>
#include <stdlib.h>

#include "error.h"

struct URL {
	char *protocol;
	char *username;
	char *password;
	char *hostname;
	uint16_t port;
	char *pathname;
	char *search;
	char *hash;

	void *private;
};

err_code parse_url(const char *href, size_t href_size, struct URL **url_out);
err_code dispose_url(struct URL **url);

#endif
