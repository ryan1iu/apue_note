#include <stdio.h>
#include <stdlib.h>

#include "sqllist.h"

/* create a sqlist */
sqlist *create(void)
{
	sqlist *list;
	list = malloc(sizeof(sqlist));
	if (list == NULL) {
		fprintf(stderr, "create failed\n");
	}
	list->next_index = 0;
	return list;
}

/* create a sqlist */
void create1(sqlist **list)
{
	*list = malloc(sizeof(sqlist));
	if (*list == NULL) {
		fprintf(stderr, "create failed\n");
		return;
	}
	(*list)->next_index = 0;
}

void destory(sqlist *sqlist)
{
	free(sqlist);
	sqlist = NULL;
}

int insert_index(sqlist *sqlist, unsigned index, data_type data)
{
	if (sqlist == NULL) {
		fprintf(stderr, "sqlist can not be NULL\n");
		return -1;
	}
	if (index > sqlist->next_index) {
		fprintf(stderr, "index is invalid\n");
		return -2;
	}
	if (sqlist->next_index > DATASIZE) {
		fprintf(stderr, "sqlist is full\n");
		return -3;
	}

	for (int i = sqlist->next_index; i > index; i--) {
		sqlist->data[i] = sqlist->data[i - 1];
	}

	sqlist->data[index] = data;
	return index;
}

int insert(sqlist *sqlist, data_type data)
{
	if (sqlist == NULL) {
		fprintf(stderr, "sqlist can not be NULL\n");
		return -1;
	}
	if (sqlist->next_index > DATASIZE - 1) {
		fprintf(stderr, "sqlist is full\n");
		return -3;
	}
	sqlist->data[sqlist->next_index] = data;
	sqlist->next_index++;
	return sqlist->next_index;
}

int sql_remove(sqlist *sqlist, unsigned index)
{
	return 0;
}

int change(sqlist *sqlist, unsigned index, data_type data)
{
	return 0;
}

int find(sqlist *sqlist, data_type data)
{
	return 0;
}

void display(sqlist *sqlist)
{
	if (sqlist == NULL) {
		return;
	}
	for (int i = 0; i < sqlist->next_index; i++) {
		printf("%d\n", sqlist->data[i]);
	}
}
