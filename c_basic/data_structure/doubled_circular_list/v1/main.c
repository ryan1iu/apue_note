#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#define NAMESIZE 32
typedef struct stu_st {
	char name[NAMESIZE];
	int age;
	int score;
} student;

void print(const void *data)
{
	const student *stu = data;
	printf("name: %s age: %d score: %d\n", stu->name, stu->age, stu->score);
}
int main()
{
	dlist *list = llist_create(sizeof(student));
	student stu = { "liurun", 10, 20 };
	llist_insert(list, &stu, HEAD);
	snprintf(stu.name, NAMESIZE, "jack");
	llist_insert(list, &stu, HEAD);
	snprintf(stu.name, NAMESIZE, "ryan");
	llist_insert(list, &stu, HEAD);

	llist_travel(list, print);

	llist_destroy(list);

	exit(0);
}
