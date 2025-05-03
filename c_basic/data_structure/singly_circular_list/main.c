#include <stdio.h>
#include <stdlib.h>

#include "list.h"

int main(void)
{
	link_list *link_list;
	link_list = linked_list_create();

	linked_list_insert_tail(link_list, 1);
	linked_list_insert_tail(link_list, 2);
	linked_list_insert_tail(link_list, 3);
	linked_list_insert_tail(link_list, 4);

	linked_list_traverse(link_list);

	linked_list_delete_tail(link_list);
	linked_list_delete_tail(link_list);

	linked_list_insert_head(link_list, 5);
	linked_list_insert_head(link_list, 6);

	linked_list_traverse(link_list);

	linked_list_reverse(link_list);

	linked_list_traverse(link_list);

	linked_list_destory(link_list);

	exit(0);
}
