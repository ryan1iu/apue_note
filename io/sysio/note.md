文件IO=系统调用IO
文件描述符是在文件IO中贯穿始终的类型

# 文件操作符的概念

整型数，数组下标，文件描述符优先使用当前可用范围内最小的

# 文件IO操作，read，write，open，close，lseek

## open系统调用的flags

可以在 flags 中进行按位或的零个或多个文件创建标志和文件状态标志。文件创建标志包括 O_CLOEXEC、O_CREAT、O_DIRECTORY、O_EXCL、O_NOCTTY、O_NOFOLLOW、O_TMPFILE 和 O_TRUNC。文件状态标志是下面列出的所有其他标志。这两组标志之间的区别在于，文件创建标志会影响打开操作本身的语义，而文件状态标志会影响后续 I/O 操作的语义。

# 文件IO与标准IO的区别

关键区别是标准IO提供了一个缓冲区，可以减少系统调用次数，减小模式转换的开销
标准IO的吞吐量大，文件IO的响应快

标准IO与文件IO不可混用

# IO的效率问题

# 文件共享

# 原子操作

# 程序中的重定向 dup dup2

`dup` 和 `dup2` 是 Unix/Linux 系统中用于 **复制文件描述符** 的函数。它们允许程序创建新的文件描述符，指向与原始文件描述符相同的文件表项，从而共享同一个文件偏移量、访问模式等。

---

### **1. `dup` 函数**

#### **定义**

```c
#include <unistd.h>

int dup(int oldfd);
```

#### **功能**

- 创建一个新的文件描述符，其指向与 `oldfd` 相同的文件。
- 新的文件描述符是当前进程中可用的最小整数值。
- 新文件描述符与原始文件描述符共享同一个文件表项：
  - 文件偏移量。
  - 文件状态标志（如 `O_APPEND`, `O_NONBLOCK` 等）。
  - 引用计数增加。

#### **返回值**

- 成功：返回新的文件描述符。
- 失败：返回 `-1`，并设置 `errno`。

#### **示例**

```c
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int fd = open("example.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    int new_fd = dup(fd);
    if (new_fd == -1) {
        perror("dup");
        return 1;
    }

    write(fd, "Hello, ", 7);
    write(new_fd, "world!\n", 7);

    close(fd);
    close(new_fd);

    return 0;
}
```

**解释**：

- 两次 `write` 使用不同的文件描述符 `fd` 和 `new_fd`，但都写入同一个文件，因为它们共享同一个文件表项。

---

### **2. `dup2` 函数**

#### **定义**

```c
#include <unistd.h>

int dup2(int oldfd, int newfd);
```

#### **功能**

- 将 `newfd` 文件描述符重定向为 `oldfd`。
- 如果 `newfd` 已经打开，会先关闭它。
- 如果 `newfd` 和 `oldfd` 相同，`dup2` 什么也不做，只返回 `newfd`。

#### **返回值**

- 成功：返回 `newfd`。
- 失败：返回 `-1`，并设置 `errno`。

#### **示例**

```c
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    int fd = open("example.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    // 将标准输出重定向到文件
    if (dup2(fd, STDOUT_FILENO) == -1) {
        perror("dup2");
        return 1;
    }

    printf("This will be written to the file instead of the terminal.\n");

    close(fd);

    return 0;
}
```

**解释**：

- 使用 `dup2` 将文件描述符 `fd` 替换标准输出 `STDOUT_FILENO`。
- 所有写入标准输出的内容会被重定向到 `example.txt` 文件。

---

### **3. 区别**

| 特性             | `dup`                      | `dup2`                    |
| ---------------- | -------------------------- | ------------------------- |
| 参数数量         | 一个（`oldfd`）            | 两个（`oldfd`, `newfd`）  |
| 可指定新描述符   | 否，返回当前最小可用描述符 | 是，由 `newfd` 指定       |
| 自动关闭新描述符 | 不适用                     | 是（如果 `newfd` 已打开） |
| 功能             | 复制文件描述符             | 重定向文件描述符          |

---

### **4. 常见用途**

- **文件描述符重定向**：
  - 将标准输入/输出/错误重定向到文件或其他流。
- **进程间通信**：
  - 通过共享文件描述符实现通信。
- **实现特定功能**：
  - 例如，`dup2` 常用于将标准输出重定向到日志文件中。

---

### **注意事项**

1. **错误处理**：

   - 检查返回值，尤其是当文件描述符可能无效时。
   - 常见错误：
     - `EBADF`: `oldfd` 或 `newfd` 无效。
     - `EMFILE`: 打开的文件描述符超出系统限制。

2. **资源管理**：

   - 如果 `dup2` 自动关闭了 `newfd`，确保之前已完成必要的操作。

3. **线程安全**：
   - `dup` 和 `dup2` 本质上是线程安全的，但可能涉及文件共享，需要特别注意。

---

这两个函数是文件描述符操作的基础，在系统编程中非常常用，例如实现 I/O 重定向或构建更复杂的流处理机制。

# 同步 sync fsync fdatasync

# fcntl() ioctl()

# /dev/fd/ 目录
