#pragma once
#include <stdio.h>

void convert_audio(FILE *rawaudio);
void upsample(FILE *rawaudio);
void deinterleave_stereo(FILE *rawaudio);
void encode_to_mp3(FILE *rawaudio);
