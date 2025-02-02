/* 建立守护进程
 * 输入输出重定向
 * 脱离控制终端
 */

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <sys/syslog.h>
#include <unistd.h>
#include <syslog.h>
#include <errno.h>
#include <signal.h>

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
	exit(0);
}

void exit_handler(int sig)
{
	// fclose(fp);
	closelog();
	exit(0); // 将异常终止转换为正常终止
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

	while (1) {
		fprintf(fp, "%d\n", 1);
		fflush(fp);
		sleep(1);
	}

	// 守护进程只能被信号异常杀死，因此下面的释放操作不会正确执行
	// fclose(fp);
	// closelog();

	// 一个容易想到的解决办法是拦截会将进程杀死的信号，将释放资源操作封装成一个信号处理函数，例如下面这样
	// signal(SIGINT, exit_handler);
	// signal(SIGTERM, exit_handler);
	// 但是会存在一个问题，如果在一个信号处理函数执行过程中，另一个信号到来了，信号处理函数会嵌套执行，导致资源被重复释放导致发生重入错误
	// 因此，sigaction函数用来解决这个问题，sigaction函数在执行过程中可以屏蔽掉指定的信号，从而避免嵌套问题

	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	sigaddset(&set, SIGTERM);

	struct sigaction action;
	action.sa_mask = set;
	action.sa_flags = 0;
	action.sa_handler = exit_handler;

	sigaction(SIGINT, &action, NULL);
	sigaction(SIGTERM, &action, NULL);

	exit(0);
}
