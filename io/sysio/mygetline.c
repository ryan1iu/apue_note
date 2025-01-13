#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include "mygetline.h"

int mygetline(int fd, char **ptr, int *n)
{
	int i = 0;
	char ch;
	int count = 1;
	int ret;
	if (*ptr == NULL) {
		/* 如果ptr是NULL，说明是第一次调用当前函数，因此需要malloc一块初始的空间 */
		*ptr = malloc(INIT_ALLOC_SIZE__ * count);
	} else {
		count = (*n) / INIT_ALLOC_SIZE__;
	}

	while (1) {
		ret = read(fd, &ch, 1);
		if (ret == 0) {
			// 文件读取到末尾
			break;
		} else if (ret < 0) {
			LOG_ERROR;
			free(*ptr);
			return -1;
		}

		// 如果内存不足，扩展空间
		if (i >= INIT_ALLOC_SIZE__ * (++count)) {
			*ptr = realloc(*ptr, INIT_ALLOC_SIZE__ * count);
			if (*ptr == NULL) {
				perror("realloc failed");
				return -1;
			}
		}

		(*ptr)[i++] = ch;

		// 如果读取到换行符，终止
		if (ch == '\n') {
			break;
		}
	}

	*n = INIT_ALLOC_SIZE__ * count;
	return i;
}
