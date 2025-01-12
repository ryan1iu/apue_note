#ifndef STDIO_H__
#define STDIO_H__

#include <stdio.h>
#define LOG_ERROR {fprintf(stderr, "%s:%d %s\n", __FUNCTION__, __LINE__, strerror(errno));}

/* 测试fopen()函数族相关的使用 */
void _fopen();

/* 测试fgetc() fputc() fgets() fputs() */
void _fgetc(int argc, char **argv);
void _fgets(int argc, char **argv);

/* 测试fread和fwrite */
void _fread(int argc, char **argv);

/* 测试文件位置指针相关的函数 */
void _fseek_ftell(int argc, char **argv);

/* 缓冲区fflush强制刷新缓冲区 */
void _fflush();

/* mygetline - 自己的getline()实现
 * @ptr:    函数会将读取的内容存储到堆中开辟的一块内存空间并将该空间的起始地址写入ptr
 * @n:      函数此次读取行中所包含的字节个数，包含换行符等特殊字符，但是不包含'\0'
 * @fp:     要操作的文件指针
 */
int mygetline(char **ptr, int *n, FILE *fp);

/* mygetline_free - 与getline匹配的函数，用来释放getline开辟的内存空间，解决默认getline函数存在的隐式内存泄漏问题
 * @fp:     要操作的目标文件
 */
int mygetline_free(FILE *fp);

/* tmpfile创建匿名文件 */
void _tmpfile();

/* 测试字符串常量是否能被修改 */
void _test_char();


#endif
