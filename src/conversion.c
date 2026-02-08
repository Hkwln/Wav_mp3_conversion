// Here the actual conversion happens:
// TODO:
// - convert audio into 1152 frames
// - Upsampling from 8khz to 44.1 khz
// - de interleave stereo
// - using LAME encoder to encodeit into mp3
#include "datahandling.h"

void convert_audio(FILE *rawaudio) {}
void upsample(FILE *rawaudio) {}
void deinterleave_stereo(FILE *rawaudio) {}
void encode_to_mp3(FILE *rawaudio) {}
