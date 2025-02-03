/*
 * 每个线程从同一个文件中读取一个数字加一，然后写回
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#define THDNUM 20
#define FILENAME "./lockfile.txt"
static pthread_mutex_t rwmutex = PTHREAD_MUTEX_INITIALIZER;

static void *rw(void *p)
{
	FILE *fp;
	char *bufline = NULL;
	size_t liensize;
	fp = fopen(FILENAME, "r+");
	if (fp == NULL) {
		perror("fopen");
		pthread_exit(NULL);
	}

	// 临界区
	pthread_mutex_lock(&rwmutex);
	getline(&bufline, &liensize, fp);
	fseek(fp, 0, SEEK_SET);
	fprintf(fp, "%d\n", atoi(bufline) + 1);
	pthread_mutex_unlock(&rwmutex);

	free(bufline);
	fclose(fp);
	pthread_exit(NULL);
}

int main()
{
	pthread_t tid;
	int i = 0;

	for (i = 0; i < THDNUM; ++i) {
		int err = pthread_create(&tid, NULL, rw, NULL);
		if (err < 0) {
			fprintf(stderr, "%s", strerror(err));
			break;
		}
	}

	for (int j = 0; j < i; ++j) {
		int err = pthread_join(tid, NULL);
		if (err < 0) {
			fprintf(stderr, "%s", strerror(err));
		}
	}

	pthread_mutex_destroy(&rwmutex);

	exit(0);
}
