/* 
 * 打开文件
 * 读取当前系统时间
 * 转换成目标格式
 * 写入文件，以追加模式
 * 循环执行上面的步骤
 */

#include <string.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define LOG_FILE "./log"
int main()
{
	char datebuf[32];
	time_t tmptr;
	struct tm *tm;
	int fd;

	fd = open(LOG_FILE, O_WRONLY | O_CREAT | O_APPEND, 0600);
	if (fd < 0) {
		perror("open failed\n");
		return -1;
	}

	while (1) {
		/* 获取日历时间，没有检测返回值是否发生了错误 */
		time(&tmptr);

		/* 转换为分解的时间结构 */
		tm = localtime(&tmptr); // 转换为本地时间
		// tm = gmtime(&tmptr); // 转换为UTC时间

		/* 转换为人类可识别的格式化字符串 */
		strftime(datebuf, 32, "%F %T\n", tm);

		/* 写入文件 */
		int lenth = strlen(datebuf);
		printf("length is %d\n", lenth);
		if (write(fd, datebuf, lenth) < 0) {
			perror("write failed\n");
			break;
		}

		sleep(1);
	}

	close(fd);
}
