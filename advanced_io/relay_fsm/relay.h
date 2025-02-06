/* 创建单向中继任务，与视频中不同 */
#ifndef REALY_H__
#define REALY_H__

enum job_stat{
    RUNNING = 1,
    CANCELED,
    OVER
};

struct relay_jobstat_st{
    int state;
    int fd1;
    int fd2;
    int count; // 处理的字节数
};

#define JOB_MAX 1024
    
/*
 * relay_add - 添加一个任务
 * @fd1: 源文件描述符
 * @fd2: 目标文件描述符
 *
 * 该函数用于添加一个中继任务
 *
 * 返回值: 如果成功返回任务id，失败返回errno 
 */
int relay_add(int fd1, int fd2);

int relay_cancel(int job);

int relay_wait(int job);

int relay_stat(int job, struct relay_jobstat_st *stat);

#endif
