#include <stdio.h>
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
#define BUFSIZE (4096 << 11)
int main(int argc, char **argv)
{
	int count;
	if (argc != 3) {
		fprintf(stderr, "Usage error");
	}

	int len, wlen;
	int fds, fdd = 0;
	char buf[BUFSIZE];

	fds = open(argv[1], O_RDONLY);
	if (fds < 0) {
		LOG_ERROR;
		exit(1);
	}

	fdd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
	if (fdd < 0) {
		LOG_ERROR;
		close(fds);
		exit(1);
	}

	while (1) {
		len = read(fds, buf, BUFSIZE);
		if (len < 0) {
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
				LOG_ERROR;
				break;
			}
			len -= wlen;
		}
		count++;
	}
	printf("count is %d\n", count);

	close(fdd);
	close(fds);
	return 0;
}
