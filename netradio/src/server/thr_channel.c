#include "thr_channel.h"

void thr_chnl_crt(struct chnldesc_st *chnl)
{
	pthread_create(&thr_channel[tid_nextpos].tid, NULL, thr_channel_snder,
		       ptr);

	thr_channel[tid_nextpos].chnid = ptr->chnid; //填写频道信息
	tid_nextpos++;
}

void thr_chnl_destory(struct chnldesc_st *chnl);
