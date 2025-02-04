/* 令牌桶实现 */
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include "./bucket.h"

#define LOG_ERROR                                                     \
	{                                                             \
		fprintf(stderr, "%s:%d %s\n", __FUNCTION__, __LINE__, \
			strerror(errno));                             \
	}
#define BUFSIZE (20)

int main(int argc, char **argv)
{
	int len, wlen;
	int fds, fdd = 0;
	char buf[BUFSIZE];
	bucket_t *bucket;

	if (bucket_init(&bucket, 10, 100) < 0) {
		fprintf(stderr, "bucket init error\n");
		exit(1);
	}
	if (argc != 2) {
		fprintf(stderr, "Usage error\n");
		exit(1);
	}

	fds = open(argv[1], O_RDONLY);
	if (fds < 0) {
		LOG_ERROR;
		exit(1);
	}

	fdd = 1;
	while (1) {
		int token = bucket_fetch(bucket, BUFSIZE);
		len = read(fds, buf, token);

		if (len < token) {
			bucket_return(bucket, token - len);
		}

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
	bucket_destory(bucket);
	close(fds);
	exit(0);
}
