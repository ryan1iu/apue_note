// 节目单模块
#ifndef THR_LIST_H__
#define THR_LIST_H__
#include "medialib.h"

/* 
 * list : 节目单信息结构体指针
 * count: 节目数量
 */
void thr_list_crt(struct chnldesc_st *list, int count);
void thr_list_destory();

#endif
