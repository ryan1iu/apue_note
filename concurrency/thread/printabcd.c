/* 创建四个进程，按照abcd的顺序循环打印 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

static pthread_mutex_t mutex[4];

static void *func(void *ch)
{
	int i = *(int *)ch;
	char chv = 'a' + i;

	while (1) {
		pthread_mutex_lock(mutex + i);
		write(1, &chv, 1);
		pthread_mutex_unlock(mutex + (i + 1) % 4);
	}

	pthread_exit(NULL);
}

int main()
{
	pthread_t tid[4];
	int i;
	int data[4];

	// 初始化互斥量
	for (i = 0; i < 4; ++i) {
		pthread_mutex_init(mutex + i, NULL);
	}

	// 创建4个线程
	for (i = 0; i < 4; i++) {
		data[i] = i;
		int err = pthread_create(tid + i, NULL, func, data + i);
		if (err < 0) {
			fprintf(stderr, "%s", strerror(err));
		}
	}

	pthread_mutex_unlock(mutex + 0);

	// 线程收尸
	for (int j = 0; j < i; ++j) {
		int err = pthread_join(tid[j], NULL);
		if (err < 0) {
			fprintf(stderr, "%s", strerror(err));
		}
	}

	// 销毁互斥量
	for (int i = 0; i < 4; ++i) {
		int err = pthread_mutex_destroy(mutex + i);
		if (err < 0) {
			fprintf(stderr, "%s", strerror(err));
		}
	}

	exit(0);
}
