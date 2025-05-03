// 频道模块
#ifndef THR_CHANNEL_H__
#define THR_CHANNEL_H__
#include "medialib.h"
/*
 * 创建频道线程
 * chnl : 指定频道
 */
void thr_chnl_crt(struct chnldesc_st *chnl);

void thr_chnl_destory(struct chnldesc_st *chnl);

#endif
