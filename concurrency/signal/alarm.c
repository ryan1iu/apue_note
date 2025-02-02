#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

static volatile unsigned long long count;
volatile int flag = 0;

void alarm_handler(int sig)
{
	flag = 1;
}

int main()
{
	alarm(5);
	signal(SIGALRM, alarm_handler);

	while (!flag) {
		count++;
	}

	printf("%lld\n", count);
	exit(0);
}
