/* Here the actual conversion happens:
 TODO:
 - Upsampling from 8khz to 44.1 khz
 - convert audio into 1152 frames
  - Process in chunks: Read your raw audio data in blocks of 1152 samples (after
 upsampling to 44.1kHz). For stereo, that's 1152 samples × 2 channels ×
 bytes_per_sample.
     - Handle remainder samples: If your audio length isn't divisible by 1152,
 pad the last frame with silence (zeros) to reach exactly 1152 samples.
 - de interleave stereo
 - using LAME encoder to encodeit into mp3
*/
#include "conversion.h"
#include <stdint.h>
#include <stdlib.h>

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
// input upsampled audio from before
void convert_audio(uint8_t *rawaudio) {
  int total_samples = audio_length / bytes_per_sample;
  for (int i = 0; i <= sizeof(rawaudio); i += 1152) {
    int frame_size = min(1152, total_samples - i);
    if (sizeof(*rawaudio) % 1152 != 0) {
      // add 0 at the end
    }
  }
}
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
