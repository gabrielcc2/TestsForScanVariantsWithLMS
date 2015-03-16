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
  int32_t x72 = x5 * 4;
  bool x73 = x72 < x4;
  //#Scan Variants
  // generated code for Scan Variants
  int32_t x1 = 0;
  int32_t x2 = 1;
  for(int x7=0; x7 < x5; x7++) {
    int32_t x8 = x7 * 4;
    int32_t x9 = x8 + 3;
    float x10 = x0[x9];
    bool x11 = x10 >= x3;
    int32_t x23 = 1 + x8;
    int32_t x24 = x23 + 3;
    float x25 = x0[x24];
    bool x26 = x25 >= x3;
    int32_t x38 = 2 + x8;
    int32_t x39 = x38 + 3;
    float x40 = x0[x39];
    bool x41 = x40 >= x3;
    int32_t x53 = 3 + x8;
    int32_t x54 = x53 + 3;
    float x55 = x0[x54];
    bool x56 = x55 >= x3;
    //#run iteration from unrolled loop
    // generated code for run iteration from unrolled loop
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x11) {
      int32_t x12 = x1;
      int32_t x13 = x12 + 3;
      int32_t x14 = x13 + x4;
      x0[x14] = x8;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x26) {
      int32_t x27 = x1;
      int32_t x28 = x27 + 3;
      int32_t x29 = x28 + x4;
      x0[x29] = x23;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x41) {
      int32_t x42 = x1;
      int32_t x43 = x42 + 3;
      int32_t x44 = x43 + x4;
      x0[x44] = x38;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x56) {
      int32_t x57 = x1;
      int32_t x58 = x57 + 3;
      int32_t x59 = x58 + x4;
      x0[x59] = x53;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
    //#run iteration from unrolled loop
  }
  if (x73) {
    for(int x75=x72; x75 < x4; x75++) {
      int32_t x76 = x75 + 3;
      float x77 = x0[x76];
      bool x78 = x77 >= x3;
      //#decorated instruction
      // generated code for decorated instruction
      //#run instruction with branching
      // generated code for run instruction with branching
      if (x78) {
        int32_t x79 = x1;
        int32_t x80 = x79 + 3;
        int32_t x81 = x80 + x4;
        x0[x81] = x75;
        x1 += 1;
      } else {
      }
      //#run instruction with branching
      //#decorated instruction
    }
  } else {
  }
  int32_t x94 = x1;
  printf("%s\n","Number of tuples found: ");
  printf("%d\n",x94);
  bool x98 = x94 == 0;
  if (x98) {
  } else {
    printf("%s\n","Output array: ");
    int32_t x100 = 3 + x4;
    for(int x102=0; x102 < x94; x102++) {
      int32_t x103 = x102 + x100;
      float x104 = x0[x103];
      printf("%f\n",x104);
    }
  }
  //#Scan Variants
}
/*****************************************
End of C Generated Code
*******************************************/
