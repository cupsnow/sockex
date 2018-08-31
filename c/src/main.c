#include <unistd.h>

#include "sockex.h"

const char *aloe_newline = "\n";
const char *aloe_abbr = "(...)\n";

static char shared_buf[500];
#define TAG "MAIN"

size_t aloe_log_ts(void *_data, size_t cap, int level, const void *func,
		long lno, const char *tag) {
	char *data = (char*)_data;
	time_t ts;
	struct tm tm;
	int r;

    time(&ts);
    localtime_r(&ts, &tm);
    return strftime(data, cap, "%Y-%m-%d %H:%M:%S", &tm);
}

size_t aloe_log_printf(int level, const void *func, long lno,
		const char *tag, const char *fmt, ...) {
	va_list va;
	int r, oe = (level & aloe_log_mask_level) <= aloe_log_level_error;

	va_start(va, fmt);
	r = aloe_log_vsnprintf(shared_buf, sizeof(shared_buf),
			level | aloe_log_mask_force_ln | aloe_log_mask_show_ts,
			func, lno, tag, fmt, va);
	va_end(va);

	if (r > 0) {
		fprintf(oe ? stderr : stdout, "%s", shared_buf);
//		fsync(oe ? STDERR_FILENO : STDOUT_FILENO);
	}
	return r;
}

int main(int argc, char **argv) {
	aloe_log_debug(TAG, "ddddddddddddd");
	aloe_log_error(TAG, "eeeeeeeeeeeeee");
	aloe_log_info(TAG, "iiiiiiiiiiii");
	return 0;
}
