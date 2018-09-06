#ifndef _H_SOCKEX_SOCKEX
#define _H_SOCKEX_SOCKEX

#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>
#include <errno.h>

#include <sys/types.h>

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ALOE_ARRAYSIZE(_arr) (sizeof(_arr) / sizeof((_arr)[0]))

#define aloe_log_debug(...) aloe_log_printf( \
		aloe_log_level_debug | aloe_log_mask_force_ln | aloe_log_mask_show_ts, \
		__func__, __LINE__, ##__VA_ARGS__)
#define aloe_log_error(...) aloe_log_printf( \
		aloe_log_level_error | aloe_log_mask_force_ln | aloe_log_mask_show_ts, \
		__func__, __LINE__, ##__VA_ARGS__)
#define aloe_log_info(...) aloe_log_printf( \
		aloe_log_level_info | aloe_log_mask_force_ln | aloe_log_mask_show_ts, \
		__func__, __LINE__, ##__VA_ARGS__)
#define aloe_log_verbose(...) aloe_log_printf( \
		aloe_log_level_verbose | aloe_log_mask_force_ln | aloe_log_mask_show_ts, \
		__func__, __LINE__, ##__VA_ARGS__)

typedef enum aloe_log_level_enum {
	aloe_log_level_fatal = (1 << 4),
	aloe_log_level_error,
	aloe_log_level_info,
	aloe_log_level_debug,
	aloe_log_level_verbose,
	aloe_log_mask_level = 0xff,
	aloe_log_mask_force_ln = 0x100,
	aloe_log_mask_show_ts = 0x200,
} aloe_log_level_t;

extern const char *aloe_newline;
extern const char *aloe_abbr;

extern size_t aloe_log_printf(int level, const void *func, long lno,
		const char *tag, const char *fmt, ...)
		__attribute__((format(printf, 5, 6)));
extern size_t aloe_log_ts(void *data, size_t cap, int level, const void *func,
		long lno, const char *tag);
size_t aloe_log_vsnprintf(void *data, size_t cap, int level, const void *func,
		long lno, const char *tag, const char *fmt, va_list ap)
		__attribute__((format(printf, 7, 0)));

#ifdef __cplusplus
} // extern "C"
#endif

#endif // _H_SOCKEX_SOCKEX
