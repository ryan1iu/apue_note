# 进程基本知识

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

## 进程标识符pid

    pid_t
    命令ps
    进程号是顺次向下增长
    getpid()
    getppid()

## 进程的产生

    fork()
        fork后父子进程的区别
            frok的返回值不同
            pid不同，ppid不同
            未决信号和文件锁不继承，资源利用量清0
        init进程：是所有进程的祖先进程
        调度器的策略决定哪个进程先运行
        在fork之前要调用fflush刷新缓冲区
    vfork() 基本废弃

## 进程的消亡与释放资源

    wait()
    waitpid()
    waitid()

## exec()函数族的使用

    execl()
    execlp()
    execle()
    execv()
    execvp()

## 用户权限和组权限

    u+s
    g+s
        real
            用户的实际身份标识，代表运行进程的实际用户
            与用户登录系统时的身份一致，通常由登录会话分配
        effective
            进程实际使用的权限标识，用于决定进程运行时的权限级别
        save
            不一定存在
        S_ISUID     04000   set-user-ID bit (see execve(2))
            如果设置了这个位，进程在执行该文件时将获得文件所有者的权限而不是进程所有者的权限
        S_ISGID     02000   set-group-ID bit (see below)
    getuid()
    geteuid()
    getgid()
    getegid()
    setuid() set effictive uid sudo的底层原理
    setgid()
    setreuid()
    setregid()
    seteuid()
    setegid()

## 观摩课：解释器文件

    #!/bin/解释器

## system()

    相当于 execl("/bin/sh", "sh", "-c", command, (char *) NULL);

## 进程会计

    acct() 方言
    当进程结束时会在指定的文件中写入进程信息

## 进程时间

    times()

## 守护进程

    会话session，sid
    终端
        一个会话绑定一个终端
    前台进程组和后台进程组
        前台进程组最多只能有一个
        前台进程组可以接收来自终端的输入
    setsid()
        使进程脱离控制终端
    getpgrp()
    getpgid()
    单实例守护进程: 锁文件/var/run/name.pid

## 系统日志

    syslogd系统日志
    openlog()
    syslog()
    closelog()
