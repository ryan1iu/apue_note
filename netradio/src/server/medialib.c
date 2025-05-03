#include "medialib.h"
#include "bucket.h"
#include "../include/proto.h"
#include "server_conf.h"
#include <fcntl.h>
#include <glob.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/syslog.h>
#include <unistd.h>
// 隐藏频道结构体
struct chnl_descall_st {
	chnid_t chnid;
	char *desc;
	int fd; // 当前音乐文件的描述符
	int seek; // 偏移量
	bucket_t *buk; // 令牌桶
};
static struct chnl_descall_st channel[CHNL_NUM + 1];
static int list_size;
void media_init()
{
	char path[CHNL_NUM + 1];
	snprintf(path, CHNL_NUM + 1, "%s/*", server_conf.media_dir);
	glob_t globres;
	glob(path, GLOB_PERIOD, NULL, &globres);
	int globc = globres.gl_pathc;
	for (int i = 0; i < globc; i++) {
		char *descfile = strdup(globres.gl_pathv[i]);
		strcat(descfile, "/desc.txt");

		// 读取频道描述
		int descfd = open(descfile, O_RDONLY);
		char desc[512];
		struct stat statres;
		if (fstat(descfd, &statres) < 0) {
			syslog(LOG_ERR, "fstat() failed\n");
			exit(1);
		}
		// 获取文件大小
		int descsize = statres.st_size;

		// 建立对应大小的缓冲区
		char *buffer = malloc(descsize);
		read(descfd, buffer, descsize);

		char *musicfile = strdup(globres.gl_pathv[i]);
		strcat(musicfile, "/desc.txt");

		// 初始化令牌桶
		bucket_t *bucket;
		bucket_init(&bucket, RATE, SIZE);

		channel[i].fd = open(musicfile, O_RDONLY);
		channel[i].desc = buffer;
		channel[i].buk = bucket;
		channel[i].chnid = i + 1;
		channel[i].seek = 0;

		free(descfile);
		free(musicfile);
	}
}

void getchnl_list(struct chnldesc_st **list, int *count)
{
	list = malloc(sizeof(struct chnldesc_st) * CHNL_NUM + 1);
	*count = list_size;
	for (int i = 0; i < list_size; i++) {
		list[i]->chnid = channel[i].chnid;
		list[i]->desc = strdup(channel[i].desc);
	}
}
