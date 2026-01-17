#include "datahandling.h"
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

struct WAVheader *getwavheader(char *file) {
  FILE *WAVfile = fopen(file, "rb");
  if (!WAVfile) {
    printf("Error: Cannot open file\n");
    return NULL;
  }
  struct WAVheader *wavheader = malloc(sizeof(struct WAVheader));
  wavheader->wav = *WAVfile;
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

  fclose(WAVfile);
  return wavheader;
}
uint32_t readaudiodata(struct WAVheader wavheader) {
  /*TODO: read the first 100 bits and store them in the wavheader*/
  uint32_t bits;
  // jump to the start of the sound file:
  uint16_t start = sizeof(wavheader.data_format) +
                   sizeof(wavheader.sample_data) + sizeof(wavheader.master);
  for (int i = 0; i < 100; i++) {
    // hier muss mit wavheader noch irgendwas mit fread passieren
    bits = (wavheader.wav >> start) & 1;
  }

  return bits;
}
#if 1
int main() {
  struct WAVheader *header =
      getwavheader("../audiosamples/M1F1-AlawWE-AFsp.wav");

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

  return 0;
}
#endif
