#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>

#define CMDSIZE 20
struct cmd_st {
	int argc;
	char **argv;
};
void promot()
{
	printf("\033[1;32mmysh > \033[0m");
}

static void parse(char *linebuf, struct cmd_st *cmd)
{
	char *token = NULL;
	int i = 0;
	token = strtok(linebuf, " \n\t");
	while (token != NULL) {
		if (i > CMDSIZE) {
			fprintf(stderr, "The command has too many arguments\n");
			return;
		}
		// cmd->argv[i] = malloc(strlen(token) + 1);
		// strncpy(cmd->argv[i], token, strlen(token) + 1);
		cmd->argv[i] = strdup(token);
		i++;
		token = strtok(NULL, " \n\t");
	}
	cmd->argc = i;
}

int main()
{
	pid_t pid;
	char *linebuf = NULL;
	unsigned long line_length = 0;
	struct cmd_st cmd;
	cmd.argv = malloc(sizeof(char *) * CMDSIZE);
	while (1) {
		promot();
		sleep(1);
		if (getline(&linebuf, &line_length, stdin) < 0)
			break;

		/* 解析外部命令 */
		parse(linebuf, &cmd);

		/* 创建子进程 */
		pid = fork();
		if (pid < 0) {
			perror("fork");
			exit(1);
		}
		if (pid == 0) {
			execv(cmd.argv[0], cmd.argv);
			perror("execvp");
			exit(1);
		}

		/* 释放cmd_st堆内存 */
		for (int i = 0; i < cmd.argc; i++) {
			free(cmd.argv[i]);
		}
		wait(NULL);
	}
	free(cmd.argv);

	exit(0);
}
