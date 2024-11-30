#ifndef __ERROR_H
#define __ERROR_H 1

// Zero means all is well. Anything else means something went wrong!
typedef int err_code;

enum ERROR_CODE {
	ERR_BOUNDS = 0x01,
	ERR_NULL = 0x02,
	ERR_UNSUPPORTED = 0x03,
	ERR_NOT_IMPLEMENTED = 0x04,
};

const char *error_code_str(err_code code);

#define debug_code(error_code)                                                 \
	do {                                                                       \
		if (DEBUG) {                                                           \
			const char *__errstr = error_code_str(error_code);                 \
			if (__errstr == NULL) {                                            \
				printf("ERROR CODE: %#x | %s (%s at %d)\n", error_code,        \
					   __func__, __FILE__, __LINE__);                          \
			} else {                                                           \
				printf("%s (%#x) | %s (%s at %d)\n", __errstr, error_code,     \
					   __func__, __FILE__, __LINE__);                          \
			}                                                                  \
		}                                                                      \
	} while (0)

#endif
