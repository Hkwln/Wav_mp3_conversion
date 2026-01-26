#include "datahandling.h"
#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
int main() {
  size_t size = 100;
  int16_t *buf = malloc(size);
  printf("size = %zu\n", size);
  free(buf);
  return 0;
}
