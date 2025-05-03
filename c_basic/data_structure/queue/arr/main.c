#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

int main()
{
	queue *sq;
	datatype arr[] = { 2, 34, 89, 12 };
	int i;

	sq = qu_create();
	if (NULL == sq)
		exit(1);

	for (i = 0; i < sizeof(arr) / sizeof(*arr); i++)
		qu_enqueue(sq, &arr[i]);

	qu_travel(sq);

	datatype tmp;
	qu_dequeue(sq, &tmp);
	printf("DEQUEUE:%d\n", tmp);

#if 0
    datatype tmp = 100;
    int      ret;

    ret = qu_enqueue(sq, &tmp);
    if (-1 == ret)
        printf("Queue is full!\n");
    else
        qu_travel(sq);
#endif

	qu_destroy(sq);

	exit(0);
}
