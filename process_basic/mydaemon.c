/* 建立守护进程
 * 输入输出重定向
 * 脱离控制终端
 */

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <syslog.h>
#include <errno.h>

static int daemonize()
{
	pid_t pid;
	int fd;
	pid = fork();
	if (pid < 0) {
		perror("fork()");
		return -1;
	}
	if (pid == 0) {
		// 输入输出重定向
		fd = open("/dev/null", O_RDWR | O_APPEND);
		if (fd < 0) {
			perror("open()");
			return -1;
		}
		dup2(fd, 0);
		dup2(fd, 1);
		close(fd);

		/* 脱离控制终端 */
		setsid();

		/* 更改工作路径 , 防止设备占用*/
		chdir("/");
		return 0;
	}
	exit(0); // 父进程正常终止
}
int main()
{
	int rs;
	FILE *fp;
	rs = daemonize();
	openlog(NULL, LOG_PID, LOG_DAEMON);
	if (rs < 0) {
		// fprintf(stderr, "daemonize failed");
		syslog(LOG_ERR, "daemonize failed");
		exit(1);
	}

	syslog(LOG_INFO, "daemonize succeed");
	fp = fopen("/tmp/out", "a+");
	if (fp == NULL) {
		// fprintf(stderr, "fopen failed");
		syslog(LOG_ERR, "fopen failed: %s", strerror(errno));
		exit(1);
	}

	for (int i = 0; i < 100; i++) {
		fprintf(fp, "%d\n", i);
		fflush(fp);
		sleep(1);
	}

	fclose(fp);
	closelog();

	exit(0);
}
