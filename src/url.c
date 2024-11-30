#include "url.h"
#include "error.h"

err_code parse_url(const char *href, size_t href_size, struct URL **url_out)
{
	*url_out = NULL;

	// TODO not implemented
	(void)href;
	(void)href_size;
	(void)url_out;

	return ERR_NOT_IMPLEMENTED;
}

err_code dispose_url(struct URL **url)
{

	// TODO not implemented
	(void)url;

	return ERR_NOT_IMPLEMENTED;
}
