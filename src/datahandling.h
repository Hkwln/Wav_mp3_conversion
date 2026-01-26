#pragma once
#include <stdint.h>
#include <stdio.h>

struct master_riff_chunk {
  char FileTypeBlocID[4]; // "RIFF"
  uint32_t FileSize;      // File size - 8
  char FileFormatID[4];   // "WAVE"
};

struct data_format {
  char FormatBlocID[4];   // "fmt "
  uint32_t BlocSize;      // Size of this chunk (usually 16, can be more)
  uint16_t AudioFormat;   // 1=PCM, 6=A-law, 7=mu-law, etc.
  uint16_t NbrChannels;   // 1=Mono, 2=Stereo
  uint32_t Frequency;     // Sample rate in Hz
  uint32_t BytePerSec;    // Bytes per second
  uint16_t BytePerBloc;   // Block align
  uint16_t BitsPerSample; // Bits per sample
};

struct sample_data {
  char DataBlocID[4]; // "data"
  uint32_t DataSize;  // Size of audio data
};

struct WAVheader {
  struct master_riff_chunk master;
  struct data_format data_format;
  struct sample_data sample_data;
  long pos;
};
struct Audio {
  int left;
  int right;
};
struct WAVheader *getwavheader(char *file);
void readaudiodata(char *file, struct WAVheader wavheader);
