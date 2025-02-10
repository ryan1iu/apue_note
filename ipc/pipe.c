/* 创建一个匿名管道，父写子读，子进程加载mpg123，播放父进程传输的音频数据 */

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>

int main()
{
	int pid;
	int ret;
	int pd[2];
	char buf[192 * 1024]; // 192kBs

	ret = pipe(pd);
	assert(ret == 0);
	pid = fork();
	assert(pid >= 0);
	if (pid == 0) {
		// 关闭写端
		close(pd[1]);
		// 读端重定向到标准输入
		dup2(pd[0], 0);
		close(pd[0]);
		// 加载mpg123
		execlp("mpg123", "mpg123", "-", NULL);
		perror("execlp()");
		exit(0);
	}

	int file = open("./yellow.mp3", O_RDONLY);
	// emmmm，这个简单的流控有意义吗，读端来控制读取的速率不是更好
	// 确实没意义，不加流控也能正确播放
	do {
		ret = read(file, buf, 192 * 1024);
		write(pd[1], buf, 192 * 1024);
		// sleep(1);
	} while (ret > 0);

	close(pd[0]);
	close(pd[1]);
	close(file);
	exit(0);
}
