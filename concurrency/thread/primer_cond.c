/*
 * 使用生产者消费者模式改写
 * main线程不断生产计算任务
 * 计算线程获取计算任务
 * 使用条件变量替换忙等
        ？使用一个条件变量还是两个，两个在逻辑上更加清晰一些
 */
#include <sched.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>

#define THRDNUM 4

static pthread_mutex_t num_mutex;
static pthread_cond_t full_cond;
static pthread_cond_t empty_cond;
static int num; // 计算任务

static void *primer_func(void *p)
{
	int flag = 0;
	int i;
	while (1) {
		// 获取一个计算任务
		pthread_mutex_lock(&num_mutex);
		while (num == 0) {
			pthread_cond_wait(&full_cond, &num_mutex);
		}
		if (num == -1) {
			pthread_mutex_unlock(&num_mutex);
			pthread_exit(NULL);
		} else {
			i = num;
			num = 0;
			pthread_cond_signal(&empty_cond); // 通知生产者进程
			pthread_mutex_unlock(&num_mutex);
		}
		pthread_mutex_unlock(&num_mutex);

		// 计算并判断是否为目标值
		for (int j = 2; j < i / 2; j++) {
			if (i % j == 0) {
				flag = 1;
				break;
			}
		}
		if (flag == 0) {
			printf("%d is primer\n", i);
		}
		flag = 0; // !!!
	}
	pthread_exit(NULL);
}

int main()
{
	int left = 30000000;
	int right = 30000200;
	pthread_t tid[THRDNUM];
	int i;

	// 初始化互斥锁
	pthread_mutex_init(&num_mutex, NULL);

	// 创建线程
	for (i = 0; i < THRDNUM; i++) {
		int err = pthread_create(tid + i, NULL, primer_func, NULL);
		if (err < 0) {
			fprintf(stderr, "%s", strerror(err));
			break;
		}
	}

	// 生产计算任务
	i = left;
	while (i <= right) {
		pthread_mutex_lock(&num_mutex);
		while (num != 0) {
			pthread_cond_wait(&empty_cond, &num_mutex);
		}
		num = i;
		pthread_cond_broadcast(&full_cond); // 通知消费者线程
		i++;
		pthread_mutex_unlock(&num_mutex);
	}

	// 监视任务是否处理完成
	pthread_mutex_lock(&num_mutex);
	while (num != 0) {
		pthread_cond_wait(&empty_cond, &num_mutex);
	}
	num = -1; // 设置完成标记
	pthread_cond_broadcast(&full_cond); // !!!
	pthread_mutex_unlock(&num_mutex);

	// 回收线程
	for (int j = 0; j < THRDNUM; j++) {
		pthread_join(tid[j], NULL);
	}

	// 销毁互斥锁，条件变量
	pthread_mutex_destroy(&num_mutex);
	pthread_cond_destroy(&empty_cond);
	pthread_cond_destroy(&full_cond);

	exit(0);
}
