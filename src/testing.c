#include "datahandling.h"
// first print all the info:
int main(int argc, const char **argv) {
  // enter filename:
  char *file = "M1F1-AlawWE-AFsp.wav";
  getWAVheader(file);
  return 0;
}
