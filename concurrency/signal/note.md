# 并发

同步：
异步：事件什么时候到来不知道，事件会产生什么样的结果不知道
异步事件的处理：查询、通知

## 信号的概念

信号是软件层面的中断
信号的响应依赖于中断 \*信号会打断阻塞的系统调用 open sleep write read accept poll...

## signal()

## 信号的不可靠

信号的行为不可靠

## sig_atomic_t的局限性

虽然`sig_atomic_t`保证了单次读写操作的安全性，但它不能保证多步操作的原子性

1. count++不是原子操作
   - 读取count
   - 递增count
   - 写入count

## 可重入函数

信号处理函数需要使用[可重入函数](./reentrant_function.md)
所有的系统调用都是可重入的
一部分库函数是可重入的

## 信号屏蔽字和Pending集的处理

未决集和阻塞集 pending mask, 内核为每个进程维护，位图，32位
mask是掩码，默认全为1，pending默认全为0
mask & pending

## 信号的响应过程

信号是在程序从内核态到用户态转化之前进行一次检查，mask & pending，来判断是否有待处理信号
如果存在待处理信号，内核将该信号的mask位和pending位置零（目的是保证handler在执行过程中不会因为同样的信号被打断），
然后将原本程序上下文中保存的返回地址，替换为信号Handler的地址，信号处理函数执行完成之后返回，
将之前置零的mask位置1，重新进行mask & pending
因此
信号处理会有延迟
思考
如何忽略掉一个信号
标准信号为什么会丢失
标准信号的响应没有严格的顺序
不能从信号处理函数中随意的往外跳（setjump\longjmp),使用sigsetjmp和siglongjmp替代

## 信号常用函数

- kill()
  向进程或进程组发信号，也可以用来检测某个进程是否存在，具体看手册
- arise()
  给当前进程或线程发送一个信号
- alarm()
- pause()
  等待一个信号
- abort()
  `abort()` 函数首先会解除对 `SIGABRT` 信号的阻塞，然后向调用进程发送该信号（类似于调用 `raise(3)`）。这将导致进程异常终止，除非 `SIGABRT` 信号被捕获并且信号处理程序没有返回（参见 `longjmp(3)`）。

如果 `SIGABRT` 信号被忽略，或者被一个会返回的信号处理程序捕获，`abort()` 函数仍然会终止进程。它会通过恢复 `SIGABRT` 信号的默认处理方式，然后再次发送该信号来实现这一点。

与其他异常终止的情况一样，使用 `atexit(3)` 和 `on_exit(3)` 注册的函数不会被调用。

- system()

## sleep()函数的局限性

可以把sleep当作alarm和pause的封装（但Linux不是这样实现的），pause会被任意信号打断，因此在sleep过程中如果有其他信号到来，会打断当前
阻塞的系统调用

可以使用`nanosleep`或`usleep`替代。sleep不具有移植性

## 高精度计时器 setitimer()

[高精度计时器](./高精度计时器.md)，提供毫秒级精度响应

## 信号集操作函数

- sigemptyset()
- sigfillset()
- sigaddset()
- sigdelset()
- sigismember()
- sigprocmask()
  将某些信号添加到屏蔽字Mask中，阻止或解除阻止
- sigpending()
  似乎不好用

## sigsuspend()与信号驱动程序

sigsuspend与pause类似都是等待一个信号的到来，但是

```c
for(;;) {
    // 阻塞某些信号
    sigprocmask(SIG_BLOCK, &set, &oset);
    dosomething();
    // 恢复之前的mask
    sigprocmask(SIG_SETMASK, &oset, NULL);
    // 等待一个信号到来之后再继续循环，所谓信号驱动
    pause();
}
```

观察以上程序，会发现在dosomething过程中发送一个信号，信号并没有打在pause上，而是在sigprocmask 和 pause之间溜走到了信号处理程序上，究其根本是因为sigprocmask和pause不原子
**sigsuspend**就是为了解决这个问题而存在的

```c
for(;;) {
    // 阻塞某些信号
    sigprocmask(SIG_BLOCK, &set, &oset);
    dosomething();
    // 恢复之前的mask并阻塞并等待一个信号到来
    sigsuspend(&oset);
}
```

## sigaction()与signal()所面对的问题

- sigaction可以避免信号处理函数嵌套导致的重入问题[例子](./mydaemon_normalexit.c)
- sigaction可以获取到有关信号的更加详细的信息，从而可以做进一步的判断。可以看token_bucket_sigaction

## 标准信号和实时信号的区别

- 实时信号不会丢失，有一个实时信号待处理队列
  队列长度有限制，可以通过`ulimit -a`查看，`pending signals`
