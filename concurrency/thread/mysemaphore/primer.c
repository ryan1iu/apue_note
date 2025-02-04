#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include "mysm.h"

#define THREADMAX 4
static sm_t *sm;
static void *primer_func(void *data)
{
	int flag = 0;
	int i = *(int *)data;
	for (int j = 2; j < i / 2; j++) {
		if (i % j == 0) {
			flag = 1;
			break;
		}
	}
	if (flag == 0) {
		printf("[%lu] %d is primer\n", pthread_self(), i);
	}
	sm_signal(sm, 1);
	pthread_exit(NULL);
}

int main()
{
	int left = 30000000;
	int right = 30000200;
	int *data = malloc(
		sizeof(int) *
		(right - left + 1)); // 每个数字单独使用一个空间，防止线程竞争
	pthread_t tid[right - left];
	int i;

	// 初始化信号量
	sm_init(&sm, THREADMAX);

	for (i = left; i <= right; i++) {
		sm_wait(sm, 1);
		data[i - left] = i;
		int err = pthread_create(tid + i - left, NULL, primer_func,
					 data + i - left);
		if (err < 0) {
			fprintf(stderr, "%s", strerror(err));
			break;
		}
	}

	for (int j = 0; j < i - left; j++) {
		if (pthread_kill(tid[j], 0) == 0) {
			pthread_join(tid[j], NULL);
		}
	}

	sm_destory(sm);

	exit(0);
}
