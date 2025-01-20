# 进程基本知识

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

## 观摩课：解释器文件

## system()

## 进程会计

## 进程时间

## 守护进程

## 系统日志


