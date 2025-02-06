#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>

#define BUFSIZE 512
enum {
	STATE_READ = 1, // 读
	STATE_WRITE, // 写
	STATE_EX, // 异常
	STATE_TERM, // 终止
};
struct fsm_st {
	int state;
	int source;
	int dest;
	char buf[BUFSIZE];
	int read_len;
	int write_pos;
	char *errmsg;
};

void fsm_driver(struct fsm_st *fsm)
{
	size_t ret;
	switch (fsm->state) {
	case STATE_READ:
		fsm->read_len = read(fsm->source, fsm->buf, BUFSIZE);
		if (fsm->read_len < 0) {
			// 如果不是假错
			if (errno != EAGAIN) {
				fsm->errmsg = "read failed";
				fsm->state = STATE_EX;
			}
			// 如果是假错，状态不变，重新读
		} else if (fsm->read_len > 0) {
			fsm->state = STATE_WRITE;
		} else {
			// 说明已经读完
			fsm->state = STATE_TERM;
		}
		break;
	case STATE_WRITE:
		// 写坚持
		ret = write(fsm->dest, fsm->buf + fsm->write_pos,
			    fsm->read_len);
		if (ret < 0) {
			if (errno != EAGAIN) {
				fsm->errmsg = "write failed";
				fsm->state = STATE_EX;
			}
		} else {
			fsm->read_len -= ret;
			fsm->write_pos += ret;
			if (fsm->read_len == 0) {
				fsm->state = STATE_READ;
			}
		}
		break;
	case STATE_EX:
		perror(fsm->errmsg);
		fsm->state = STATE_TERM;
		break;
	case STATE_TERM:
		/* do something */
		break;
	default:
		abort();
		break;
	}
}

static void copy_fsm(int fd1, int fd2)
{
	struct fsm_st fsm1, fsm2;
	int epfd; // 是一个文件描述符

	// 确保文件以非阻塞方式打开
	int oldflag1 = fcntl(fd1, F_GETFL);
	fcntl(fd1, F_SETFL, oldflag1 | O_NONBLOCK);

	int oldflag2 = fcntl(fd2, F_GETFL);
	fcntl(fd2, F_SETFL, oldflag2 | O_NONBLOCK);

	// 初始化状态机
	fsm1.source = fd1;
	fsm1.dest = fd2;
	fsm1.read_len = 0;
	fsm1.write_pos = 0;
	fsm1.state = STATE_READ;

	fsm2.source = fd2;
	fsm2.dest = fd1;
	fsm2.read_len = 0;
	fsm2.write_pos = 0;
	fsm2.state = STATE_READ;

	// 创建epoll实例
	epfd = epoll_create(10);
	if (epfd < 0) {
		perror("epoll");
		exit(1);
	}

	struct epoll_event event;
	event.events = 0;
	event.data.fd = fd1;
	epoll_ctl(epfd, EPOLL_CTL_ADD, fd1, &event);
	event.events = 0;
	event.data.fd = fd2;
	epoll_ctl(epfd, EPOLL_CTL_ADD, fd2, &event);

	while (fsm1.state != STATE_TERM && fsm2.state != STATE_TERM) {
		// 如果是异常态跳过监视
		if (fsm1.state == STATE_EX || fsm2.state == STATE_EX) {
			fsm_driver(&fsm1);
			fsm_driver(&fsm2);
			continue;
		}

		// 布置监视任务
		event.data.fd = fd1;
		event.events = 0;
		if (fsm1.state == STATE_READ) {
			event.events |= EPOLLIN;
		}
		if (fsm2.state == STATE_WRITE) {
			event.events |= EPOLLOUT;
		}
		epoll_ctl(epfd, EPOLL_CTL_MOD, fd1, &event);

		if (fsm1.state == STATE_WRITE) {
			event.events |= EPOLLOUT;
		}
		if (fsm2.state == STATE_READ) {
			event.events |= EPOLLIN;
		}
		epoll_ctl(epfd, EPOLL_CTL_MOD, fd2, &event);

		// 以阻塞方式监视
		int ret = 0;
		do {
			ret = epoll_wait(epfd, &event, 1, -1);
			// 如果不是被信号中断
			if (ret < 0) {
				if (errno != EINTR) {
					perror("epoll_wait");
					exit(1);
				}
			}
		} while (ret < 0);

		// 查看监视结果
		if (event.data.fd == fd1 && event.events & EPOLLIN ||
		    event.data.fd == fd1 && event.events & EPOLLOUT) {
			fsm_driver(&fsm1);
		}
		if (event.data.fd == fd2 && event.events & EPOLLIN ||
		    event.data.fd == fd2 && event.events & EPOLLOUT) {
			fsm_driver(&fsm2);
		}
	}

	// 恢复文件选项
	fcntl(fd1, F_SETFL, oldflag1);
	fcntl(fd2, F_SETFL, oldflag2);

	close(epfd); // !!!
}
int main()
{
	int fd1, fd2;
	fd1 = open("/dev/tty11", O_RDWR);
	if (fd1 < 0) {
		perror("open");
		exit(1);
	}
	fd2 = open("/dev/tty12", O_RDWR | O_NONBLOCK);
	if (fd2 < 0) {
		perror("open");
		close(fd1);
		exit(1);
	}
	write(fd1, "tty1\n", 5);
	write(fd2, "tty2\n", 5);

	copy_fsm(fd1, fd2);

	close(fd2);
	close(fd1);

	exit(0);
}
