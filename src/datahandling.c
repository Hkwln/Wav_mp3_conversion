#include "datahandling.h"
#include "decoding_algorithm.h"
#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct WAVheader *getwavheader(char *file) {
  FILE *WAVfile = fopen(file, "rb");
  if (!WAVfile) {
    printf("%s: %s\n", file, strerror(errno));
    return NULL;
  }
  struct WAVheader *wavheader = malloc(sizeof(struct WAVheader));
  // Read RIFF chunk (12 bytes)
  fread(wavheader->master.FileTypeBlocID, 4, 1, WAVfile);
  fread(&wavheader->master.FileSize, 4, 1, WAVfile);
  fread(wavheader->master.FileFormatID, 4, 1, WAVfile);

  // Read fmt chunk ID and size (8 bytes)
  fread(wavheader->data_format.FormatBlocID, 4, 1, WAVfile);
  fread(&wavheader->data_format.BlocSize, 4, 1, WAVfile);

  // Read fmt data (16 bytes minimum)
  fread(&wavheader->data_format.AudioFormat, 16, 1, WAVfile);

  // Skip extra bytes if fmt chunk is larger than 16
  if (wavheader->data_format.BlocSize > 16) {
    fseek(WAVfile, wavheader->data_format.BlocSize - 16, SEEK_CUR);
  }

  // Search for 'data' chunk
  char chunk_id[4];
  uint32_t chunk_size;
  while (fread(chunk_id, 4, 1, WAVfile) == 1) {
    fread(&chunk_size, 4, 1, WAVfile);
    if (memcmp(chunk_id, "data", 4) == 0) {
      memcpy(wavheader->sample_data.DataBlocID, chunk_id, 4);
      wavheader->sample_data.DataSize = chunk_size;
      break;
    }
    fseek(WAVfile, chunk_size, SEEK_CUR);
  }
  long pos = ftell(WAVfile);
  wavheader->pos = pos;
  fclose(WAVfile);
  return wavheader;
}
void printbuf(int16_t *buf) {
  for (int i = 0; i < 100; i++) {
    if (i % 5 == 0 && i > 4)
      printf("\n");
    printf("%#08X \t", buf[i]);
  }
  printf("\n");
}
/* read the first 100 bits and store them in the buffer*/
void readaudiodata(char *filename, struct WAVheader wavheader) {
  FILE *wav = fopen(filename, "rb"); // b = binary mode
  // first we have to know it it is a 8 bit or 16 bit audio, first we asume 16
  // bit.
  // FIXME: buffer has to be freed when should i do that? (maybe usage of memory
  // // pool?) NOTICE this applies only if you want to return the buffer,
  // currently it does not
  int16_t *buf = malloc(50 * sizeof(int16_t));
  fseek(wav, wavheader.pos, SEEK_SET);
  fread(buf, 100, 1, wav);
  fclose(wav);
  printf("these are the first 100 bits of the audio data in hexa:\n");
  printbuf(buf);
  // now encode it and print once again:
  // FIXME: referece to alaw
  int16_t *newbuf = malloc(5 * sizeof(int16_t));
  for (int i = 0; i < 10; i++)
    newbuf[i] = alaw(buf[i]);

  // TODO: calculate average amplitude:
  printf("encodet with alaw:");
  printbuf(newbuf);
  free(buf);
}
#if 1
int main() {
  char *file = "../audiosamples/M1F1-AlawWE-AFsp.wav";
  struct WAVheader *header = getwavheader(file);

  if (header) {
    printf("\n=== WAV File Header ===\n");
    printf("ChunkID: %.4s\n", header->master.FileTypeBlocID);
    printf("FileSize: %u bytes\n", header->master.FileSize + 8);
    printf("Format: %.4s\n\n", header->master.FileFormatID);

    printf("FormatID: %.4s\n", header->data_format.FormatBlocID);
    printf("FormatSize: %u\n", header->data_format.BlocSize);
    printf("AudioFormat: %u\n", header->data_format.AudioFormat);
    printf("Channels: %u\n", header->data_format.NbrChannels);
    printf("SampleRate: %u Hz\n", header->data_format.Frequency);
    printf("ByteRate: %u\n", header->data_format.BytePerSec);
    printf("BlockAlign: %u\n", header->data_format.BytePerBloc);
    printf("BitsPerSample: %u\n\n", header->data_format.BitsPerSample);

    printf("DataID: %.4s\n", header->sample_data.DataBlocID);
    printf("DataSize: %u bytes\n", header->sample_data.DataSize);

    free(header);
  }
  readaudiodata(file, *header);
  return 0;
}
#endif
