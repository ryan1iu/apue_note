#include <stdio.h>
#include <stdlib.h>

#include <include/func.h>

int i = 10;
int main(void) {

  printf("[%s]: %d\n", __FUNCTION__, i);

  func();

  exit(0);
}
