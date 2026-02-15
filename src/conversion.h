#pragma once
#include "datahandling.h"
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

void convert_audio(uint8_t *rawaudio);
uint8_t *upsample(uint8_t *rawaudio);
struct Audio deinterleave_stereo(uint8_t *rawaudio);
void encode_to_mp3(FILE *rawaudio);
