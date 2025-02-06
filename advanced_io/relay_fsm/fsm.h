#ifndef FSM_H__
#define FSM_H__

#define BUFSIZE 512
enum {
	FSM_STATE_READ = 1, // 读
	FSM_STATE_WRITE, // 写
	FSM_STATE_EX, // 异常
	FSM_STATE_TERM, // 终止
};
struct fsm_st {
	int state;
	int source;
	int dest;
	char buf[BUFSIZE];
	int read_len;
	int write_pos;
	char *errmsg;
	int count; // 处理的字节数
};
#endif
