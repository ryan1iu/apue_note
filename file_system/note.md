# 目录和文件

## 获取文件属性

stat

## 文件访问权限

st_mode 是一个16位的位图

## umask: 防止产生权限过松的文件

## 文件权限的修改

chmod
fchmod

## 粘住位

t位，现在已经不再常用，目的是让某一个二进制文件在执行完成后留在内存中方便下一次加载

## 文件系统：FAT和UFS

## 硬链接、符号链接

link
unlink

rename
remove

remove() deletes a name from the filesystem. It calls unlink(2) for files, and rmdir(2) for directories.

硬链接与目录项是同义词，硬链接建立有限制：不能够跨分区建立，不能够给目录建立；符号链接可以

## utime: 更改文件最后读的时间和最后修改的时间

## 目录的创建和销毁

mkdir
rmdir

## 更改当前工作路径

chdir
fchdir
getcwd

## 分析目录/读取目录内容

glob
opendir
readdir
rewinddir
seekdir
closedir

# 系统数据文件和信息

和进程环境相关，和要做的ls练习相关

## /etc/passwd

    getpwuid() 根据uid或name来查询
    getpwnam()

## /etc/group

    getgrgid()
    getgrgrnam()

## /etc/shadow

    getspnam() 根据name返回shadow文件中的一行
    crypt() 加密
    getpass() 关闭终端回显，从终端接收密码

## 时间戳

    各种时间的转换函数，可以参考unix系统编程里面的图示
    time_t char * struct tm
    time()
    gmtime()
    localtime()
    mktime()
    strftime() 格式化的时间和日期

# 进程环境

## main函数

## 进程的终止 背诵

    正常终止
        从main函数返回
        调用exit 返回值从-128到127
            在进程正常终止时会调用atexit注册的函数
        调用_exit或_Exit
            exit是库函数，_exit是系统调用
        最后一个线程从其启动例程返回
        最后一个线程调用pthread_exit
    异常终止
        调用abort函数，发送一个杀死进程的信号
        接到一个信号并终止
        最后一个线程对其取消请求做出响应

## 命令行参数的分析

    getopt()
    getopt_long()

## 环境变量

    程序员和用户的约定
    KEY = VALUE
    getenv()
    setenv() 改变或添加一个环境变量
    unsetenv() 删除一个环境变量
    putenv() 不好用

## C程序的存储空间布局

    ps axf
    pmap

## 库

    动态库
    静态库
    共享库/手工装载库
        dlopen()
        dlclose()
        dlerror()
        dlsym()

## 函数跳转

    setjmp() 安全的跨函数跳转，区别于goto
    longjmp()
    有一种情况不能跳，以后学到信号再来补充

## 资源的获取以及控制

    getrlimit()
    setrlimit()

# 实现myls

支持ls -a -l -n -i /dirorfile
inode节点号 文件类型文件权限 用户 组 大小 时间 文件名
