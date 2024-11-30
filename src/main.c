#include <stdio.h>

#include "error.h"
#include "http.h"
#include "request.h"

#define PORT 8080

/**
 * Gets the first line in the given buffer. If the line does not contain a new
 * line buffer then it returns NULL.
 */
// int get_line(char **line, char **buffer, const int buffer_size)
// {
// 	int line_length = 0;

// 	// Read until we come across CRLF or we reach the end of the buffer.
// 	while (line_length <= buffer_size &&
// 		   (*buffer)[line_length] !=
// 			   '\n') // TODO what if the line is empty??? Currently the
// 					 // line_length returned is 1 or more. Never 0 because of
// 					 // the '\r` character.
// 	{
// 		line_length++;
// 	}

// 	if (line_length == buffer_size) {
// 		return -1;
// 	}

// 	*line = (char *)malloc(line_length * sizeof(char));

// 	// assert(*line != NULL);//TODO

// 	strncpy(*line, *buffer, line_length);
// 	// line_length-1 to get rid of the '/r'.
// 	(*line)[line_length - 1] = '\0';

// 	// Move buffer address forward to the end of the line so it is ready for the
// 	// next sgetline call.
// 	*buffer += line_length + 1;

// 	return line_length;
// }

err_code handler(struct Request *req)
{
	printf("Request URL: %s\n", req->url);
	return 0;
}

int main(void)
{
	err_code err = 0;
	if ((err = http_listen(&handler, PORT))) {
		return err;
	}

	return 0;
}
