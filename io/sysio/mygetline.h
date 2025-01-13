#ifndef MYGETLINE_H__
#define MYGETLINE_H__
#include <errno.h>

#define INIT_ALLOC_SIZE__ 64
#define LOG_ERROR                                                     \
	{                                                             \
		fprintf(stderr, "%s:%d %s\n", __FUNCTION__, __LINE__, \
			strerror(errno));                             \
	}

int mygetline(int fd, char **ptr, int *n);

#endif
