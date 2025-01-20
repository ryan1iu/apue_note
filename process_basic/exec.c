#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
	pid_t pid;
	printf("Begin\n");
	fflush(NULL); // !!! 在fork和exec之前一定要刷新文件缓冲区

	pid = fork();
	if (pid < 0) {
		perror("fork");
		exit(1);
	}

	if (pid == 0) {
		execlp("/home/ryan/learn_subject/unix_system_programing/process_basic/exec",
		       "boom", NULL);
		perror("execlp");
		exit(1);
	}

	wait(NULL);
	exit(0);
}
