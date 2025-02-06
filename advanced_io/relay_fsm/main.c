#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>

#include "relay.h"
static int fd1, fd2;
static void close_fd()
{
	close(fd2);
	close(fd1);
}

int main()
{
	struct relay_jobstat_st *stat;
	stat = malloc(sizeof(struct relay_jobstat_st));

	atexit(close_fd);

	fd1 = open("/dev/tty11", O_RDWR);
	if (fd1 < 0) {
		perror("open");
		exit(1);
	}
	fd2 = open("/dev/tty12", O_RDWR | O_NONBLOCK);
	if (fd2 < 0) {
		perror("open");
		exit(1);
	}
	write(fd1, "TTY11\n", 5);
	write(fd2, "TTY12\n", 5);

	int job = relay_add(fd1, fd2);
	if (job < 0) {
		fprintf(stderr, "%s", strerror(job));
		exit(1);
	}

	do {
		relay_stat(job, stat);
		printf("stat is %d and count is %d\n", stat->state,
		       stat->count);
		sleep(5);
	} while (stat->state == 1 && stat->count < 5);

	relay_cancel(job);
	relay_stat(job, stat);
	printf("stat is %d and count is %d\n", stat->state, stat->count);

	relay_wait(job);
	int ret = relay_stat(job, stat);
	if (ret < 0)
		printf("job is over\n");

	exit(0);
}
