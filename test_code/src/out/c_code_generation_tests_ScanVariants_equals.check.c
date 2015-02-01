#include <fcntl.h>
#include <errno.h>
#include <err.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdint.h>
void Scan(float*);
int main(int argc, char *argv[])
{
  if (argc < 3) {
    printf("Missing arguments. Usage: filename numberOfTuples compareValue\n");
    return 0;
  }
  Scan((float*)argv);
  return 0;
}
/*****************************************
Emitting C Generated Code
*******************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
void Scan(float*  x0) {
  float x2 = x0[1];
  float x1 = x0[0];
  //#Scan Variants
  // generated code for Scan Variants
  for(int x4=0; x4 < x2; x4++) {
    int32_t x5 = x4 + 2;
    float x6 = x0[x5];
    bool x7 = x6 == x1;
    //#run instruction
    // generated code for run instruction
    if (x7) {
      printf("%f\n",x6);
    } else {
    }
    //#run instruction
  }
  //#Scan Variants
}
/*****************************************
End of C Generated Code
*******************************************/
