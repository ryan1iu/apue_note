#include <assert.h>
#include <string.h>
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
	int flag_broadcast;
	struct sockaddr_in addr;
	struct msg_st *msg;
	int sd;

	sd = socket(AF_INET, SOCK_DGRAM, 0);
	assert(sd > 0);

	// enable the SO_BROADCAST flag
	flag_broadcast = 1;
	setsockopt(sd, SOL_SOCKET, SO_BROADCAST, &flag_broadcast,
		   sizeof(flag_broadcast));

	addr.sin_family = AF_INET;
	addr.sin_port = htons(4517);

	// set broadcast address
	inet_pton(AF_INET, "255.255.255.255", &addr.sin_addr);

	msg = malloc(sizeof(struct msg_st) + NAMEMAXSIZE);
	msg->chinese = htonl(56);
	msg->math = htonl(68);
	strcpy(msg->name, "liming");

	sendto(sd, msg, sizeof(struct msg_st) + NAMEMAXSIZE, 0, (void *)&addr,
	       sizeof(addr));

	puts("ok");
	free(msg);
	close(sd);

	exit(0);
}
