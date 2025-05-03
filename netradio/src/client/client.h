#ifndef CLIENT_H__
#define CLIENT_H__
#define DEFAULT_PLAYER "mpg123"
// 默认配置
struct client_conf_st {
	char *mgroup; // 默认多播组
	char *mport; // 端口
	char *cmd; // 解码器
};

extern struct client_conf_st client_conf;
#endif
