#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/* initializing the linked list */
link_list *linked_list_create()
{
	link_list *head = malloc(sizeof(link_list));
	if (head == NULL) {
		fprintf(stderr, "[%s]:(%s)line:%d :Memory allocation failed\n",
			__FILE__, __FUNCTION__, __LINE__);
	}
	head->data = 0;
	head->next = head;
	return head;
}

/* destorying the linked list */
int linked_list_destory(link_list *list)
{
	link_list *p = list;
	link_list *q;
	while (p != NULL && p != list) {
		q = p->next;
		free(p);
		p = NULL;
		p = q;
	}
	return 0;
}

/* inserting a node at the beginning of the linked list */
int linked_list_insert_head(link_list *list, int data)
{
	if (list == NULL)
		return -1;
	link_list *p = malloc(sizeof(link_list));
	if (p == NULL) {
		fprintf(stderr, "[%s]:(%s)line:%d :Memory allocation failed\n",
			__FILE__, __FUNCTION__, __LINE__);
	}
	p->data = data;
	p->next = list->next;
	list->next = p;
	return 0;
}

/* inserting a node at the end of the linked list */
int linked_list_insert_tail(link_list *list, int data)
{
	if (list == NULL)
		return -1;
	link_list *p = list->next;
	while (p->next != list)
		p = p->next;

	link_list *new = malloc(sizeof(link_list));
	if (new == NULL) {
		fprintf(stderr, "[%s]:(%s)line:%d :Memory allocation failed\n",
			__FILE__, __FUNCTION__, __LINE__);
	}

	new->data = data;
	new->next = p->next;
	p->next = new;
	return 0;
}

/* deleting the head node */
int linked_list_delete_head(link_list *list)
{
	if (list == NULL)
		return -1;
	if (list->next == list)
		return -2;

	link_list *p = list->next;
	list->next = list->next->next;
	free(p);
	p = NULL;

	return 0;
}

/* deleting the tail node */
int linked_list_delete_tail(link_list *list)
{
	if (list == NULL)
		return -1;
	if (list->next == list)
		return -2;

	link_list *p = list->next;
	while (p->next->next != list)
		p = p->next;

	link_list *q = p->next;
	p->next = p->next->next;
	free(q);

	return 0;
}

/* deleting a specified node */
int linked_list_delete_specified(link_list *list, int data)
{
	link_list *p = list;
	while (p != list) {
		if (p->next->data == data) {
			link_list *q = p->next;
			p->next = p->next->next;
			free(q);
			q = NULL;
			break;
		}
		p = p->next;
	}
	return 0;
}

/* searching a node by index */
link_list *linked_list_find_index(link_list *list, int index)
{
	int i = 1;
	link_list *p = list->next;
	while (p != list) {
		if (i == index) {
			return p;
		}
		p = p->next;
		i++;
	}
	return NULL;
}

/* searching a node by value and returning the pointer to the first matching node */
link_list *linked_list_find_value(link_list *list, int value)
{
	if (list == NULL)
		return NULL;
	link_list *p = list->next;

	while (p != list) {
		if (p->data == value) {
			return p;
		}
		p = p->next;
	}

	return NULL;
}

/* traversing the linked list */
void linked_list_traverse(link_list *list)
{
	link_list *p = list->next;
	while (p != list) {
		printf("%d\n", p->data);
		p = p->next;
	}
}

/* returning length of linked list */
unsigned linked_list_length(link_list *list)
{
	int length = 0;
	if (list == NULL)
		return 0;
	link_list *p = list;
	while (p->next != list) {
		length++;
		p = p->next;
	}

	return length;
}

/* reversing the linked list */
void linked_list_reverse(link_list *list)
{
	if (list == NULL)
		return;
	link_list *p = list->next;
	link_list *q = list->next->next;
	while (q != list) {
		p->next = q->next;
		q->next = list->next;
		list->next = q;
		q = p->next;
	}
}
