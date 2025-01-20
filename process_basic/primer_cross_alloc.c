#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

/* 钩子函数，用来清理子进程资源 */
/* 钩子函数会被子进程继承 */
void wait_childp()
{
	for (int n = 0; n < 3; n++) {
		wait(NULL);
	}
	printf("process[%d]: wait_childp end\n", getpid());
}
int main()
{
	int left = 200000;
	int right = 200200;

	atexit(wait_childp);
	/* 将计算任务交叉分配给3个进程来完成 */
	for (int n = 0; n < 3; n++) {
		pid_t pid = fork();
		if (pid < 0) {
			perror("fork()");
			exit(0);
		}
		if (pid == 0) {
			for (int i = left + n; i <= right; i += 3) {
				int flag = 0;
				for (int j = 2; j < i / 2; j++) {
					if (i % j == 0) {
						flag = 1;
						break;
					}
				}
				if (flag == 0) {
					printf("process[%d]: %d is primer\n",
					       getpid(), i);
				}
			}
			exit(0);
		}
	}
	exit(0);
}
