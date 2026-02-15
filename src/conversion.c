// Here the actual conversion happens:
// TODO:
// - convert audio into 1152 frames
// - Upsampling from 8khz to 44.1 khz
// - de interleave stereo
// - using LAME encoder to encodeit into mp3
#include "conversion.h"
#include <stdint.h>
#include <stdlib.h>

void convert_audio(uint8_t *rawaudio) {}

// input = a buffer of the raw audio data
uint8_t *upsample(uint8_t *rawaudio) {
  // 8 * 5.5125 = 44.1?
  // 1. define a new rawaudio which is 5.5125 times as big as the old one
  // 2. fill in the lost non bits with interpolation bits
  uint8_t *upsampled = malloc(sizeof(rawaudio) * 5.5125);
  for (int i = 0; i < sizeof(*rawaudio); i++) {
    upsampled[i] = rawaudio[i] + (rawaudio[i + 1] - rawaudio[i - 1]) * 5.5125;
  }
  return upsampled;
};
struct Audio deinterleave_stereo(uint8_t *rawaudio) {
  // basically do the same as in
  struct Audio stereo;
  uint8_t *bufleft = malloc(sizeof(*rawaudio) / 2);
  uint8_t *bufright = malloc(sizeof(*rawaudio) / 2);

  stereo.left |= bufleft;
  stereo.right |= bufright;
  return stereo;
}
// TODO: LAME
void encode_to_mp3(FILE *rawaudio) {}
