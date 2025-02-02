以下是一个因为在信号处理函数中使用了不可重入函数而导致程序行为异常的例子。

______________________________________________________________________

### 问题示例

程序中有一个信号处理函数会捕获 `SIGINT` 信号，并试图使用 `printf` 输出一条信息。然而，`printf` 不是一个可重入函数，如果信号在主程序的某些地方（例如正在调用 `printf`）触发，可能会导致输出混乱甚至崩溃。

#### 示例代码

```c
#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void signal_handler(int sig) {
    // 不推荐：使用不可重入函数 printf
    printf("Signal %d caught!\n", sig); // printf 不是可重入的
}

int main() {
    // 注册信号处理函数
    signal(SIGINT, signal_handler);

    // 主程序逻辑
    while (1) {
        printf("Working...\n"); // 主程序也在调用 printf
        sleep(1);
    }

    return 0;
}
```

______________________________________________________________________

### 问题描述

1. **程序运行**：

   - 当程序运行时，主循环会不断调用 `printf` 输出 "Working...”。

1. **发送信号**：

   - 当按下 `Ctrl+C` 时，操作系统发送 `SIGINT` 信号，触发 `signal_handler`。

1. **潜在问题**：

   - 如果信号在主程序的 `printf` 尚未完成时触发，信号处理函数也调用 `printf`，导致两次调用对全局状态（如缓冲区）产生竞争。
   - `printf` 使用全局状态（例如内部缓冲区）来管理输出，这种状态可能被并发的 `printf` 修改，造成数据不一致或程序崩溃。

______________________________________________________________________

### 运行结果（可能的异常行为）

- 正常情况（低概率）：

  ```
  Working...
  Signal 2 caught!
  Working...
  ```

- 异常情况（更常见）：

  ```
  WorkSignal 2 caught!ing...
  ```

- 严重情况：

  - 输出乱码。
  - 程序崩溃。

______________________________________________________________________

### 修复方法

为了避免这个问题，应该在信号处理函数中使用可重入函数，例如 `write`：

#### 修复后的代码

```c
#include <signal.h>
#include <unistd.h>

void signal_handler(int sig) {
    // 推荐：使用可重入函数 write
    const char *message = "Signal caught!\n";
    write(STDOUT_FILENO, message, 14);
}

int main() {
    // 注册信号处理函数
    signal(SIGINT, signal_handler);

    // 主程序逻辑
    while (1) {
        const char *message = "Working...\n";
        write(STDOUT_FILENO, message, 10); // 使用 write 替代 printf
        sleep(1);
    }

    return 0;
}
```

______________________________________________________________________

### 修复后的行为

修复后的程序避免了使用 `printf`，信号处理函数使用 `write`，从而保证了程序的可重入性：

```
Working...
Signal caught!
Working...
```

______________________________________________________________________

### 总结

在信号处理函数中，应避免使用不可重入函数（如 `printf`、`malloc`、`free` 等）。使用简单且可重入的函数（如 `write`）可以避免竞争条件和不确定行为。
