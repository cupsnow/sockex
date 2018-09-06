#include <unistd.h>
#include <getopt.h>
#include <signal.h>
#include <pthread.h>

#include "sockex.h"

#define TAG "MAIN"

typedef struct {
	int fd, port;
	char addr_str[INET_ADDRSTRLEN], recv_buf[1000];
} svc_t;

static char shared_buf[500];
static int log_level = aloe_log_level_info;

static const char *opt_short = "-c:vh";
static struct option opt_long[] = {
	{"connect", required_argument, NULL, 'c'},
	{"verbose", no_argument, NULL, 'v'},
	{"help", no_argument, NULL, 'h'},
	{NULL, 0, NULL, 0},
};

static void help(const char *fn) {
	aloe_log_info(TAG, "\nShow help\n"
	"COMMAND\n"
	"  %s [OPTIONS]\n"
	"\n"
	"OPTIONS\n"
	"  -c, --connect=[IP:PORT]\n"
	"                 Connect to server\n"
	"  -v, --verbose  More message\n"
	"  -h, --help     Show help\n",
	fn);
}

static int start_client(const char *ip, int port) {
	int r;
	struct sockaddr_in sin;
	socklen_t sin_len;
	svc_t *svc;

	aloe_log_debug(TAG, "Connecting to %s:%d", ip, port);

	if ((svc = malloc(sizeof(*svc))) == NULL) {
		aloe_log_error(TAG, "alloc svc");
		goto finally;
	}

	if ((svc->fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		r = errno;
		aloe_log_error(TAG, "socket: %s(%d)", strerror(r), r);
		goto finally;
	}

	memset(&sin, 0, sin_len = sizeof(sin));
	sin.sin_family = AF_INET;
	inet_pton(AF_INET, ip, &sin.sin_addr);
	sin.sin_port = htons(port);

	if (connect(svc->fd, (struct sockaddr*)&sin, sin_len) != 0) {
		r = errno;
		aloe_log_error(TAG, "connect: %s(%d)", strerror(r), r);
		goto finally;
	}

	aloe_log_info(TAG, "Connected to %s:%d", ip, port);

	while (1) {
		char data[64];
		time_t ts;
		struct tm tm;
		int len;

	    time(&ts);
	    localtime_r(&ts, &tm);
	    len = strftime(data, sizeof(data), "%Y-%m-%d %H:%M:%S", &tm);
		while (len > 0) {
			r = write(svc->fd, data, len);
			if (r < 0) {
				r = errno;
				aloe_log_error(TAG, "%s:%d failed write %s(%d)", ip, port,
						strerror(r), r);
				goto finally;
			}
			if (r == 0) {
				aloe_log_info(TAG, "%s:%d closed", ip, port);
				goto finally;
			}
			len -= r;
		}
		sleep(1);
	}
finally:
	return r;
}

static void* svc_proc(void *arg) {
	svc_t *svc = (svc_t*)arg;

	while (1) {
		int r, len;

		r = read(svc->fd, svc->recv_buf, sizeof(svc->recv_buf));
		if (r < 0) {
			r = errno;
			aloe_log_error(TAG, "%s:%d failed read %s(%d)", svc->addr_str, svc->port,
					strerror(r), r);
			goto finally;
		}
		if (r == 0) {
			aloe_log_info(TAG, "%s:%d closed", svc->addr_str, svc->port);
			goto finally;
		}
		len = r;
		if (len < sizeof(svc->recv_buf) - 1) {
			svc->recv_buf[len] = '\0';
			aloe_log_debug(TAG, "recv %d bytes: %s", len, svc->recv_buf);
		} else {
			aloe_log_debug(TAG, "recv %d bytes", len);
		}

		while (len > 0) {
			r = write(svc->fd, svc->recv_buf, len);
			if (r < 0) {
				r = errno;
				aloe_log_error(TAG, "%s:%d failed write %s(%d)", svc->addr_str, svc->port,
						strerror(r), r);
				goto finally;
			}
			if (r == 0) {
				aloe_log_info(TAG, "%s:%d closed", svc->addr_str, svc->port);
				goto finally;
			}
			len -= r;
		}
	}
finally:
	if (svc->fd != -1) close(svc->fd);
	free(svc);
	return NULL;
}

static int start_server(void) {
	int r, fd = -1, port = 0;
	struct sockaddr_in sin;
	socklen_t sin_len;
	char addr_str[INET_ADDRSTRLEN];
	svc_t *svc = NULL;

	aloe_log_debug(TAG, "Server mode");

	if ((fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
		r = errno;
		aloe_log_error(TAG, "socket: %s(%d)", strerror(r), r);
		goto finally;
	}

	memset(&sin, 0, sin_len = sizeof(sin));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	if (port > 0) sin.sin_port = htons(port);
	if (bind(fd, (struct sockaddr*)&sin, sin_len) != 0) {
		r = errno;
		aloe_log_error(TAG, "bind: %s(%d)", strerror(r), r);
		goto finally;
	}
	if (getsockname(fd, (struct sockaddr*)&sin, &sin_len) != 0) {
		r = errno;
		aloe_log_error(TAG, "getsockname: %s(%d)", strerror(r), r);
		goto finally;
	}
	if (!inet_ntop(sin.sin_family, &sin.sin_addr, addr_str, sizeof(addr_str))) {
		r = errno;
		aloe_log_error(TAG, "inet_ntop: %s(%d)", strerror(r), r);
		goto finally;
	}
	if (listen(fd, 2) != 0) {
		r = errno;
		aloe_log_error(TAG, "listen: %s(%d)", strerror(r), r);
		goto finally;
	}
	while (1) {
		struct sockaddr_in sin2;
		socklen_t sin2_len;
		pthread_t thread_id;

		aloe_log_info(TAG, "sockex listen on %s:%d", addr_str, ntohs(sin.sin_port));

		if ((svc = malloc(sizeof(*svc))) == NULL) {
			aloe_log_error(TAG, "alloc svc");
			goto finally;
		}
		sin2_len = sizeof(sin2);
		if ((svc->fd = accept(fd, (struct sockaddr*)&sin2, &sin2_len)) == -1) {
			r = errno;
			aloe_log_error(TAG, "accept: %s(%d)", strerror(r), r);
			goto finally;
		}
		if (!inet_ntop(sin2.sin_family, &sin2.sin_addr, svc->addr_str,
				sizeof(svc->addr_str))) {
			r = errno;
			aloe_log_error(TAG, "inet_ntop: %s(%d)", strerror(r), r);
			goto finally;
		}
		svc->port = ntohs(sin2.sin_port);
		aloe_log_info(TAG, "sockex accepted %s:%d", svc->addr_str, svc->port);
		if ((r = pthread_create(&thread_id, NULL, &svc_proc, svc)) != 0) {
			aloe_log_error(TAG, "pthread_create: %s(%d)", strerror(r), r);
			goto finally;
		}
		svc = NULL;
	}
finally:
	if (fd != -1) close(fd);
	if (svc) {
		if (svc->fd != -1) close(svc->fd);
		free(svc);
	}
	return r;
}

const char *aloe_newline = "\n";
const char *aloe_abbr = "(...)\n";

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

size_t aloe_log_printf(int _level, const void *func, long lno,
		const char *tag, const char *fmt, ...) {
	va_list va;
	int r, level = _level & aloe_log_mask_level,
			oe = level <= aloe_log_level_error;

	if (level > log_level) return 0;

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
	int opt_op, opt_idx, opt_help = 0, port = -1, i;
	const char *conn_addr = NULL, *ip_addr;

	optind = 0;
	while((opt_op = getopt_long(argc, argv, opt_short,
			opt_long, &opt_idx)) != -1) {
		if (opt_op == 'c') {
			conn_addr = optarg;
			continue;
		}
		if (opt_op == 'v') {
			if (log_level < aloe_log_level_verbose) log_level++;
		}
		if (opt_op == 'h') {
			opt_help = 1;
			continue;
		}
	}
	if (opt_help) {
		help(argv[0]);
		return -1;
	}

	if (conn_addr) {
		char *tok, *tok_iter;

		if ((ip_addr = strtok_r((char*)conn_addr, ": ", &tok_iter)) == NULL) {
			aloe_log_error(TAG, "failed to parse connect ip address");
			return -1;
		}

		if ((tok = strtok_r(NULL, ": ", &tok_iter)) != NULL) {
			long val = strtol(tok, NULL, 0);

			if (errno == EINVAL || errno == ERANGE) {
				aloe_log_error(TAG, "failed to parse connect port");
				return -1;
			}
			port = (int)val;
		}
	}

	return conn_addr ? start_client(ip_addr, port) : start_server();
}
