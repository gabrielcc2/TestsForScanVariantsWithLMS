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
  float x4 = x0[1];
  float x5 = x4 / 4.0f;
  float x3 = x0[0];
  int32_t x64 = x5 * 4;
  bool x65 = x64 < x4;
  //#Scan Variants
  // generated code for Scan Variants
  int32_t x1 = 0;
  int32_t x2 = 1;
  for(int x7=0; x7 < x5; x7++) {
    int32_t x8 = x7 * 4;
    int32_t x9 = x8 + 3;
    float x14 = x0[x9];
    bool x15 = x14 >= x3;
    int32_t x21 = 1 + x8;
    int32_t x22 = x21 + 3;
    float x27 = x0[x22];
    bool x28 = x27 >= x3;
    int32_t x34 = 2 + x8;
    int32_t x35 = x34 + 3;
    float x40 = x0[x35];
    bool x41 = x40 >= x3;
    int32_t x47 = 3 + x8;
    int32_t x48 = x47 + 3;
    float x53 = x0[x48];
    bool x54 = x53 >= x3;
    //#run iteration from unrolled loop
    // generated code for run iteration from unrolled loop
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction without branching
    // generated code for run instruction without branching
    int32_t x10 = x1;
    int32_t x11 = x10 + 3;
    int32_t x12 = x11 + x4;
    x0[x12] = x8;
    x1 += x15;
    //#run instruction without branching
    //#decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction without branching
    // generated code for run instruction without branching
    int32_t x23 = x1;
    int32_t x24 = x23 + 3;
    int32_t x25 = x24 + x4;
    x0[x25] = x21;
    x1 += x28;
    //#run instruction without branching
    //#decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction without branching
    // generated code for run instruction without branching
    int32_t x36 = x1;
    int32_t x37 = x36 + 3;
    int32_t x38 = x37 + x4;
    x0[x38] = x34;
    x1 += x41;
    //#run instruction without branching
    //#decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction without branching
    // generated code for run instruction without branching
    int32_t x49 = x1;
    int32_t x50 = x49 + 3;
    int32_t x51 = x50 + x4;
    x0[x51] = x47;
    x1 += x54;
    //#run instruction without branching
    //#decorated instruction
    //#run iteration from unrolled loop
  }
  if (x65) {
    for(int x67=x64; x67 < x4; x67++) {
      int32_t x68 = x67 + 3;
      float x73 = x0[x68];
      bool x74 = x73 >= x3;
      //#decorated instruction
      // generated code for decorated instruction
      //#run instruction without branching
      // generated code for run instruction without branching
      int32_t x69 = x1;
      int32_t x70 = x69 + 3;
      int32_t x71 = x70 + x4;
      x0[x71] = x67;
      x1 += x74;
      //#run instruction without branching
      //#decorated instruction
    }
  } else {
  }
  int32_t x84 = x1;
  printf("%s\n","Number of tuples found: ");
  printf("%d\n",x84);
  bool x88 = x84 == 0;
  if (x88) {
  } else {
    printf("%s\n","Output array: ");
    int32_t x90 = 3 + x4;
    for(int x92=0; x92 < x84; x92++) {
      int32_t x93 = x92 + x90;
      float x94 = x0[x93];
      printf("%f\n",x94);
    }
  }
  //#Scan Variants
}
/*****************************************
End of C Generated Code
*******************************************/
