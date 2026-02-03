#include "datahandling.h"
#include "decoding_algorithm.h"
#include <errno.h>
#include <stdbool.h>
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
void printbuf(int16_t *buf, size_t buffsize) {
  printf("left: \t\t\t\t\t right: \n");
  // 46930
  for (int i = 0; i < buffsize; i++) {
    if (i % 2 == 0) {
      printf("%#08X \t\t\t\t", buf[i]);
    } else {
      printf(" %#08X \n", buf[i]);
    }
  }
  printf("\n");
}
// Requires buffer to be the same or get the
struct Audio calcamplitude(uint8_t *buf1, int16_t *buf2,
                           size_t smallestbuffsize) {
  // FIXME: This nested loop is O(n^2) and processes data incorrectly
  // TODO: Stereo samples are interleaved: [L0][R0][L1][R1]...
  // TODO: Loop should be: for(i=0; i<num_samples; i+=2) { left += buf2[i]; right += buf2[i+1]; }
  // TODO: Then divide by (num_samples/2) to get average amplitude
  int leftamplitude = 0;
  int rightamplitude = 0;
  bool active = true;
  for (int i = 0; i < smallestbuffsize; i++) {
    for (int b = 0; b < smallestbuffsize; b++) {
      if (i % 2 == 0) {
        if (b % 2 == 0 && active) {
          leftamplitude += buf1[b] + buf1[b + 1] - buf2[i];
          active = false;
        } else {
          active = true;
        }
      } else if (b % 2 == 0 && active) {
        rightamplitude += buf1[i] + buf1[i + 1] - buf2[i];
        active = false;
      } else {
        active = true;
      }
    }
  }
  struct Audio audio;
  audio.left = leftamplitude;
  audio.right = rightamplitude;
  return audio;
}
/* read the first 100 bits and store them in the buffer*/
void readaudiodata(char *filename, struct WAVheader wavheader) {
  FILE *wav = fopen(filename, "rb"); // b = binary mode
  size_t sizebuf = wavheader.sample_data.DataSize;
#if 1
  uint8_t *buf = malloc(sizebuf);
  fseek(wav, wavheader.pos, SEEK_SET);
  fread(buf, 1, sizebuf, wav);
  fclose(wav);
  // printbuf(buf, sizebuf / sizeof(int16_t));
  //  now encode it and print once again:
  // FIXME: newbuf should allocate sizebuf * sizeof(int16_t), not just sizebuf
  // TODO: A-law decodes uint8_t (1 byte) -> int16_t (2 bytes), so output is 2x larger
  int16_t *newbuf = malloc(sizebuf);
  // FIXME: Loop processes only half the bytes! sizebuf=46986 bytes, but loop runs 23493 times
  // TODO: Should be: for (int i = 0; i < sizebuf; i++) since each buf[i] is one A-law byte
  for (int i = 0; i < sizebuf / sizeof(int16_t); i++)
    newbuf[i] = alaw(buf[i]);

  //: calculate average amplitude:
  // printf("encodet with alaw:\n");
  // printbuf(newbuf, sizenewbuf);
  // TODO: calcamplitude receives wrong size - should be number of int16_t samples, not bytes
  struct Audio amplitude = calcamplitude(buf, newbuf, sizebuf);
  printf("amplitude: \nleft: %d \t\t\t\t right: %d\n", amplitude.left,
         amplitude.right);
  // store the buffer into a raw file:
  FILE *raw = fopen("data.raw", "wb");
  // TODO: Should write (sizebuf * sizeof(int16_t)) bytes if you decoded all samples correctly
  fwrite(newbuf, sizebuf, 1, raw);
  fclose(raw);
  free(buf);
  free(newbuf);
#endif
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
    // printf("data size without the header: %ld \n",
    //   header->sample_data.DataSize - sizeof(*header));
  }
  readaudiodata(file, *header);
  free(header);
  return 0;
}
#endif
