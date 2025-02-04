/* 使用多线程和互斥量改写令牌桶
 * 将之前的信号处理函数独立为一个写线程
 * 使用条件变量来替换程序中的忙等
 */
#include "./bucket.h"
#include <sched.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

// 隐藏结构体
struct bucket_st {
	int volatile token; // 持有token数
	int volatile rate; // 每秒钟增加的token数
	int volatile size; // 持有token值上限
	int index; // 存储桶下标
	pthread_mutex_t bkt_mutex; // 存储桶互斥量
	pthread_cond_t bkt_cond; // 条件变量
};

static struct bucket_st *job[BUCKET_MAX_SIZE];
static pthread_mutex_t job_mux = PTHREAD_MUTEX_INITIALIZER; // job数组互斥锁
static pthread_once_t once_control;
static pthread_t token_pd;

// 标记为unlock,表示调用这个函数前需要加锁
static int find_low_index_unlock()
{
	int i;
	// 遍历job数组，找出第一个空位
	for (i = 0; i < BUCKET_MAX_SIZE; i++) {
		if (job[i] == NULL) {
			break;
		}
	}
	if (i < BUCKET_MAX_SIZE) {
		return i;
	} else {
		return -1;
	}
}

static void *token_produce_thd(void *p)
{
	struct timespec time, timeremain;
	int nanosleep_ret;
	time.tv_sec = 1;
	time.tv_nsec = 0;
	while (1) {
		for (int i = 0; i < BUCKET_MAX_SIZE; i++) {
			pthread_mutex_lock(&job_mux);
			if (job[i] != NULL) {
				pthread_mutex_lock(&job[i]->bkt_mutex);

				int tokenadd = job[i]->token + job[i]->rate;
				job[i]->token = tokenadd > job[i]->size ?
							job[i]->size :
							tokenadd;
				// 通知
				pthread_cond_broadcast(&job[i]->bkt_cond);
				pthread_mutex_unlock(&job[i]->bkt_mutex);
			}
			pthread_mutex_unlock(&job_mux);
		}
		// 等待一秒钟
		do {
			nanosleep_ret = nanosleep(&time, &timeremain);
			if (nanosleep_ret < 0) {
				if (errno == EINTR) {
					time = timeremain;
				} else {
					perror("nanosleep");
					break;
				}
			}
		} while (nanosleep_ret < 0);
	}
	pthread_exit(NULL);
}

static void unload_module()
{
	// 关闭token生产线程
	pthread_cancel(token_pd);
	pthread_join(token_pd, NULL);

	// 销毁所有的令牌桶
	for (int i = 0; i < BUCKET_MAX_SIZE; i++) {
		if (job[i] != NULL) {
			bucket_destory(job[i]);
		}
	}

	// 销毁互斥量job_mux
	pthread_mutex_destroy(&job_mux);
}

static void load_module()
{
	// 创建生产token的线程
	int err = pthread_create(&token_pd, NULL, token_produce_thd, NULL);
	if (err < 0) {
		fprintf(stderr, "%s", strerror(err));
	}
	atexit(unload_module);
}

// 初始化一个令牌桶
int bucket_init(bucket_t **bucket, int rate, int size)
{
	int low_index;
	struct bucket_st *buk;

	// 加载且仅加载一次
	//    pthread_mutex_lock()
	// if (!load_flag) {
	// 	load_module();
	// 	load_flag = 1;
	// }
	//    pthread_mutex_unlock()
	pthread_once(&once_control, load_module);

	// 申请存储桶
	buk = malloc(sizeof(struct bucket_st));
	if (buk == NULL) {
		return -ENOMEM;
	}

	// 互斥获取index，获取index和写job数组应该在一个临界区完成
	pthread_mutex_lock(&job_mux);
	low_index = find_low_index_unlock();

	// 如果job数组空间不足
	if (low_index == -1) {
		pthread_mutex_unlock(&job_mux);
		return -1;
	}

	job[low_index] = buk;
	pthread_mutex_unlock(&job_mux);

	// 将非关键代码移出临界区
	buk->token = 0;
	buk->rate = rate;
	buk->size = size;
	buk->index = low_index;
	pthread_mutex_init(&buk->bkt_mutex, NULL);
	pthread_cond_init(&buk->bkt_cond, NULL);
	*bucket = buk;
	return 0;
}

int bucket_fetch(bucket_t *bucket, int token)
{
	struct bucket_st *bk = bucket;
	if (token < 0 || token > bk->size) {
		return -EINVAL;
	}
	// 如果桶中的token不足则忙等，实现流速控制
#if 0
	while (1) {
		pthread_mutex_lock(&bk->bkt_mutex);
		if (bk->token < token) {
			pthread_mutex_unlock(&bk->bkt_mutex);
			sched_yield();
		} else {
			bk->token -= token;
			pthread_mutex_unlock(&bk->bkt_mutex);
			break;
		}
	}
#endif
	// 使用条件变量替换忙等
	pthread_mutex_lock(&bk->bkt_mutex);
	while (bk->token < token) {
		pthread_cond_wait(&bk->bkt_cond, &bk->bkt_mutex);
	}
	bk->token -= token;
	pthread_mutex_unlock(&bk->bkt_mutex);
	return token;
}

int bucket_return(bucket_t *bucket, int token)
{
	struct bucket_st *bk = bucket;
	if (token < 0 || token > bk->size) {
		return -EINVAL;
	}

	pthread_mutex_lock(&bk->bkt_mutex);
	int tokenadd = token + bk->token;
	if (tokenadd > bk->size)
		tokenadd = bk->size;
	bk->token = tokenadd;
	pthread_cond_broadcast(&bk->bkt_cond);
	pthread_mutex_unlock(&bk->bkt_mutex);

	return 0;
}

int bucket_destory(bucket_t *bucket)
{
	struct bucket_st *bk = bucket;

	// 互斥访问job
	pthread_mutex_lock(&job_mux);
	job[bk->index] = NULL;
	pthread_mutex_unlock(&job_mux);

	pthread_mutex_destroy(&bk->bkt_mutex);
	pthread_cond_destroy(&bk->bkt_cond);
	free(bucket);
	bucket = NULL;
	return 0;
}
