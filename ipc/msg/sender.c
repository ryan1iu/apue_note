#include <errno.h>
#include <assert.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include "proto.h"

int main()
{
	int ret;
	struct transfer_st msgp;
	int msg_id;
	int key;
	// 打开消息队列
	key = ftok(KEYPATH, KEYPROJ);
	msg_id = msgget(key, 0600);
	if (msg_id < 0) {
		perror("msgget()");
		exit(1);
	}

	strncpy(msgp.name, "afdfdalf", NAMESIZE);
	// 发送消息
	for (int i = 0; i < 3; i++) {
		msgp.math = rand() % 101;
		msgp.english = rand() % 101;
		do {
			ret = msgsnd(msg_id, &msgp, sizeof(msgp) - sizeof(long),
				     0);
			if (ret < 0 && errno != EINTR) {
				perror("msgsnd()");
				exit(1);
			}
		} while (ret < 0);
	}

	exit(0);
}
