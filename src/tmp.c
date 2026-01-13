#include "datahandling.h"
#include <stdint.h>
#include <stdio.h>
int main() {
  uint8_t bit;
  int32_t bits;
  struct WAVheader *wavheader;
  printf("sizeof datastructure:(should be 44 bytes) %zu \n",
         sizeof(struct WAVheader));
  printf("uint8_t :%zu\n", sizeof(bit));
  printf("int32_r: %zu \n", sizeof(bits));

  return 0;
}
