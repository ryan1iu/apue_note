#ifndef STDIO_H__
#define STDIO_H__

#define LOG_ERROR {fprintf(stderr, "%s:%d %s\n", __FUNCTION__, __LINE__, strerror(errno));}

/* 测试fopen()函数族相关的使用 */
void _fopen();

/* 测试fgetc() fputc() fgets() fputs() */
void _fgetc(int argc, char **argv);
void _fgets(int argc, char **argv);

/* 测试fread和fwrite */
void _fread(int argc, char **argv);
/* 测试字符串常量是否能被修改 */
void _test_char();


#endif
