#include <stddef.h>

#include "error.h"

#define STRINGIFY(s) #s

const char *error_code_str(err_code code)
{
	switch (code) {
	case ERR_BOUNDS:
		return STRINGIFY(ERR_BOUNDS);
	case ERR_NULL:
		return STRINGIFY(ERR_NULL);
	case ERR_UNSUPPORTED:
		return STRINGIFY(ERR_UNSUPPORTED);
	case ERR_NOT_IMPLEMENTED:
		return STRINGIFY(ERR_NOT_IMPLEMENTED);

		return NULL;
	}
}
