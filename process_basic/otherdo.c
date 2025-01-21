#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pwd.h>

#define NAMESIZE 256
int main(int argc, char **argv)
{
	int opt;
	char username[NAMESIZE];
	struct passwd *password;
	uid_t uid;
	uid_t cur_euid;
	pid_t pid;
	while ((opt = getopt(argc, argv, "u:")) != -1) {
		switch (opt) {
		case 'u':
			strncpy(username, optarg, NAMESIZE);
			break;
		default: /* '?' */
			fprintf(stderr, "Usage: %s [-u] name\n", argv[0]);
			exit(EXIT_FAILURE);
		}
		if (opt == 'u')
			break;
	}

	/* 保存当前的EUID */
	cur_euid = geteuid();

	/* 获取指定用户的uid */
	password = getpwnam(username);
	uid = password->pw_uid;
	// printf("uid is %d\n", uid);

	/* 修改EUID */
	seteuid(uid);

	/* 创建子进程 */
	pid = fork();
	if (pid < 0) {
		perror("fork");
		exit(EXIT_FAILURE);
	}
	if (pid == 0) {
		execvp(argv[3], argv + 3);
		perror("execvp filed");
		exit(EXIT_FAILURE);
	}

	/* 恢复euid */
	seteuid(cur_euid);

	wait(NULL);

	exit(0);
}
