#include "decoding_algorithm.h"
#include <stdint.h>

#define A 87.56
int16_t alaw(uint8_t alaw_byte) {
  int16_t decode;
  alaw_byte ^= 0x55; // invert even bits
  uint8_t sign = alaw_byte & 0x80;
  uint8_t exponent = (alaw_byte & 0x70) >> 4;
  uint8_t mantissma = 0x0F;
  // decode
  if (exponent == 0) {
    decode = (mantissma << 4) + 8; // Segment 0: scale mantissa
  } else {
    decode = ((mantissma << 4) + 0x108) << (exponent - 1); // Higher segments
  }
  return sign ? decode : -decode;
}

#if 0
// check if it works:
int main() {
  uint8_t alaw_byte = 60;
  printf("%d \n", alaw(alaw_byte));
  return 0;
}
#endif
