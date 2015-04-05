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
  float x2 = x0[0];
  float x3 = x0[1];
  int32_t x42 = 3 + x3;
  //#Scan Variants
  // generated code for Scan Variants
  int32_t x1 = 0;
  int32_t x4 = sizeof(float);
  int32_t x5 = sizeof(__m128);
  __m128* x7 = (__m128*) &(x0[3]);
  int32_t x8 = (intptr_t)(x7);
  int32_t x9 = x8 % x5;
  int32_t x10 = x5 - x9;
  int32_t x11 = x10 / x4;
  int32_t x15 = x11;
  char* tmp_array= (char*) x7;
  tmp_array+=x10;
  x7=(__m128*)(tmp_array);
  bool x17 = x10 == 0;
  if (x17) {
  } else {
    for(int x19=0; x19 < x11; x19++) {
      int32_t x20 = x19 + 3;
      float x21 = x0[x20];
      bool x22 = x21 >= x2;
      //#decorated instruction
      // generated code for decorated instruction
      //#run instruction with branching
      // generated code for run instruction with branching
      if (x22) {
        int32_t x23 = x1;
        int32_t x24 = x23 + 3;
        int32_t x25 = x24 + x3;
        x0[x25] = x19;
        x1 += 1;
      } else {
      }
      //#run instruction with branching
      //#decorated instruction
    }
  }
  int32_t x38 = x11;
  __m128 x39 = _mm_set1_ps(x2);
  __m128 x40 = _mm_set1_ps(0);
  __m128 x41 = _mm_set1_ps(0);
  int32_t x12 = x3 - x11;
  int32_t x13 = x12 * x4;
  int32_t x14 = x13 / x5;
  int32_t x43 = x14 / 1;
  for(int x45=0; x45 < x43; x45++) {
    float* x47 = (float*) &(x7[x45]);
    __m128 x48 = _mm_load_ps(x47);
    __m128 x49 = _mm_cmpge_ps(x48,x39);
    int32_t x50 = _mm_movemask_ps((__m128)x49);
    int32_t x51 = x38;
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x52 = (x50 & 1);
    if (x52) {
      int32_t x53 = x1;
      int32_t x54 = x53 + x42;
      x0[x54] = x51;
      x1 += 1;
    } else {
    }
    //#decorated instruction for vectorization
    x38 += 1;
    int32_t x62 = x38;
    int32_t x63 = (x50 >> 1);
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x64 = (x63 & 1);
    if (x64) {
      int32_t x65 = x1;
      int32_t x66 = x65 + x42;
      x0[x66] = x62;
      x1 += 1;
    } else {
    }
    //#decorated instruction for vectorization
    x38 += 1;
    int32_t x74 = x38;
    int32_t x75 = (x50 >> 2);
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x76 = (x75 & 1);
    if (x76) {
      int32_t x77 = x1;
      int32_t x78 = x77 + x42;
      x0[x78] = x74;
      x1 += 1;
    } else {
    }
    //#decorated instruction for vectorization
    x38 += 1;
    int32_t x86 = x38;
    int32_t x87 = (x50 >> 3);
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x88 = (x87 & 1);
    if (x88) {
      int32_t x89 = x1;
      int32_t x90 = x89 + x42;
      x0[x90] = x86;
      x1 += 1;
    } else {
    }
    //#decorated instruction for vectorization
    x38 += 1;
  }
  int32_t x100 = x14 * x5;
  int32_t x101 = x100 / x4;
  x15 += x101;
  int32_t x103 = x15;
  for(int x105=x103; x105 < x3; x105++) {
    int32_t x106 = x105 + 3;
    float x107 = x0[x106];
    bool x108 = x107 >= x2;
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x108) {
      int32_t x109 = x1;
      int32_t x110 = x109 + 3;
      int32_t x111 = x110 + x3;
      x0[x111] = x105;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
  }
  int32_t x122 = x1;
  printf("%s\n","Number of tuples found: ");
  printf("%d\n",x122);
  bool x126 = x122 == 0;
  if (x126) {
  } else {
    printf("%s\n","Output array: ");
    for(int x129=0; x129 < x122; x129++) {
      int32_t x130 = x129 + x42;
      float x131 = x0[x130];
      printf("%f\n",x131);
    }
  }
  //#Scan Variants
}
/*****************************************
End of C Generated Code
*******************************************/
