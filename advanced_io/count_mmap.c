/* 计算一个文件中字节数 */
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>

int main()
{
	// 省略错误检测了
	char *file = "./count_mmap.c";
	int fd = open(file, O_RDONLY);
	struct stat statbuf;
	fstat(fd, &statbuf);

	char *mptr = mmap(NULL, statbuf.st_size, PROT_READ, MAP_PRIVATE, fd, 0);

	close(fd);

	char *p = mptr;
	int i = 0;
	while (*p != '\0') {
		i++;
		p++;
	}
	printf("count is %d\n", i);

	munmap(mptr, statbuf.st_size);

	exit(0);
}
