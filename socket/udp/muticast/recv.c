#include <assert.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include "proto.h"

int main()
{
	int sd;
	struct sockaddr_in addrin;
	struct sockaddr_in addrrv;
	struct msg_st *msg;
	char ip[16];
	sd = socket(AF_INET, SOCK_DGRAM, 0);
	assert(sd > 0);

	// 加入一个多播组
	struct ip_mreq mreq;
	mreq.imr_multiaddr.s_addr = inet_addr("224.0.0.0");
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);

	setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));

	// bind local address
	// specify socket domain which defines the addressing schema.
	addrin.sin_family = AF_INET;
	addrin.sin_port = htons(4517);
	// convert dotted decimal to machine number.
	inet_pton(AF_INET, "0.0.0.0", &addrin.sin_addr);

	if (bind(sd, (void *)&addrin, sizeof(addrin)) < 0) {
		perror("bind");
		exit(1);
	}

	// loop listening
	msg = malloc(sizeof(struct msg_st) + NAMEMAXSIZE);
	socklen_t addrrv_len = sizeof(addrrv);
	while (1) {
		recvfrom(sd, msg, sizeof(struct msg_st) + NAMEMAXSIZE, 0,
			 (void *)&addrrv, &addrrv_len);
		inet_ntop(AF_INET, &addrrv.sin_addr, ip, 16);
		printf("%s:%d\n", ip, ntohs(addrrv.sin_port));
		printf("chinese:%d\n", ntohl(msg->chinese));
		printf("math:%d\n", ntohl(msg->math));
	}

	free(msg);
	close(sd);
	exit(0);
}
