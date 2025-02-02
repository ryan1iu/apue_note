#include "bucket.h"
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

struct bucket_st {
	int volatile token; // 持有token数
	int volatile rate; // 每秒钟增加的token数
	int volatile size; // 持有token值上限
	int index; // 存储桶下标
};

static struct bucket_st *array[BUCKET_MAX_SIZE];
static int load_flag;
static struct sigaction old_act;
static struct itimerval old_timer;

static int find_low_index()
{
	int i;
	for (i = 0; i < BUCKET_MAX_SIZE; i++) {
		if (array[i] == NULL) {
			break;
		}
	}
	if (i < BUCKET_MAX_SIZE) {
		return i;
	} else {
		return -1;
	}
}

void alrm_handler(int sig_num, siginfo_t *act, void *p)
{
	if (act->si_code != SI_KERNEL) {
		printf("not kernel\n");
		return; // 如果信号不是内核发出的，忽略
	}
	for (int i = 0; i < BUCKET_MAX_SIZE; i++) {
		if (array[i] != NULL) {
			int tokenadd = array[i]->token + array[i]->rate;
			array[i]->token = tokenadd > array[i]->size ?
						  array[i]->size :
						  tokenadd;
		}
	}
}

void unload_module()
{
	// 恢复定时器
	setitimer(ITIMER_REAL, &old_timer, NULL);
	for (int i = 0; i < BUCKET_MAX_SIZE; i++) {
		if (array[i] != NULL) {
			free(array[i]);
		}
	}
	// 恢复默认处理函数
	sigaction(SIGALRM, &old_act, NULL);
}

void load_module()
{
	// 保存初始状态
	// 设置alarm处理函数
	struct sigaction act;
	sigset_t set;
	sigemptyset(&set);
	act.sa_sigaction = alrm_handler;
	act.sa_mask = set;
	act.sa_flags =
		SA_SIGINFO; // 设置为SA_SIGINFO标明使用sa_sigaction指定的处理函数
	sigaction(SIGALRM, &act, &old_act);
	atexit(unload_module);

	// 使用setitimer设置周期定时器
	struct itimerval timer;
	timer.it_value.tv_sec = 1;
	timer.it_value.tv_usec = 0;
	timer.it_interval.tv_sec = 1;
	timer.it_interval.tv_usec = 0;

	setitimer(ITIMER_REAL, &timer, &old_timer);
}

int bucket_init(bucket_t **bucket, int rate, int size)
{
	if (!load_flag) {
		load_module();
		load_flag = 1;
	}
	int low_index;
	struct bucket_st *buk;
	low_index = find_low_index();
	if (low_index == -1) {
		return -1;
	}
	buk = malloc(sizeof(struct bucket_st));
	if (buk == NULL) {
		return -ENOMEM;
	}
	array[low_index] = buk;
	buk->token = 0;
	buk->rate = rate;
	buk->size = size;
	buk->index = low_index;
	*bucket = buk;
	return 0;
}

int bucket_fetch(bucket_t *bucket, int token)
{
	struct bucket_st *bk = bucket;
	if (token < 0 || token > bk->size) {
		return -EINVAL;
	}
	while (bk->token < token) {
		pause();
	}
	bk->token -= token;
	return token;
}

int bucket_return(bucket_t *bucket, int token)
{
	if (token < 0) {
		return -EINVAL;
	}
	struct bucket_st *bk = bucket;
	int tokenadd = token + bk->token;
	if (tokenadd > bk->size)
		tokenadd = bk->size;
	bk->token = tokenadd;
	return 0;
}

int bucket_destory(bucket_t *bucket)
{
	struct bucket_st *bk = bucket;
	array[bk->index] = NULL;
	free(bucket);
	bucket = NULL;
	return 0;
}
