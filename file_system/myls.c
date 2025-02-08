/* 实现思路
 * 解析命令行参数
 * 如果没指定文件或目录那么默认为当前目录
 * 如果指定了文件和目录，使用stat判断文件的类型是文件还是目录
 * 如果是目录使用glob函数拿到目录下的文件，如果选项指定了a就列出隐藏文件
 * 如果选项指定了i，就对每个文件调用stat获取其Inode
 * 如果选项指定了l, 对每个文件调用stat，然后获取文件的权限、大小、uid gid 、修改时间， 根据uid和gid获取用户名和组名
 * 如果指定了选项n,对每个文件调用stat，然后获取文件的权限、大小、uid gid 、修改时间
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#include <pwd.h>
#include <grp.h>

#define WDSIZE 512

static int show_all;
static int show_list;
static int show_inode;
static int show_numeric_uid;

int show_file_info(char *pwd, char *filename)
{
	struct passwd *password;
	struct group *group;
	struct stat statbuf;
	char datebuf[32];
	struct tm *time;
	char pathbuf[4096];

	/* 构造文件路径 */
	if (pwd != filename) {
		if (sizeof(pwd) < 4096 - 1) {
			strcat(pwd, "/");
		}
		if (sizeof(pwd) + sizeof(filename) >= 4096) {
			fprintf(stderr, "Path is too long\n");
			return -1;
		}
		snprintf(pathbuf, sizeof(pathbuf), "%s%s", pwd, filename);
	} else {
		strncpy(pathbuf, pwd, sizeof(pathbuf) - 1);
	}

	if (stat(pathbuf, &statbuf) == -1) {
		perror("stat failed");
		return -1;
	}

	if (show_inode == 1) {
		/* 打印文件inode */
		printf("%ld ", statbuf.st_ino);
	}

	if (show_list == 1 || show_numeric_uid == 1) {
		/* 打印文件类型和权限 */
		switch (statbuf.st_mode & S_IFMT) {
		case S_IFSOCK:
			printf("s");
			break;
		case S_IFLNK:
			printf("l");
			break;
		case S_IFREG:
			printf("-");
			break;
		case S_IFBLK:
			printf("b");
			break;
		case S_IFDIR:
			printf("d");
			break;
		case S_IFCHR:
			printf("c");
			break;
		case S_IFIFO:
			printf("f");
			break;
		default:
			fprintf(stderr, "Unknown file type\n");
			return -1;
		}
		printf((statbuf.st_mode & S_IRUSR) ? "r" : "-");
		printf((statbuf.st_mode & S_IWUSR) ? "w" : "-");
		printf((statbuf.st_mode & S_IXUSR) ? "x" : "-");
		printf((statbuf.st_mode & S_IRGRP) ? "r" : "-");
		printf((statbuf.st_mode & S_IWGRP) ? "w" : "-");
		printf((statbuf.st_mode & S_IXGRP) ? "x" : "-");
		printf((statbuf.st_mode & S_IROTH) ? "r" : "-");
		printf((statbuf.st_mode & S_IWOTH) ? "w" : "-");
		printf((statbuf.st_mode & S_IXOTH) ? "x" : "-");

		/* 打印链接计数 */
		printf(" %ld ", statbuf.st_nlink);

		if (show_numeric_uid == 1) {
			printf("%u %u ", statbuf.st_uid, statbuf.st_gid);
		} else {
			/* 获取用户名和组名 */
			password = getpwuid(statbuf.st_uid);
			group = getgrgid(statbuf.st_gid);

			printf("%s %s ",
			       password == NULL ? "unknown" : password->pw_name,
			       group == NULL ? "unknown" : group->gr_name);
		}

		/* 打印文件大小 */
		printf("%-6ld ", statbuf.st_size);

		/* 打印最后修改时间 */
		time = localtime(&statbuf.st_mtim.tv_sec);
		strftime(datebuf, 32, "%F %T", time);
		printf("%s ", datebuf);

		/* 打印文件名 */
		printf("%s\n", filename);
	} else {
		printf("%s ", filename);
	}

	return 0;
}

void list_directory(char *pwd)
{
	struct stat statbuf;
	DIR *dir;
	struct dirent *dent;

	/* 判断文件类型 */
	if (stat(pwd, &statbuf) == -1) {
		perror("stat failed");
		return;
	}

	if ((statbuf.st_mode & S_IFMT) == S_IFDIR) {
		/* 如果是目录 */
		dir = opendir(pwd);
		if (dir == NULL) {
			perror("opendir failed");
			return;
		}
		while ((dent = readdir(dir)) != NULL) {
			if (show_all != 1) {
				if (strcmp(".", dent->d_name) == 0 ||
				    strcmp("..", dent->d_name) == 0)
					continue;
			}
			show_file_info(pwd, dent->d_name);
		}
		closedir(dir);

	} else {
		/* 如果是文件 */
		show_file_info(pwd, pwd);
	}
}

int main(int argc, char **argv)
{
	int opt;
	char *pwd;

	/* 初始化工作目录 */
	pwd = malloc(WDSIZE);
	if (pwd == NULL) {
		perror("malloc failed");
		exit(1);
	}
	strcpy(pwd, ".");

	/* 解析命令行选项 */
	/* 关闭getopt函数的错误打印 */
	opterr = 0;
	while ((opt = getopt(argc, argv, "-alin")) != -1) {
		switch (opt) {
		case 1:
			strncpy(pwd, argv[optind - 1], WDSIZE - 1);
			break;
		case 'a':
			show_all = 1;
			break;
		case 'l':
			show_list = 1;
			break;
		case 'i':
			show_inode = 1;
			break;
		case 'n':
			show_numeric_uid = 1;
			break;
		default:
			fprintf(stderr, "Unknown argument %s\n",
				argv[optind - 1]);
			exit(1);
		}
	}

	list_directory(pwd);

	free(pwd);
	exit(0);
}
