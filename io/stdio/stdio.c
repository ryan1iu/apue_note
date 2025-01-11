#include "stdio.h"
#include <errno.h>
#include <stdio.h>
#include <string.h>

void _fopen() {
  FILE *fp = NULL;
  int count = 0;
  /* FILE *p = fopen("/home/ryan/.zshrc", "o"); */

  /* 查看errno是如何定义的，背后是如何实现的 */
  fp = fopen("/home/ryan/notexistfile", "o");
  /* fopen失败会返回NULL，并设置error值 */
  if (fp == NULL) {
    fprintf(stderr, "errno is %d\n", errno);
    /* 更加直观的两种打印错误类型的函数 */
    perror("Function _fopen(): ");
    fprintf(stderr, "%s:%d %s\n", __FUNCTION__, __LINE__, strerror(errno));
  } else {
    /* fclose不能接受一个NULL指针 */
    fclose(fp);
  }

  /* 文件不存在则创建，指向文件开始处，允许读写.
   * 新创建的文件的permission bit由当前系统的mask决定
   * 计算公式为0666 & ~umask
   * 在当前系统中，umask是022，文件默认的权限位为644，表示允许当前用户读写，组和其他用户只读*/
  fp = fopen("test.txt", "w+");
  fclose(fp);

  /* 测试当前系统最多能打开的文件个数 */
  while (1) {
    fp = fopen("test.txt", "w");
    if (fp == NULL) {
      fprintf(stderr, "%s:%d %s\n", __FUNCTION__, __LINE__, strerror(errno));
      break;
    }
    count++;
  }
  /* 最多能打开1024个
   * 可以通过ulimit -a查看系统资源打开数上限*/
  printf("count = %d\n", count);
  return;
}

void _fgetc(int argc, char **argv) {
  /* 使用fgetc和fputc实现将一个文件复制到另一个文件中 */
  /* 不进行参数校验了 */
  int ch;
  FILE *fps = fopen(argv[1], "r");
  if (fps == NULL) {
    LOG_ERROR
    return;
  }
  FILE *fpd = fopen(argv[2], "w");
  if (fpd == NULL) {
    LOG_ERROR
    return;
  }
  while (1) {
    ch = fgetc(fps);
    if (ch == EOF)
      break;
    fputc(ch, fpd);
  }
  fclose(fpd);
  fclose(fps);
}

void _fgets(int argc, char **argv) {
  /* 使用fgets()和fputs()实现文件的复制 */
  char ch[512];
  FILE *fps = fopen(argv[1], "r");
  if (fps == NULL) {
    LOG_ERROR
    return;
  }
  FILE *fpd = fopen(argv[2], "w");
  if (fpd == NULL) {
    LOG_ERROR
    return;
  }
  while (1) {
    if (fgets(ch, 512, fps) == NULL)
      break;
    fputs(ch, fpd);
  }
  fclose(fpd);
  fclose(fps);
}

void _fread(int argc, char **argv) {
  /* 使用fread和fwrite实现文件的复制 */
  char ch[512];
  int count = 0;
  FILE *fps = fopen(argv[1], "r");
  if (fps == NULL) {
    LOG_ERROR
    return;
  }
  FILE *fpd = fopen(argv[2], "w");
  if (fpd == NULL) {
    LOG_ERROR
    return;
  }
  while (1) {
    /* fread返回正确写入的对象个数，有可能不满512 */
    if ((count = fread(ch, 1, 512, fps)) <= 0)
      break;
    /* 写入的对象数应该与读出的对象数相同 */
    printf("count is %d\n", count);
    fwrite(ch, 1, count, fpd);
  }
  fclose(fpd);
  fclose(fps);
}

void _test_char() {
  char *ptr = "abc";
  /* 在gcc下修改一个字符串常量会报段错误，但是在某些环境下比如toubc下，可能能够修改
   */
  ptr[0] = 'x';
  return;
}

int main(int argc, char **argv) {
  // _test_char();
  // _fopen();
  // _fgetc(argc, argv);
  // _fgets(argc, argv);
  _fread(argc, argv);
  return 0;
}
