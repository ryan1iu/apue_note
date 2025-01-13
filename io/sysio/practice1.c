/* 练习：实现删除指定文件的某一行，只能使用系统调用函数 
 * 实现思路：将一个文件打开两次，两个文件描述符指向同一个文件
 * 一个文件描述符用来读，另一个负责写
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <unistd.h>

#include "mygetline.h"

#define LOG_ERROR                                                     \
	{                                                             \
		fprintf(stderr, "%s:%d %s\n", __FUNCTION__, __LINE__, \
			strerror(errno));                             \
	}

int seek_init(int fd1, int fd2, int line)
{
	char ch[1];
	int len1, len2 = 0;
	/* fd1定位到要删除行的下一行行首位置，fd2定位到删除行的行首位置 */
	while (line > 1) {
		len2 = read(fd2, ch, 1);
		if (len2 < 0) {
			LOG_ERROR;
			return -1;
		}
		if (len2 == 0) {
			fprintf(stderr, "line %d doesn't exist\n", line);
			return -1;
		}
		if (ch[0] == '\n' || ch[0] == '\0')
			line--;
	}

	/* 获取fd2文件位置指针偏移量，并让fd1指向相同位置 */
	off_t cur2 = lseek(fd2, 0, SEEK_CUR);
	if (cur2 < 0) {
		LOG_ERROR;
		return -1;
	}
	off_t cur1 = lseek(fd1, cur2, SEEK_CUR);
	if (cur1 < 0) {
		LOG_ERROR;
		return -1;
	}

	/* 将fd1文件指针向后偏移一行 */
	while (1) {
		len1 = read(fd1, ch, 1);
		if (len1 < 0) {
			LOG_ERROR;
			return -1;
		}
		if (len1 == 0) {
			fprintf(stderr, "line %d doesn't exist\n", line);
			return -1;
		}
		if (ch[0] == '\n' || ch[0] == '\0')
			break;
	}
	cur1 = lseek(fd1, 0, SEEK_CUR);
	// fprintf(stdout, "seek1 is %ld, seek2 is %ld\n", cur1, cur2);
	return cur1 - cur2;
}

void exit_(int fd1, int fd2)
{
	close(fd1);
	close(fd2);
}

int main(int argc, char **argv)
{
	int line = atoi(argv[2]);

	if (argc != 3) {
		fprintf(stderr, "Usage wrong");
		return -1;
	}

	/* 打开文件 */
	int fd1 = open(argv[1], O_RDONLY);
	if (fd1 < 0) {
		LOG_ERROR;
		return -1;
	}

	int fd2 = open(argv[1], O_RDWR);
	if (fd2 < 0) {
		LOG_ERROR;
		close(fd1);
		return -1;
	}

	/* 初始化文件位置指针 */
	int line_len = 0;
	if ((line_len = seek_init(fd1, fd2, line)) < 0) {
		exit_(fd1, fd2);
		return -1;
	}
#if 1
	/* 从fd1读取一行 */
	char *buf = NULL;
	int n = 0;
	int len = 0;

	while (1) {
		if ((len = mygetline(fd1, &buf, &n)) < 0) {
			free(buf);
			exit_(fd1, fd2);
			return -1;
		}
		fprintf(stderr, "mygetline len is %d\n", len);
		if (len == 0)
			break;
		/* 向fd2写入一行 */
		if (write(fd2, buf, len) < 0) {
			LOG_ERROR;
			free(buf);
			exit_(fd1, fd2);
			return -1;
		}
	}
	free(buf);
#endif

	/* 将fd2文件截断为length(fd1) - line_len */
	int fd1_seek = lseek(fd1, 0, SEEK_CUR);
	ftruncate(fd2, fd1_seek - line_len);
	exit_(fd1, fd2);
	return 0;
}
