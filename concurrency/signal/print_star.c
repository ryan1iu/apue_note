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
	signal(SIGINT, sig_handler);

	// Loop to print '*' and handle interruptions
	for (int i = 0; i < 20; i++) {
		if (write(1, "*", 1) < 0) {
			if (errno == EINTR) {
				i--;
				break;
			}
		}
		slept = 10;
		do {
			slept = sleep(slept);
		} while (slept > 0);
	}

	exit(0);
}
