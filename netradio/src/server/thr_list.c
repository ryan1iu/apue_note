#include "thr_list.h"
#include <netinet/in.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <syslog.h>
#include <errno.h>
#include "../include/proto.h"
#include "server_conf.h"
#include <unistd.h>

static pthread_t list_tid;
static int list_size;
static struct chnldesc_st *list_entry; // 频道列表

void *thr_list(void *p)
{
	int totalsize;
	struct chnl_list_st *entrylistptr;
	struct chnl_desc_entry_st *entryptr;
	int ret;
	int size;

	totalsize = sizeof(chnid_t); // 之后逐步累计
	for (int i = 0; i < list_size; ++i) {
		totalsize += sizeof(struct chnl_desc_entry_st) +
			     strlen(list_entry[i].desc);
	}
	entrylistptr = malloc(totalsize);
	if (entrylistptr == NULL) {
		syslog(LOG_ERR, "malloc():%s", strerror(errno));
		exit(1);
	}
	entrylistptr->chnid = 0; // 这是列表频道

	entryptr = entrylistptr->list;
	for (int i = 0; i < list_size; ++i) {
		size = sizeof(struct chnl_desc_entry_st) +
		       strlen(list_entry[i].desc);

		entryptr->chnid = list_entry[i].chnid;
		entryptr->len = htons(size);
		strcpy(entryptr->desc, list_entry[i].desc);
		entryptr =
			(void *)(((char *)entryptr) + size); // 向后移动entptr
	}

	// 发送到多播组，持续发送，每秒钟一次
	while (1) {
		sendto(serversd, entrylistptr, totalsize, 0, (void *)&sndaddr,
		       sizeof(sndaddr));
		sleep(1);
	}
}
void thr_list_crt(struct chnldesc_st *list, int count)
{
	int err;
	list_entry = list;
	list_size = count;
	err = pthread_create(&list_tid, NULL, thr_list, NULL);
	if (err) {
		syslog(LOG_ERR, "pthread_create():%s", strerror(errno));
	}
}
void thr_list_destory()
{
	pthread_cancel(list_tid);
	pthread_join(list_tid, NULL);
}
