#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "mysm.h"

typedef struct {
	int count;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
} sm_st;

int sm_init(sm_t **sm, int n)
{
	if (n < 0) {
		return -EINVAL;
	}

	sm_st *sema = malloc(sizeof(sm_st));
	if (sema == NULL) {
		return -errno;
	}

	sema->count = n;
	pthread_mutex_init(&sema->mutex, NULL);
	pthread_cond_init(&sema->cond, NULL);

	return 0;
}

int sm_wait(sm_t *sm, int n)
{
	if (n < 0 || sm == NULL) {
		return -EINVAL;
	}

	sm_st *sema = sm;

	pthread_mutex_lock(&sema->mutex);
	while (sema->count < n) {
		pthread_cond_wait(&sema->cond, &sema->mutex);
	}
	sema->count -= n;
	pthread_mutex_unlock(&sema->mutex);

	return 0;
}

int sm_signal(sm_t *sm, int n)
{
	if (n < 0 || sm == NULL) {
		return -EINVAL;
	}

	sm_st *sema = sm;
	pthread_mutex_lock(&sema->mutex);
	sema->count += n;
	pthread_cond_broadcast(&sema->cond);
	pthread_mutex_unlock(&sema->mutex);
	return 0;
}

int sm_destory(sm_t *sm)
{
	sm_st *sema = sm;
	int ret;
	ret = pthread_mutex_destroy(&sema->mutex);
	if (ret < 0) {
		fprintf(stderr, "%s", strerror(ret));
	}
	ret = pthread_cond_destroy(&sema->cond);
	if (ret < 0) {
		fprintf(stderr, "%s", strerror(ret));
	}
	return 0;
}
