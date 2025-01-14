#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* 将标准输出重定向到test.txt中 */
int main()
{
	int fd = open("test.txt", O_WRONLY | O_CREAT | O_APPEND, 0666);
	if (fd < 0) {
		perror("open failed");
		exit(1);
	}

	dup2(fd, 1);

	if (fd != 1)
		close(fd);

	puts("hello, world\n");

	/* 模块化思想，在结束调用之前恢复原状 */
	// dup2(, 1);

	// puts("hello, world2\n");
	exit(0);
}
