
#include "conversion.h"
#include "datahandling.h"
#include <stdlib.h>
int main(int argc, char **argv) {

  char *file = "../audiosamples/M1F1-AlawWE-AFsp.wav";
  struct WAVheader *header = getwavheader(file);
  char *rawfile = readaudiodata(file, *header);
  free(header);
  FILE *RAW = fopen(rawfile, "wb");
  convert_audio(RAW);
  upsample(RAW);
  deinterleave_stereo(RAW);
  encode_to_mp3(RAW);
  return 0;
}
