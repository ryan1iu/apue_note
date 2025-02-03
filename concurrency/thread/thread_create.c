#include <stdlib.h>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

static void cleanfunc(void *p)
{
	puts(p);
}

static void *func(void *p)
{
	puts("func running");
	// push和pop是用宏实现的，必须成对出现，否则会大括号不匹配
	pthread_cleanup_push(cleanfunc, "clean1");
	pthread_cleanup_push(cleanfunc, "clean2");
	pthread_cleanup_push(cleanfunc, "clean3");
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);
	pthread_cleanup_pop(1);
	// return NULL;
	pthread_exit(NULL);
}

int main()
{
	pthread_t pid;
	puts("begin");

	int err = pthread_create(&pid, NULL, func, NULL);
	if (err < 0) {
		fprintf(stderr, "%s", strerror(err));
		exit(1);
	}

	// sleep(1); // 等待一下调度器调度func线程
	pthread_join(pid, NULL);

	puts("end");

	exit(0);
}
