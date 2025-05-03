#include <stdio.h>
#include <stdlib.h>
#include "sqllist.h"
int main()
{
	// sqlist *list = create();
	sqlist *list;
	create1(&list);

	insert(list, 1);
	insert(list, 2);
	insert(list, 3);
	insert(list, 4);
	insert(list, 5);
	insert(list, 6);
	insert(list, 7);
	insert(list, 8);
	insert(list, 9);
	// insert(list, 10);

	display(list);

	destory(list);
	exit(0);
}
