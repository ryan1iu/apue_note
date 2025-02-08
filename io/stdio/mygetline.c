/* 这个写的有问题，懒得改了 */
#include "stdio.h"
#include <stdio.h>
#include <stdlib.h>
#define INIT_ALLOC_SIZE__ 120

int mygetline(char **ptr, int *n, FILE *fp)
{
	if (fp == NULL)
		return -1;
	int i = 0;
	char *chr = NULL;
	char ch;
	int count = 1;
	if (ptr == NULL) {
		/* 如果ptr是NULL，说明是第一次调用当前函数，因此需要malloc一块初始的空间 */
		chr = malloc(INIT_ALLOC_SIZE__ * count);
	}
	ch = fgetc(fp);
	while (ch != '\0') {
		/* 如果申请的内存空间不足以存放读取的数据 */
		if ((i) >= INIT_ALLOC_SIZE__ * count) {
			count++;
			/* 每次以INIT_ALLOC_SIZE__大小递增 */
			chr = realloc(chr, INIT_ALLOC_SIZE__ * count);
			if (chr == NULL)
				return -2;
		}
		chr[i] = ch;
		(i)++;
		if (ch == '\n')
			break;
		ch = fgetc(fp);
	}
	*n = INIT_ALLOC_SIZE__ * count;
	return i;
}

int mygetline_free(FILE *fp);
