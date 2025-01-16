/* 解析一个文件或目录所占用的磁盘空间KB */

#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#define FILE_NAME_SZIE 1024
static int64_t du(char *path)
{
	int64_t sum = 0;
	glob_t pglob;
	int globc;

	/* 判断文件类型是否为目录文件 */
	static struct stat statres;
	if (lstat(path, &statres) < 0) {
		perror("lstat() failed\n");
		return -1;
	}

	if ((statres.st_mode & S_IFMT) != S_IFDIR) {
		// printf("%s size is %ld\n", path, statres.st_blocks);
		return statres.st_blocks;
	}

	/* 如果是目录文件的话，解析该目录文件，递归调用du函数 */
	size_t path_len = strlen(path);
	if (path_len + 2 >= FILE_NAME_SZIE) {
		printf("%s\n", path);
		fprintf(stderr, "Path length exceeds buffer size\n");
		return -1;
	}
	snprintf(path + path_len, FILE_NAME_SZIE - path_len, "/*");

	if (glob(path, GLOB_PERIOD, NULL, &pglob) < 0) {
		perror("glob falied\n");
		return -1;
	}
	globc = pglob.gl_pathc;
	/* 令i从2开始，从而跳过.和..，防止死循环 */
	for (int i = 2; i < globc; i++) {
		strcpy(path, pglob.gl_pathv[i]);
		int ret = du(path);
		if (ret < 0) {
			free(path);
			globfree(&pglob);
			return -1;
		}
		sum += ret;
	}
	globfree(&pglob);
	return sum;
}

int main(int argc, char **argv)
{
	int64_t dures;
	char *path = malloc(FILE_NAME_SZIE);
	if (path == NULL) {
		fprintf(stderr, "memory allocation failed\n");
		return -1;
	}
	if (argc > 2) {
		fprintf(stderr, "Uasge is wrong\n");
		return -1;
	}

	/* 如果argc is 1, 默认解析当前目录 */
	if (argc == 1) {
		strncpy(path, ".", 2);
	} else {
		strncpy(path, argv[1], strlen(argv[1]) + 1);
	}

	dures = du(path);
	if (dures < 0) {
		fprintf(stderr, "du() failed\n");
	} else {
		printf("%ld\n", dures / 2);
	}

	free(path);
}
