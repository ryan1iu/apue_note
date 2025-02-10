/*
 * 每个线程从同一个文件中读取一个数字加一，然后写回
 * 使用信号量数组实现
 */

#include <sys/sem.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define THDNUM 20
#define FILENAME "./lockfile.txt"

static int semid;

static void rw()
{
	FILE *fp;
	char *bufline = NULL;
	size_t liensize;
	struct sembuf sembuf;
	sembuf.sem_num = 0;
	sembuf.sem_flg = 0;

	fp = fopen(FILENAME, "w+");
	if (fp == NULL) {
		perror("fopen");
		exit(1);
	}

	// 临界区
	sembuf.sem_op = -1;
	semop(semid, &sembuf, 0); // P

	getline(&bufline, &liensize, fp);
	fseek(fp, 0, SEEK_SET);
	fprintf(fp, "%d\n", atoi(bufline) + 1);

	sembuf.sem_op = 1;
	semop(semid, &sembuf, 0); // V

	free(bufline);
	fclose(fp);
}

int main()
{
	int pid;
	int i = 0;

	// 获取一个信号量集合
	semid = semget(IPC_PRIVATE, 1, 0600);

	// 设置信号量资源数为1以充当互斥量
	semctl(semid, 0, SETVAL, 1);
	for (i = 0; i < THDNUM; ++i) {
		pid = fork();
		if (pid == 0) {
			rw();
			exit(0);
		}
	}

	for (int j = 0; j < i; ++j) {
		wait(NULL);
	}

	exit(0);
}
