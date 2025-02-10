/* 使用IPC_PRIVATE选项创建亲缘进程之间共享内存 */
#include <sys/wait.h>
#include <sys/shm.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define MEMSIZE 512
int main()
{
	int pid;
	int shmid;
	char *mptr;

	// 获取一个共享内存段
	shmid = shmget(IPC_PRIVATE, MEMSIZE, 0600);

	// 将共享内存段附加到进程的地址空间
	// 附加到进程地址空间的共享内存段在fork时不遵循COW
	mptr = shmat(shmid, NULL, 0);

	pid = fork();
	if (pid == 0) {
		strncpy(mptr, "hello\n", 6);
		exit(0);
	}

	wait(NULL);

	puts(mptr);
	// 将内存段与进程地址空间相分离
	shmdt(mptr);
	shmctl(shmid, IPC_RMID, NULL);

	exit(0);
}
