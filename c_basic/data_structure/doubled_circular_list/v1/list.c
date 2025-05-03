#include <stdlib.h>
#include <string.h>

#include "list.h"

dlist *llist_create(int init_size)
{
	dlist *list = malloc(sizeof(dlist));
	if (list == NULL) {
		return NULL;
	}

	list->size = init_size;
	list->head.prev = &list->head;
	list->head.next = &list->head;
	return list;
}

int llist_insert(dlist *list, const void *data, enum mode mode)
{
	if (list == NULL) {
		return -1;
	}

	dlist_node *new = malloc(sizeof(dlist_node));

	/* allocate memory space of list->size */
	void *p = malloc(list->size);
	if (data == NULL) {
		return -2;
	}
	memcpy(p, data, list->size);

	if (new == NULL) {
		free(p);
		return -2;
	}
	new->data = p;

	if (mode == HEAD) {
		new->next = list->head.next;
		new->prev = &list->head;
		list->head.next = new;
		new->next->prev = new;
	} else if (mode == TAIL) {
		new->next = &list->head;
		new->prev = list->head.prev;
		list->head.prev = new;
		new->prev->next = new;
	} else {
		return -3;
	}
	return 0;
}

dlist *llist_fetch(dlist *list, const void *data, const void *key, void *cmp)
{
	return NULL;
}

void llist_travel(dlist *list, show show)
{
	if (list == NULL)
		return;
	dlist_node *ptr = list->head.next;
	while (ptr != &list->head) {
		show(ptr->data);
		ptr = ptr->next;
	}
}

int llist_destroy(dlist *list)
{
	if (list == NULL)
		return -1;

	dlist_node *ptr = list->head.next;
	while (ptr != &list->head) {
		dlist_node *p = ptr->next;
		free(ptr->data);
		free(ptr);
		ptr = p;
	}
	return 0;
}
