#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <errno.h>

// Signal handler function to handle SIGINT
static void sig_handler(int num)
{
	write(1, "!", 1);
}

int main()
{
	int slept;
	sigset_t block_set, old_set;
	signal(SIGINT, sig_handler);

	sigemptyset(&block_set);
	sigaddset(&block_set, SIGINT);
	// Loop to print '*' and handle interruptions
	for (int i = 0; i < 20; i++) {
		// 保存旧的掩码
		sigprocmask(SIG_BLOCK, &block_set, &old_set);
		if (write(1, "*", 1) < 0) {
			if (errno == EINTR) {
				i--;
				break;
			}
		}
		// slept = 10;
		// do {
		// 	slept = sleep(slept);
		// } while (slept > 0);
		sleep(1);
		// 恢复旧的掩码
		sigprocmask(SIG_SETMASK, &old_set, NULL);
	}

	exit(0);
}
