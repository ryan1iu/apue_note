#include <stdio.h>
#include <stdlib.h>
void fun1()
{
	printf("fun1");
}
void fun2()
{
	printf("fun2");
}
void fun3()
{
	printf("fun3");
}
int main()
{
	printf("begin \n");

	printf("end \n");

	/* 即便不显示调用exit(), 回调函数在最后仍然会被执行  */
	atexit(fun1);
	atexit(fun2);
	atexit(fun3);
}
