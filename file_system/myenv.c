#include <stdlib.h>
#include <stdio.h>

extern char **environ;

int main()
{
	int i = 0;
	while (environ[i] != NULL) {
		printf("%s\n", environ[i]);
		i++;
	}

	exit(0);
}
