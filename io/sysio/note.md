文件IO=系统调用IO
文件描述符是在文件IO中贯穿始终的类型

# 文件操作符的概念

整型数，数组下标，文件描述符优先使用当前可用范围内最小的
# 文件IO操作，read，write，open，close，lseek

# 文件IO与标准IO的区别
关键区别是标准IO提供了一个缓冲区，可以减少系统调用次数，减小模式转换的开销
标准IO的吞吐量大，文件IO的响应快

标准IO与文件IO不可混用

# IO的效率问题

# 文件共享

# 原子操作

# 程序中的重定向 dup dup2

# 同步 sync fsync fdatasync

# fcntl() ioctl()

# /dev/fd/ 目录


