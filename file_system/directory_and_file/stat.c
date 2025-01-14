/* stat函数获取围绕一个文件的所有信息 */

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
static off_t get_file_len(int fd)
{
	struct stat statres;
	if (fstat(fd, &statres) < 0) {
		perror("fstat() failed\n");
		return -1;
	}
	// printf("block size is %ld\n", statres.st_blksize);

	return statres.st_size;
}

int main(int argc, char **argv)
{
	int fd = -1;
	if (argc != 2) {
		fprintf(stderr, "Usage wrong\n");
		return -1;
	}

	if ((fd = open(argv[1], O_RDONLY)) < 0) {
		perror("open() failed\n");
		return -1;
	}

	printf("file length is %ld\n", get_file_len(fd));

	close(fd);
}
