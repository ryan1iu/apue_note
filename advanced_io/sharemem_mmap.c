/* 使用匿名mmap实现父子进程的通信 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
	// 忽略所有的错误检测
	char *ptr = mmap(NULL, 64, PROT_READ | PROT_WRITE,
			 MAP_SHARED | MAP_ANONYMOUS, -1, 0);

	int pid = fork();

	// ??? 根据写时复制机制，当子进程写内存后，父子进程的Ptr指向的应该是不同的物理内存吧，为什么还能够实现通信
	// !!! 关键是mmap创建时传入的MAP_SHARED选项可以让COW失效

	// ??? 子进程调用munmap之后不是会将ptr指向的内存区域清空吗，为什么父进程还是能够读取到内容
	// !!! mmap不会清空，只是取消了对应页表项的映射
	if (pid == 0) {
		// 子进程写
		strcpy(ptr, "hello");
		munmap(ptr, 64);
		exit(0);

	} else {
		// 父进程读
		wait(NULL);
		puts(ptr);
		munmap(ptr, 64);
		exit(0);
	}
}
