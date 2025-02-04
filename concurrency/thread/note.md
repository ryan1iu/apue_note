## 线程的概念

一个正在运行的函数

线程间通信比进程间通信要简单

线程是先标准化再实现。一个新的库发布出来默认要求必须支持线程并发

线程有多个标准，POSIX标准，是标准而非实现

pthread_t: POSIX标准下的线程标识，但是这个类型到底是什么取决于具体实现

进程就是容器，用来承载线程

- `pthread_equal()`
  比较两个线程id是否相同

- `pthread_self()`
  返回当前线程的线程标识

## 线程的创建

- `pthread_create`
  创建一个新线程

  ```c
  int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                    void *(*start_routine) (void *), void *arg);
  /*
    attr: 线程属性，NULL为默认属性
    start_routine: 线程函数的入口点
    arg: 传递给start_routine的参数
  */
  ```

## 线程的终止

- 线程的三种终止方式

  - 线程从启动例程返回，返回值就是线程的退出码
  - 线程可以被同一进程中的其他线程取消
  - 线程调用`pthread_exit()`函数，该函数会进行线程栈的清理

- 收尸
  `pthread_join()`，等待一个指定的线程结束

- 栈清理
  `pthread_cleanup_push()` `pthread_cleanup_pop()`。 两个函数必须成对出现，原因很独特

- 线程的取消选项
  - 线程取消：`pthread_cancel()`
  - 取消有两种状态，允许和不允许。可以通过`pthread_setcancelstate()`指定
  - 允许取消分为`异步cancel`和`推迟cancel`（默认）
    - 异步取消简单直接
    - `pthread_setcanceltype()` 设置取消方式
    - 推迟cancel推迟到取消点，`取消点`由POSIX定义，是可能引发阻塞的系统调用
    - `pthread_testcancel()`设置一个取消点

## 线程同步

- 互斥量

  - `pthread_mutex_t;`
  - `pthread_mutex_init()`
  - `pthread_mutex_destory()`
  - `pthread_mutex_lock()`
  - `pthread_mutex_trylock()`
  - `pthread_mutex_trylock()`
  - `pthread_mutex_unlock()`
  - `pthread_once()` 用于加载一个模块仅一次

- [条件变量](./条件变量.md)

  - `pthread_cond_t`
  - `pthread_cond_init()`
  - `pthread_cond_destory()`
  - `pthread_cond_signal()`
  - `pthread_cond_broadcast()`
  - `pthread_cond_wait()`
  - `pthread_cond_timewait()`

- 信号量
- 读写锁

## 线程属性

pthread_attr函数族

## 线程同步的属性

- 互斥量的属性
  pthread_mutexattr函数族

  - pthread_mutexattr_init()
  - pthread_mutex_attr_destory()
  - pthread_mutexattr_setpshared()
  - pthread_mutexattr_getpshared()
  - clone()
  - pthread_mutexattr_gettype()
  - pthread_mutexattr_settype()

- 条件变量
  - pthread_condattr_init()
  - ...

## 可重入

多线程中的IO处理库函数默认已经支持并发，如果不支持并发会在名字中体现出来：在函数后加一个unlocked后缀

## 线程和信号的关系

实际上，每个线程分别拥有一个mask和pending，进程用有一个pending，在从内核返回用户态反而过程中，扎回哪个线程，哪个线程负责处理所属进程的pending和本线程的penging，也就是要按位与两次

- pthread_sigmask()
- sigwait()
- pthread_kill()

## 线程与fork

POSIX原语规定新的进程只包含调用它的那个线程
