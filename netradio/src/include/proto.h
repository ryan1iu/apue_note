#ifndef PROTO_H__
#define PROTO_H__
#include "types.h"

// 多播组地址和端口
#define MGROUP "224.2.2.2"
#define MPROT "4517"

#define CHNL_NUM 100 // 频道数量
#define MIN_CHNL_NO 1 // 最小频道号
#define MAX_CHNL_NO (CHNL_NUM + MIN_CHNL_NO) // 最大频道号

// 节目单线程特殊频道号
#define CHNL_LIST_NO 0

// udp包中的数据最大限制
#define MAX_UDP_DATA (65535 - 20 - 8)
// 频道数据包的变长数组的长度限制
#define MAX_CHNL_DATA (MAX_UDP_DATA - sizeof(uint8_t))

// 数据频道传输数据协议
struct chnl_data_st {
	chnid_t chnid; // must between MIN_CHNL_NO - MAX_CHNL_NO
	uint8_t data[]; // 变长结构体
} __attribute__((packed));

// 节目单数据传输协议
#define MAX_CHNL_LIST_DATA (MAX_UDP_DATA - sizeof(uint8_t))
// chnid desc
struct chnl_desc_entry_st {
	chnid_t chnid;
	uint16_t len;
	char desc[];
} __attribute__((packed));

struct chnl_list_st {
	chnid_t chnid; // must equal 0
	struct chnl_desc_entry_st list[];
} __attribute__((packed));

#endif
