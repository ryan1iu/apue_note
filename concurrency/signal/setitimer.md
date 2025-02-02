`setitimer()` 是一个用于设置定时器的 Linux/POSIX 系统调用，可以实现微秒级（us）精度的定时器。

---

## **1. 函数原型**

```c
#include <sys/time.h>
int setitimer(int which, const struct itimerval *new_value, struct itimerval *old_value);
```

### **2. 参数说明**

- `which`：指定定时器类型，有三种：
  - `ITIMER_REAL`（**真实时间**，系统时钟）：时间到期后，发送 `SIGALRM` 信号。
  - `ITIMER_VIRTUAL`（**用户态 CPU 时间**）：仅在进程运行时计时，时间到后发送 `SIGVTALRM` 信号。
  - `ITIMER_PROF`（**用户态+内核态 CPU 时间**）：用于分析进程运行情况，发送 `SIGPROF` 信号。
- `new_value`：新的定时器值（`struct itimerval`）。
- `old_value`（可选）：存储旧的定时器值，可传 `NULL` 忽略。

---

## **3. `struct itimerval` 结构**

```c
struct itimerval {
    struct timeval it_interval; // 周期性触发的间隔
    struct timeval it_value;    // 初始触发时间
};
```

- `struct timeval`：

  ```c
  struct timeval {
      time_t tv_sec;    // 秒
      suseconds_t tv_usec; // 微秒（1秒 = 1000000微秒）
  };
  ```

---

## **4. 示例代码**

**实现 1.5 秒后触发 `SIGALRM`，并每 2 秒重复触发**

```c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <sys/time.h>

void timer_handler(int signum) {
    printf("Timer expired!\n");
}

int main() {
    struct itimerval timer;

    // 设置信号处理函数
    signal(SIGALRM, timer_handler);

    // 第一次触发时间：1.5 秒后
    timer.it_value.tv_sec = 1;
    timer.it_value.tv_usec = 500000; // 500 毫秒

    // 之后每 2 秒触发一次
    timer.it_interval.tv_sec = 2;
    timer.it_interval.tv_usec = 0;

    // 设置定时器
    setitimer(ITIMER_REAL, &timer, NULL);

    while (1) {
        pause(); // 等待信号
    }

    return 0;
}
```

---

## **5. `setitimer()` 相关操作**

### **(1) 取消定时器**

要取消定时器，可以将 `it_value` 设为 `0`：

```c
struct itimerval timer = {0}; // 结构体全零，表示取消定时器
setitimer(ITIMER_REAL, &timer, NULL);
```

---

### **(2) 获取当前定时器状态**

```c
struct itimerval curr_timer;
getitimer(ITIMER_REAL, &curr_timer);
printf("Remaining time: %ld sec, %ld usec\n",
       curr_timer.it_value.tv_sec, curr_timer.it_value.tv_usec);
```

---

## **6. `setitimer()` vs. `alarm()`**

| **方法**      | **精度** | **触发信号**                      | **是否支持周期** | **适用场景**     |
| ------------- | -------- | --------------------------------- | ---------------- | ---------------- |
| `alarm()`     | **秒级** | `SIGALRM`                         | 只触发一次       | 简单定时任务     |
| `setitimer()` | _微秒级_ | `SIGALRM`、`SIGVTALRM`、`SIGPROF` | 支持周期触发     | 需要更精确的定时 |

---

## **7. 总结**

- `setitimer()` 可实现微秒级定时，适用于高精度定时任务。
- 结合 `SIGALRM` 信号处理函数，可以定期执行任务。
- 与 `alarm()` 不同，它可以周期性触发，而 `alarm()` 只能触发一次。

如果需要更高精度（纳秒级），可以考虑 `timer_create()` + `timer_settime()`。
