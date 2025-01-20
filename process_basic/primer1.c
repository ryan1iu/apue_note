
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
int main()
{
	int left = 200000;
	int right = 200200;
	for (int i = left; i <= right; i++) {
		pid_t pid = fork();
		if (pid < 0) {
			exit(0);
		}
		if (pid == 0) {
			int flag = 0;
			for (int j = 2; j < i / 2; j++) {
				if (i % j == 0) {
					flag = 1;
					break;
				}
			}
			if (flag == 0) {
				printf("%d is primer\n", i);
			}
			// sleep(10000);
			exit(0);
		}
	}
	// sleep(10000);
}
