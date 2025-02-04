#include <stdio.h>
#include <stdlib.h>

int main()
{
#pragma omp parallel
	{
		puts("hello");
	}

	exit(0);
}
