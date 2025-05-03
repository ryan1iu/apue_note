#include <stdio.h>
#include <stdlib.h>

#include "queue.h"

queue *qu_create()
{
	queue *q = malloc(sizeof(queue));
	if (q == NULL)
		return NULL;
	q->head = 0;
	q->tail = 0;
	return q;
}

int qu_isempty(queue *q)
{
	if (q == NULL)
		return -1;
	if (q->head == q->tail)
		return 0;
	else
		return -1;
}

int qu_enqueue(queue *q, datatype *data)
{
	/* 判断队是否已满 */
	if ((q->tail + 1) % MAXSIZE == q->head) {
		printf("queue is full\n");
		return -1;
	}
	q->data[q->tail] = *data;
	q->tail = (q->tail + 1) % MAXSIZE;
	return 0;
}

int qu_dequeue(queue *q, datatype *data)
{
	/* 判断队是否为空 */
	if (q->head == q->tail)
		return -1;
	q->head = (q->head + 1) % MAXSIZE;
	return 0;
}

void qu_travel(queue *q)
{
	int head = q->head;
	int tail = q->tail;

	if (q == NULL)
		return;
	while (head != (tail - 1) % MAXSIZE) {
		printf("%d\n", q->data[head]);
		head = (head + 1) % MAXSIZE;
	}
}

void qu_clear(queue *);

void qu_destroy(queue *q)
{
	if (q == NULL)
		return;
	free(q);
	q = NULL;
}
