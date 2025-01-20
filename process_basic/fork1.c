#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
int main()
{
	pid_t pid;

	printf("%d : Parent begin\n", getpid());

	fflush(NULL);

	pid = fork();

	if (pid < 0) {
		perror("fork()");
	}
	if (pid == 0) {
		printf("%d : Child begin\n", getpid());
		printf("%d : Child end\n", getpid());
	} else {
		printf("%d : Parent end\n", getpid());
	}

	exit(0);
}
