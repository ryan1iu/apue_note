#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <netinet/ip.h>
#include <stdlib.h>
#include <getopt.h>
#include "client.h"
#include "../include/proto.h"
#include <unistd.h>

struct client_conf_st client_conf = { .mgroup = MGROUP,
				      .mport = MPROT,
				      .cmd = DEFAULT_PLAYER };

static int writen(int fd, char *buf, int len);
static void print_help()
{
	printf("-P --port   specify receive port\n");
	printf("-M --mgroup specify multicast group\n");
	printf("-p --player specify player \n");
	printf("-H --help   show help\n");
}

int main(int argc, char **argv)
{
	struct option argarr[] = { { "port", 1, NULL, 'P' },
				   { "mgroup", 1, NULL, 'M' },
				   { "help", 0, NULL, 'H' },
				   { NULL, 0, NULL, 0 } };
	int c;
	int index = 0;
	while (1) {
		/*long format argument parse*/
		c = getopt_long(argc, argv, "P:M:p:H", argarr, &index);
		if (c < 0)
			break;
		switch (c) {
		case 'P':
			client_conf.mport = optarg;
			break;
		case 'M':
			client_conf.mgroup = optarg;
			break;
		case 'h':
			client_conf.cmd = optarg;
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

	int sd = socket(AF_INET, SOCK_DGRAM, 0);
	if (sd < 0) {
		perror("socket()");
		exit(1);
	}
	// 加入一个多播组
	struct ip_mreq mreq;
	inet_pton(AF_INET, client_conf.mgroup, &mreq.imr_multiaddr);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	if (setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) <
	    0) {
		perror("setsockopt()");
		exit(1);
	}
	// improve efficiency
	int multicast_loop = 1; // 1 表示启用回环，0 表示禁用
	if (setsockopt(sd, IPPROTO_IP, IP_MULTICAST_LOOP, &multicast_loop,
		       sizeof(multicast_loop)) < 0) {
		perror("setsockopt(IP_MULTICAST_LOOP)");
		close(sd);
		exit(1);
	}

	// 绑定ip和端口
	struct sockaddr_in laddr; // local address
	laddr.sin_family = AF_INET;
	laddr.sin_port = htons(atoi(client_conf.mport));
	inet_pton(AF_INET, "0.0.0.0", &laddr.sin_addr);
	if (bind(sd, (void *)&laddr, sizeof(laddr)) < 0) {
		perror("bind()");
		exit(1);
	}

	int pd[2];
	if (pipe(pd) < 0) {
		perror("pipe()");
		exit(1);
	}
	int pid = fork();

	if (pid == 0) { // 如果是子进程
		close(sd);
		close(pd[1]); // 关闭写端
		// 将标准输入重定向到读端
		dup2(pd[0], 0);
		if (pd[0] > 0)
			close(pd[0]);

		execlp(client_conf.cmd, client_conf.cmd, "-", NULL);
		perror("execlp()");
		exit(0);
	}

	close(pd[0]);

	// 父进程首先获取节目单信息
	struct sockaddr_in remoteaddr;
	socklen_t socklen;
	struct chnl_list_st *list = malloc(MAX_UDP_DATA);
	if (list == NULL) {
		perror("malloc()");
		close(pd[1]);
		close(sd);
		exit(1);
	}

	// 先拿到频道包才能进行下一步
	while (1) {
		int len = recvfrom(sd, list, MAX_UDP_DATA, 0,
				   (void *)&remoteaddr, &socklen);
		if (list->chnid == 0 && len > sizeof(struct chnl_list_st))
			break;
	}

	// 打印节目单
	struct chnl_desc_entry_st *pos;
	for (pos = list->list; (char *)pos < ((char *)list + sizeof(*list));
	     pos = (void *)((char *)pos) + ntohs(pos->len)) {
		printf("channel:%d%s", pos->chnid, pos->desc);
	}
	/*free list*/
	free(list);

	// 选择节目单
	int ret = 0, chosenid;
	while (ret < 1) {
		ret = scanf("%d", &chosenid);
		if (ret != 1)
			exit(1);
	}

	// 接收频道数据，转发给解码器
	struct chnl_data_st *data = malloc(MAX_UDP_DATA);
	if (data == NULL) {
		perror("malloc()");
		exit(1);
	}
	while (1) {
		int len = recvfrom(sd, data, MAX_UDP_DATA, 0,
				   (void *)&remoteaddr, &socklen);
		if (len <= sizeof(struct chnl_data_st))
			continue;
		if (writen(pd[1], (char *)data->data, len - sizeof(chnid_t)) <
		    0) {
			exit(1);
		}
	}
	free(data);
	close(sd);
	close(pd[1]);
	exit(0);
}

// 坚持写够len个字节
static int writen(int fd, char *buf, int len)
{
	int count = 0;
	int pos = 0;
	while (len > 0) {
		count = write(fd, buf + pos, len);
		if (count < 0) {
			if (errno == EINTR)
				continue;
			perror("write()");
			return -1;
		}
		len -= count;
		pos += count;
	}
	return 0;
}
