#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include "relay.h"
#include "fsm.h"

// 隐藏结构体内部
struct relay_job_st {
	int stat; // 任务状态
	int fd1, oldflag1;
	int fd2, oldflag2;
	struct fsm_st fsm;
};

static struct relay_job_st *relay_job[JOB_MAX]; // 全局任务数组
static pthread_mutex_t job_mutex = PTHREAD_MUTEX_INITIALIZER; // 任务数组互斥量
static pthread_cond_t job_cond = PTHREAD_COND_INITIALIZER; // 任务数组条件变量
static pthread_once_t once_flag = PTHREAD_ONCE_INIT; // 单次初始化变量

static pthread_t tid; // 状态机推动线程

static int find_index_unlocked()
{
	int i = 0;
	for (i = 0; i < JOB_MAX; ++i) {
		if (relay_job[i] == NULL)
			break;
	}
	if (i == JOB_MAX)
		return -ENOSPC;
	else
		return i;
}

static void fsm_driver(struct fsm_st *fsm)
{
	size_t ret;
	switch (fsm->state) {
	case FSM_STATE_READ:
		fsm->read_len = read(fsm->source, fsm->buf, BUFSIZE);
		if (fsm->read_len < 0) {
			// 如果不是假错
			if (errno != EAGAIN) {
				fsm->errmsg = "read failed";
				fsm->state = FSM_STATE_EX;
			}
			// 如果是假错，状态不变，重新读
		} else if (fsm->read_len > 0) {
			fsm->state = FSM_STATE_WRITE;
		} else {
			// 说明已经读完
			fsm->state = FSM_STATE_TERM;
		}
		break;
	case FSM_STATE_WRITE:
		// 写坚持
		ret = write(fsm->dest, fsm->buf + fsm->write_pos,
			    fsm->read_len);
		if (ret < 0) {
			if (errno != EAGAIN) {
				fsm->errmsg = "write failed";
				fsm->state = FSM_STATE_EX;
			}
		} else {
			fsm->read_len -= ret;
			fsm->count += ret;
			fsm->write_pos += ret;
			if (fsm->read_len == 0) {
				fsm->state = FSM_STATE_READ;
			}
		}
		break;
	case FSM_STATE_EX:
		perror(fsm->errmsg);
		fsm->state = FSM_STATE_TERM;
		break;
	case FSM_STATE_TERM:
		/* do something */
		break;
	default:
		abort();
		break;
	}
}
static void *relay_fsm(void *p)
{
	while (1) {
		pthread_testcancel(); // ！！！设置一个取消点
		for (int i = 0; i < JOB_MAX; i++) {
			pthread_mutex_lock(&job_mutex);
			if (relay_job[i] != NULL &&
			    relay_job[i]->stat != OVER) {
				// 如果任务没有被取消并且状态机没有终止
				if (relay_job[i]->stat != CANCELED &&
				    relay_job[i]->fsm.state != FSM_STATE_TERM) {
					// 状态驱动
					fsm_driver(&relay_job[i]->fsm);
				} else {
					relay_job[i]->stat = OVER;
					// 通知wait函数
					pthread_cond_signal(&job_cond);
				}
			}
			pthread_mutex_unlock(&job_mutex);
		}
	}

	pthread_exit(NULL);
}

static void unload_module()
{
	// 销毁互斥量
	pthread_mutex_destroy(&job_mutex);

	// 结束状态机线程
	if (pthread_cancel(tid) < 0) {
		fprintf(stderr, "cancel failed");
	}
	pthread_join(tid, NULL);
}

static void load_module(void)
{
	atexit(unload_module);

	int ret = pthread_create(&tid, NULL, relay_fsm, NULL);
	if (ret < 0) {
		perror("pthread_create");
		exit(1);
	}
}

int relay_add(int fd1, int fd2)
{
	int job_id;

	pthread_once(&once_flag, load_module);

	struct relay_job_st *job = malloc(sizeof(struct relay_job_st));
	if (job == NULL) {
		return -ENOMEM;
	}

	pthread_mutex_lock(&job_mutex);
	job_id = find_index_unlocked();
	if (job_id < 0) {
		pthread_mutex_unlock(&job_mutex);
		return job_id;
	}
	relay_job[job_id] = job;
	pthread_mutex_unlock(&job_mutex);
	job->fd1 = fd1;
	job->fd2 = fd2;
	job->stat = RUNNING;
	// 确保文件以非阻塞方式打开
	int oldflag1 = fcntl(fd1, F_GETFL);
	fcntl(fd1, F_SETFL, oldflag1 | O_NONBLOCK);

	int oldflag2 = fcntl(fd2, F_GETFL);
	fcntl(fd2, F_SETFL, oldflag2 | O_NONBLOCK);
	job->oldflag1 = oldflag1;
	job->oldflag2 = oldflag2;

	// 初始化状态机
	job->fsm.source = fd1;
	job->fsm.dest = fd2;
	job->fsm.read_len = 0;
	job->fsm.write_pos = 0;
	job->fsm.state = FSM_STATE_READ;

	return job_id;
}

int relay_cancel(int job)
{
	pthread_mutex_lock(&job_mutex);
	if (relay_job[job] == NULL) {
		pthread_mutex_unlock(&job_mutex);
		return -1;
	}
	relay_job[job]->stat = CANCELED;
	pthread_mutex_unlock(&job_mutex);

	return 0;
}

// 以阻塞模式等待指定任务结束
int relay_wait(int job)
{
	struct relay_job_st *p;
	pthread_mutex_lock(&job_mutex);
	if (relay_job[job] == NULL) {
		pthread_mutex_unlock(&job_mutex);
		return -1;
	}

	// 阻塞等待
	while (relay_job[job]->stat != OVER) {
		pthread_cond_wait(&job_cond, &job_mutex);
	}

	// 恢复文件选项
	fcntl(relay_job[job]->fd1, F_SETFL, relay_job[job]->oldflag1);
	fcntl(relay_job[job]->fd2, F_SETFL, relay_job[job]->oldflag2);

	p = relay_job[job];
	relay_job[job] = NULL;

	pthread_mutex_unlock(&job_mutex);

	// 释放内存
	free(p);
	return 0;
}

int relay_stat(int job, struct relay_jobstat_st *stat)
{
	pthread_mutex_lock(&job_mutex);
	if (relay_job[job] == NULL) {
		pthread_mutex_unlock(&job_mutex);
		return -1;
	}
	stat->fd1 = relay_job[job]->fd1;
	stat->fd2 = relay_job[job]->fd2;
	stat->state = relay_job[job]->stat;
	stat->count = relay_job[job]->fsm.count;
	pthread_mutex_unlock(&job_mutex);

	return 0;
}
