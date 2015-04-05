#include <fcntl.h>
#include <errno.h>
#include <err.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <emmintrin.h>
#include <mmintrin.h>
void Scan(float*);
int main(int argc, char *argv[])
{
  if (argc < 4) {
    printf("Missing arguments. Usage: filename numberOfTuples compareValue\n");
    return 0;
  }
  FILE *ptr_file;
  char buf[1000];
  int numTuples=atoi(argv[2]);
  float compareValue=atof(argv[3]);
  int numReadTuples=0;
  ptr_file =fopen(argv[1],"r");
  if (!ptr_file){
    printf("Error. Could not open the input file.\n");
    return 0;
  }
  if (numTuples<=0){
    printf("Error. Please pass a valid number of tuples.\n");
    return 0;
  }
  float *array;
  array=(float*)malloc(((2*numTuples)+3)*sizeof(float));
  array[0]=compareValue;
  array[1]=(float)numTuples;
  array[2]=(float)0; //Unused
  while (fgets(buf,1000, ptr_file)!=NULL && numReadTuples<numTuples){
    array[numReadTuples+3]=atof(buf);
    numReadTuples++;
  }
  fclose(ptr_file);
  if (numReadTuples<numTuples){
    printf("Error, file contains less tuples than specified.\n");
    return 0;
  }
  Scan(array);
  return 1;
}
/*****************************************
Emitting C Generated Code
*******************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
void Scan(float*  x0) {
  float x3 = x0[1];
  float x4 = x3 / 4.0f;
  float x2 = x0[0];
  int32_t x63 = x4 * 4;
  //#Scan Variants
  // generated code for Scan Variants
  int32_t x1 = 0;
  for(int x6=0; x6 < x4; x6++) {
    int32_t x7 = x6 * 4;
    int32_t x8 = x7 + 3;
    float x9 = x0[x8];
    bool x10 = x9 >= x2;
    int32_t x20 = 1 + x7;
    int32_t x21 = x20 + 3;
    float x22 = x0[x21];
    bool x23 = x22 >= x2;
    int32_t x33 = 2 + x7;
    int32_t x34 = x33 + 3;
    float x35 = x0[x34];
    bool x36 = x35 >= x2;
    int32_t x46 = 3 + x7;
    int32_t x47 = x46 + 3;
    float x48 = x0[x47];
    bool x49 = x48 >= x2;
    //#run iteration from unrolled loop
    // generated code for run iteration from unrolled loop
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x10) {
      int32_t x11 = x1;
      int32_t x12 = x11 + 3;
      int32_t x13 = x12 + x3;
      x0[x13] = x7;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x23) {
      int32_t x24 = x1;
      int32_t x25 = x24 + 3;
      int32_t x26 = x25 + x3;
      x0[x26] = x20;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x36) {
      int32_t x37 = x1;
      int32_t x38 = x37 + 3;
      int32_t x39 = x38 + x3;
      x0[x39] = x33;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x49) {
      int32_t x50 = x1;
      int32_t x51 = x50 + 3;
      int32_t x52 = x51 + x3;
      x0[x52] = x46;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#run iteration from unrolled loop
  }
  for(int x65=x63; x65 < x3; x65++) {
    int32_t x66 = x65 + 3;
    float x67 = x0[x66];
    bool x68 = x67 >= x2;
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x68) {
      int32_t x69 = x1;
      int32_t x70 = x69 + 3;
      int32_t x71 = x70 + x3;
      x0[x71] = x65;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
  }
  int32_t x80 = x1;
  printf("%s\n","Number of tuples found: ");
  printf("%d\n",x80);
  bool x84 = x80 == 0;
  if (x84) {
  } else {
    printf("%s\n","Output array: ");
    int32_t x86 = 3 + x3;
    for(int x88=0; x88 < x80; x88++) {
      int32_t x89 = x88 + x86;
      float x90 = x0[x89];
      printf("%f\n",x90);
    }
  }
  //#Scan Variants
}
/*****************************************
End of C Generated Code
*******************************************/
