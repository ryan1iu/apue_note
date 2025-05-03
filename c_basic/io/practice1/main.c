#include <stdio.h>
#include <stdlib.h>

int main() {
  int kuatuo;
  float h2o_weight = 3.0e-23;
  float kuatuo_number = 950 / h2o_weight;
  float h20_number;

  scanf("%d", &kuatuo);
  h20_number = kuatuo * kuatuo_number;
  // printf("%f\n", h20_number);
  printf("%e\n", h20_number);

  exit(0);
}
