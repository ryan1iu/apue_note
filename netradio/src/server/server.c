#include <arpa/inet.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <stdio.h>
#include <getopt.h>
#include <stdlib.h>
#include <netinet/ip.h>
#include "server_conf.h"
#include "../include/proto.h"
#include "thr_list.h"
#include "thr_channel.h"
#include "medialib.h"
#include <string.h>
#include <sys/syslog.h>
#include <unistd.h>

struct sockaddr_in sndaddr;
int serversd;
struct server_conf_st server_conf = { .rcvport = MPROT,
				      .media_dir = DEFAULT_MEDIADIR,
				      .runmode = RUN_DAEMON,
				      .mgroup = MGROUP };

static struct mlib_listentry_st *list;

static void print_help()
{
	printf("-P    specify receive port\n");
	printf("-M    specify multicast group\n");
	printf("-F    specify foreground runmode \n");
	printf("-D    specify medialib location \n");
	printf("-H    show help\n");
}
static void exit_handler(int s)
{
	closelog();
}
static void dameonize()
{
	// 打开系统日志
	openlog("NETMEDIA", LOG_PID | LOG_PERROR,
		LOG_DAEMON); // 记录调用者Pid和stderr

	int pid = fork();
	if (pid < 0) {
		syslog(LOG_ERR, "fork:%s", strerror(errno));
	}
	if (pid == 0) {
		// 输入输出重定向
		int fd = open("/dev/null", O_RDWR | O_APPEND);
		if (fd < 0) {
			syslog(LOG_WARNING, "open:%s", strerror(errno));
		}
		dup2(fd, 0);
		dup2(fd, 1);
		if (fd >= 2)
			close(fd);

		// 脱离控制终端
		setsid();

		// 更改工作路径
		chdir("/");
	}
	exit(0); // 父进程正常终止
}

static void socket_init()
{
	serversd = socket(AF_INET, SOCK_STREAM, 0);
	if (serversd < 0) {
		syslog(LOG_ERR, "socket");
		exit(1);
	}
	sndaddr.sin_family = AF_INET;
	sndaddr.sin_port = htons(atoi(server_conf.rcvport));
	inet_pton(AF_INET, server_conf.mgroup, &sndaddr.sin_addr);
}

int main(int argc, char **argv)
{
	// 设置进程异常终止时的资源回收函数
	sigset_t set;
	sigemptyset(&set);
	sigaddset(&set, SIGINT);
	sigaddset(&set, SIGTERM);
	sigaddset(&set, SIGQUIT);

	struct sigaction action;
	action.sa_mask = set;
	action.sa_flags = 0;
	action.sa_handler = exit_handler;

	// 使用sigaction防止信号处理函数嵌套导致资源回收函数被重复调用导致错误
	sigaction(SIGINT, &action, NULL);
	sigaction(SIGTERM, &action, NULL);
	sigaction(SIGQUIT, &action, NULL);

	char c;
	while (1) {
		c = getopt(argc, argv, "M:P:FD:I:H");
		printf("get command c:%c\n", c);
		if (c < 0) {
			break;
		}
		switch (c) {
		case 'M':
			server_conf.mgroup = optarg;
			break;
		case 'P':
			server_conf.rcvport = optarg;
			break;
		case 'F':
			server_conf.runmode = RUN_FOREGROUND;
			break;
		case 'D':
			server_conf.media_dir = optarg;
			break;
		case 'H':
			print_help();
			exit(0);
			break;
		default:
			abort();
			break;
		}
	}

	// 如果指定后台运行
	if (server_conf.runmode == RUN_DAEMON) {
		dameonize();
	}

	// socket初始化
	socket_init();

	// 获取节目单信息
	struct chnldesc_st *list;
	int list_size;
	getchnl_list(&list, &list_size);
	// 创建节目单线程
	thr_list_crt(list, list_size);

	// 创建频道线程
	for (int i = 0; i < list_size; i++) {
		thr_chnl_crt(list + i);
	}
	exit(0);
}
