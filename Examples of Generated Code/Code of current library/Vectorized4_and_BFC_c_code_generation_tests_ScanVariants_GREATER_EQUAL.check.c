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
  float x2 = x0[0];
  int32_t x40 = 3 + x3;
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
      float x25 = x0[x20];
      bool x26 = x25 >= x2;
      //#decorated instruction
      // generated code for decorated instruction
      //#run instruction without branching
      // generated code for run instruction without branching
      int32_t x21 = x1;
      int32_t x22 = x21 + 3;
      int32_t x23 = x22 + x3;
      x0[x23] = x19;
      x1 += x26;
      //#run instruction without branching
      //#decorated instruction
    }
  }
  int32_t x36 = x11;
  __m128 x37 = _mm_set1_ps(x2);
  __m128 x38 = _mm_set1_ps(0);
  __m128 x39 = _mm_set1_ps(0);
  int32_t x12 = x3 - x11;
  int32_t x13 = x12 * x4;
  int32_t x14 = x13 / x5;
  int32_t x41 = x14 / 1;
  for(int x43=0; x43 < x41; x43++) {
    float* x45 = (float*) &(x7[x43]);
    __m128 x46 = _mm_load_ps(x45);
    __m128 x47 = _mm_cmpge_ps(x46,x37);
    int32_t x48 = _mm_movemask_ps((__m128)x47);
    int32_t x49 = x36;
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x50 = x1;
    int32_t x51 = x50 + x40;
    x0[x51] = x49;
    int32_t x53 = (x48 & 1);
    x1 += x53;
    //#decorated instruction for vectorization
    x36 += 1;
    int32_t x58 = x36;
    int32_t x59 = (x48 >> 1);
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x60 = x1;
    int32_t x61 = x60 + x40;
    x0[x61] = x58;
    int32_t x63 = (x59 & 1);
    x1 += x63;
    //#decorated instruction for vectorization
    x36 += 1;
    int32_t x68 = x36;
    int32_t x69 = (x48 >> 2);
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x70 = x1;
    int32_t x71 = x70 + x40;
    x0[x71] = x68;
    int32_t x73 = (x69 & 1);
    x1 += x73;
    //#decorated instruction for vectorization
    x36 += 1;
    int32_t x78 = x36;
    int32_t x79 = (x48 >> 3);
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x80 = x1;
    int32_t x81 = x80 + x40;
    x0[x81] = x78;
    int32_t x83 = (x79 & 1);
    x1 += x83;
    //#decorated instruction for vectorization
    x36 += 1;
  }
  int32_t x90 = x14 * x5;
  int32_t x91 = x90 / x4;
  x15 += x91;
  int32_t x93 = x15;
  for(int x95=x93; x95 < x3; x95++) {
    int32_t x96 = x95 + 3;
    float x101 = x0[x96];
    bool x102 = x101 >= x2;
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction without branching
    // generated code for run instruction without branching
    int32_t x97 = x1;
    int32_t x98 = x97 + 3;
    int32_t x99 = x98 + x3;
    x0[x99] = x95;
    x1 += x102;
    //#run instruction without branching
    //#decorated instruction
  }
  int32_t x110 = x1;
  printf("%s\n","Number of tuples found: ");
  printf("%d\n",x110);
  bool x114 = x110 == 0;
  if (x114) {
  } else {
    printf("%s\n","Output array: ");
    for(int x117=0; x117 < x110; x117++) {
      int32_t x118 = x117 + x40;
      float x119 = x0[x118];
      printf("%f\n",x119);
    }
  }
  //#Scan Variants
}
/*****************************************
End of C Generated Code
*******************************************/
