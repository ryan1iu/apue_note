在Unix网络编程中，广播（Broadcast）和多播（Multicast）是两种高效的一对多通信机制，适用于不同场景。以下是它们的核心概念和实现要点：

---

### **1. 广播（Broadcast）**

#### **定义**

- 广播将数据包发送到同一局域网（LAN）内的所有主机。
- **IPv4广播地址**：
  - 受限广播：`255.255.255.255`（通常被路由器拦截）。
  - 定向广播：如`192.168.1.255`（针对特定子网）。

#### **实现步骤**

1. **设置套接字选项**：
   ```c
   int enable = 1;
   setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &enable, sizeof(enable));
   ```
2. **发送数据到广播地址**：
   ```c
   sendto(sockfd, buffer, len, 0, (struct sockaddr*)&broadcast_addr, sizeof(broadcast_addr));
   ```

#### **特点**

- **优点**：简单，无需接收方预先配置。
- **缺点**：浪费带宽（所有主机必须处理包），无法跨子网。

#### **典型应用**

- DHCP协议（动态分配IP地址）。

---

### **2. 多播（Multicast）**

#### **定义**

- 多播将数据包发送到**加入特定多播组**的主机。
- **IPv4多播地址**：`224.0.0.0` ~ `239.255.255.255`（D类地址）。
- **IPv6多播地址**：以`ff00::/8`开头。

#### **实现步骤**

1. **发送方**：

   - 发送数据到多播地址，无需特殊设置（但可设置TTL）：

     ```c
     struct sockaddr_in multicast_addr;
     multicast_addr.sin_family = AF_INET;
     multicast_addr.sin_addr.s_addr = inet_addr("239.255.255.250");
     multicast_addr.sin_port = htons(12345);

     sendto(sockfd, buffer, len, 0, (struct sockaddr*)&multicast_addr, sizeof(multicast_addr));
     ```

2. **接收方**：
   - 加入多播组：

     ```c
     struct ip_mreq mreq;
     mreq.imr_multiaddr.s_addr = inet_addr("239.255.255.250");
     mreq.imr_interface.s_addr = htonl(INADDR_ANY);

     setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
     ```

   - 绑定端口并接收数据。

#### **特点**

- **优点**：跨子网传输（需路由器支持IGMP/MLD），仅组成员处理数据。
- **缺点**：需管理组成员关系，配置稍复杂。

#### **典型应用**

- 视频会议、实时数据分发（如股票行情）。

---

### **3. 关键区别**

| **特性**   | **广播**                      | **多播**                 |
| ---------- | ----------------------------- | ------------------------ |
| 目标范围   | 同一子网内所有主机            | 加入特定组的主机         |
| 地址类型   | 广播地址（如`192.168.1.255`） | D类地址（如`239.0.0.1`） |
| 网络负载   | 高（所有主机处理）            | 低（仅组成员处理）       |
| 跨子网支持 | 否                            | 是（需路由器支持）       |

---

### **4. 注意事项**

- **权限问题**：广播需设置`SO_BROADCAST`选项，否则`sendto`会返回`EACCES`错误。
- **TTL控制**：多播可通过`IP_MULTICAST_TTL`选项设置生存时间，控制传播范围：
  ```c
  u_char ttl = 32; // 允许跨越32个路由器
  setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_TTL, &ttl, sizeof(ttl));
  ```
- **环路控制**：多播默认不回环，可通过`IP_MULTICAST_LOOP`启用：
  ```c
  u_char loop = 1; // 1启用，0禁用
  setsockopt(sockfd, IPPROTO_IP, IP_MULTICAST_LOOP, &loop, sizeof(loop));
  ```

---

### **5. 代码片段示例**

#### **广播发送**

```c
int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
int enable = 1;
setsockopt(sockfd, SOL_SOCKET, SO_BROADCAST, &enable, sizeof(enable));

struct sockaddr_in addr;
memset(&addr, 0, sizeof(addr));
addr.sin_family = AF_INET;
addr.sin_port = htons(12345);
addr.sin_addr.s_addr = inet_addr("192.168.1.255"); // 定向广播地址

sendto(sockfd, "Hello Broadcast", 15, 0, (struct sockaddr*)&addr, sizeof(addr));
```

#### **多播接收**

```c
int sockfd = socket(AF_INET, SOCK_DGRAM, 0);

struct sockaddr_in addr;
memset(&addr, 0, sizeof(addr));
addr.sin_family = AF_INET;
addr.sin_port = htons(12345);
addr.sin_addr.s_addr = htonl(INADDR_ANY);
bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));

struct ip_mreq mreq;
mreq.imr_multiaddr.s_addr = inet_addr("239.255.255.250");
mreq.imr_interface.s_addr = htonl(INADDR_ANY);
setsockopt(sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));

char buffer[1024];
recvfrom(sockfd, buffer, sizeof(buffer), 0, NULL, NULL);
```

---

通过合理选择广播或多播，可以优化网络资源利用，满足不同的一对多通信需求。
