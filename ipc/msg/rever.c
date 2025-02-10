/* 被动方先启动 */
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
	key_t key;
	// 创建并打开消息队列
	key = ftok(KEYPATH, KEYPROJ);
	assert(key > 0);
	msg_id = msgget(key, IPC_CREAT | 0600);
	assert(msg_id > 0);

	// 接收消息
	while (1) {
		ret = msgrcv(msg_id, &msgp, sizeof(msgp) - sizeof(long), 0, 0);
		if (ret < 0) {
			if (errno != EINTR) {
				perror("msgrcv()");
				break;
			}
		}
		printf("name: %s, math: %d, english: %d\n", msgp.name,
		       msgp.math, msgp.english);
	}

	// 关闭消息队列
	msgctl(msg_id, IPC_RMID, NULL);

	exit(0);
}
