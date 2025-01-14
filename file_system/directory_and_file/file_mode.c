

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
static void print_mode(int fd)
{
	struct stat statres;
	if (fstat(fd, &statres) < 0) {
		perror("fstat() failed\n");
	}
	switch (statres.st_mode & S_IFMT) {
	case S_IFBLK:
		printf("block device\n");
		break;
	case S_IFCHR:
		printf("character device\n");
		break;
	case S_IFDIR:
		printf("directory\n");
		break;
	case S_IFIFO:
		printf("FIFO/pipe\n");
		break;
	case S_IFLNK:
		printf("symlink\n");
		break;
	case S_IFREG:
		printf("regular file\n");
		break;
	case S_IFSOCK:
		printf("socket\n");
		break;
	default:
		printf("unknown?\n");
		break;
	}
}

int main(int argc, char **argv)
{
	int fd = -1;
	if (argc != 2) {
		fprintf(stderr, "Usage wrong\n");
	}

	fd = open(argv[1], O_RDONLY);
	if (fd < 0) {
		perror("open failed\n");
		return -1;
	}

	print_mode(fd);
}
