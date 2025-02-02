/* 令牌桶实现 */
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>

#define LOG_ERROR                                                     \
	{                                                             \
		fprintf(stderr, "%s:%d %s\n", __FUNCTION__, __LINE__, \
			strerror(errno));                             \
	}
#define BUFSIZE (10)
static volatile sig_atomic_t token = 0;
static void alarm_handler(int sig_num)
{
	alarm(1);
	token++;
}

int main(int argc, char **argv)
{
	int count;
	int len, wlen;
	int fds, fdd = 0;
	char buf[BUFSIZE];

	if (argc != 2) {
		fprintf(stderr, "Usage error\n");
		exit(0);
	}

	fds = open(argv[1], O_RDONLY);
	if (fds < 0) {
		LOG_ERROR;
		exit(1);
	}

	fdd = 1;

	signal(SIGALRM, alarm_handler);
	alarm(1);
	while (1) {
		// 用while循环的原因是防止因为其他非alam信号导致程序运行
		while (token <= 0) {
			pause();
		}
		token--;
		len = read(fds, buf, BUFSIZE);
		if (len < 0) {
			if (errno == EINTR) {
				// 如果是因为信号导致的错误
				continue;
			}
			LOG_ERROR;
			break;
		}

		/* 说明文件已经读完了 */
		if (len == 0)
			break;

		/* 坚持写 */
		while (len > 0) {
			wlen = write(fdd, buf, len);
			if (wlen <= 0) {
				if (errno == EINTR) {
					continue;
				}
				LOG_ERROR;
				break;
			}
			len -= wlen;
		}
	}
	close(fds);
	return 0;
}
