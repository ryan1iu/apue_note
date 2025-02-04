#ifndef MYSM_H__
#define MYSM_H__

typedef void sm_t;

// 初始化一个信号量
int sm_init(sm_t **, int);

int sm_wait(sm_t *, int);

int sm_signal(sm_t *, int);

// 销毁一个信号量
int sm_destory(sm_t *);

#endif
