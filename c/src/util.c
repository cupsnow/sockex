#include "sockex.h"

size_t aloe_log_vsnprintf(void *_data, size_t cap, int _level, const void *func,
		long lno, const char *tag, const char *fmt, va_list ap) {
	char *data = (char*)_data;
	int r, lmt, level = _level & aloe_log_mask_level,
			force_ln = _level & aloe_log_mask_force_ln,
			show_ts = _level & aloe_log_mask_show_ts;

	if (!data || !cap) return 0;
	lmt = 0;

	if (level == aloe_log_level_fatal) {
		r = snprintf(data + lmt, cap - lmt, "F ");
	} else if (level == aloe_log_level_error) {
		r = snprintf(data + lmt, cap - lmt, "E ");
	} else if (level == aloe_log_level_info) {
		r = snprintf(data + lmt, cap - lmt, "I ");
	} else if (level == aloe_log_level_debug) {
		r = snprintf(data + lmt, cap - lmt, "D ");
	} else if (level == aloe_log_level_verbose) {
		r = snprintf(data + lmt, cap - lmt, "V ");
	} else {
		r = 0;
	}
	if (r < 0 || (lmt += r) >= cap) goto abbr;

	if (show_ts) {
		r = aloe_log_ts(data + lmt, cap - lmt, level, func, lno, tag);
		if (r < 0 || (lmt += r) >= cap) goto abbr;
		*(data + lmt) = ' ';
		if ((lmt += 1) >= cap) goto abbr;
	}

	if (tag) {
		r = snprintf(data + lmt, cap - lmt, "%s ", (char*)tag);
		if (r < 0 || (lmt += r) >= cap) goto abbr;
	}

	if (func) {
		r = snprintf(data + lmt, cap - lmt, "%s()", (char*)func);
		if (r < 0 || (lmt += r) >= cap) goto abbr;
	}

	r = snprintf(data + lmt, cap - lmt, "#%ld ", lno);
	if (r < 0 || (lmt += r) >= cap) goto abbr;

	if (fmt) {
		r = vsnprintf(data + lmt, cap - lmt, fmt, ap);
		if (r < 0 || (lmt += r) >= cap) goto abbr;
	}

	if (force_ln && data[lmt - 1] != '\n' && data[lmt - 1] != '\r') {
		r = strlen(aloe_newline);
		if (lmt + r >= cap) goto abbr;
		memcpy(data + lmt, aloe_newline, r);
		lmt += r;
	}
	goto finally;
abbr:
	r = strlen(aloe_abbr);
	if (lmt >= cap - r - 1) lmt = cap - r - 1;
	memcpy(data + lmt, aloe_abbr, r);
	lmt += r;
finally:
	if (lmt >= cap) lmt = cap - 1;
	data[lmt] = '\0';
	return lmt;
}
