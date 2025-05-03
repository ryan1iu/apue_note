#include <stdio.h>
#include <stdlib.h>

#include <include/func.h>

static int i = 100;
extern int i;

void func(void) {

  printf("[%s]: %d\n", __FUNCTION__, i);
  exit(0);
}
