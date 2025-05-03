// 媒体库模块
#ifndef MEDIALIB_H__
#define MEDIALIB_H__
#include "../include/types.h"

#define RATE 10
#define SIZE 100
struct chnldesc_st {
	chnid_t chnid;
	char *desc;
};

// 获取节目单
void getchnl_list(struct chnldesc_st **list, int *size);

// 释放节目单指针指向的内存空间  谁申请 谁释放
void freechnl_list(struct chnldesc_st *list);

void readchn(chnid_t chnid, void *buf, int size);

#endif //MEDIALIB_H__
