#include <fcntl.h>
#include <errno.h>
#include <err.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
void Scan(float*);
int main(int argc, char *argv[])
{
  if (argc < 4) {
    printf("Missing arguments. Usage: filename numberOfTuples compareValue numThreads\n");
    return 0;
  }
  printf("Usage: filename numberOfTuples compareValue numThreads\n");
  printf("If the operator is not parallelized, please pass numThreads=0\n");
  printf("If the operator is parallelized and numThreads=0, it will result in errors.\n");
  FILE *ptr_file;
  char buf[1000];
  int numTuples=atoi(argv[2]);
  float compareValue=atof(argv[3]);
  int numThreads=atoi(argv[4]);
  int numReadTuples=0;
  ptr_file =fopen(argv[1],"r");
  if (!ptr_file){
    return 0;
  }
  float *array;
  array=(float*)malloc(((2*numTuples)+3+(2*numThreads))*sizeof(float));
  array[0]=compareValue;
  array[1]=(float)numTuples;
  array[2]=(float)numThreads;
  for (int i=0; i<(2*numThreads); i++){
    array[3+i]=(float)0;
  }
  while (fgets(buf,1000, ptr_file)!=NULL && numReadTuples<numTuples){
    array[numReadTuples+3+(2*numThreads)]=atof(buf);
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
  float x8 = x0[2];
  float x5 = x0[1];
  int32_t x7 = x5 / 4;
  float x12 = x7 / x8;
  float x6 = x0[0];
  int32_t x15 = 2 * x8;
  int32_t x16 = 3 + x15;
  int32_t x27 = x16 + 1;
  int32_t x34 = x16 + 2;
  int32_t x41 = x16 + 3;
  bool x57 = x8 > 0;
  int32_t x82 = x7 / x8;
  int32_t x137 = x7 * 4;
  bool x138 = x137 < x5;
  //#Scan Variants- timer goes here
  // generated code for Scan Variants- timer goes here
  int32_t x1 = 0;
  int32_t x2 = 1;
  int32_t x3 = 3;
  int32_t x4 = 0;
  pthread_t threads[(int)x8];
  int *inputArray;
  inputArray=(int*)malloc(x8*sizeof(int));
  void* parallelPrefixSum(void* input){
    int x10=*(int*)input;
    int32_t x17 = x10 * x7;
    int32_t x18 = x17 / x8;
    int32_t x50 = 3 + x10;
    //#parallel prefix sum
    // generated code for parallel prefix sum
    int32_t x11 = 0;
    for(int x14=0; x14 < x12; x14++) {
      int32_t x19 = x14 + x18;
      int32_t x20 = x19 * 4;
      int32_t x21 = x16 + x20;
      float x22 = x0[x21];
      bool x23 = x22 >= x6;
      if (x23) {
        x11 += 1;
      } else {
      }
      int32_t x28 = x27 + x20;
      float x29 = x0[x28];
      bool x30 = x29 >= x6;
      if (x30) {
        x11 += 1;
      } else {
      }
      int32_t x35 = x34 + x20;
      float x36 = x0[x35];
      bool x37 = x36 >= x6;
      if (x37) {
        x11 += 1;
      } else {
      }
      int32_t x42 = x41 + x20;
      float x43 = x0[x42];
      bool x44 = x43 >= x6;
      if (x44) {
        x11 += 1;
      } else {
      }
    }
    int32_t x51 = x11;
    x0[x50] = x51;
    //#parallel prefix sum
  }
  for(int x10=0; x10 < x8; x10++) {
	inputArray[x10]=x10;
	pthread_create(&threads[x10], NULL, parallelPrefixSum, (void *)&inputArray[x10]);
  }
  for(int x10=0; x10 < x8; x10++) {
	pthread_join(threads[x10], NULL);
  }
  if (x57) {
    int32_t x58 = 3 + x8;
    x0[x58] = 0;
    for(int x61=1; x61 < x8; x61++) {
      int32_t x62 = 3 + x61;
      int32_t x63 = x62 + x8;
      int32_t x64 = x62 - 1;
      float x65 = x0[x64];
      int32_t x66 = x64 + x8;
      float x67 = x0[x66];
      float x68 = x65 + x67;
      x0[x63] = x68;
    }
    int32_t x72 = x58 - 1;
    float x73 = x0[x72];
    int32_t x74 = x72 + x8;
    float x75 = x0[x74];
    float x76 = x73 + x75;
    x4 = x76;
  } else {
  }
  void* parallelChunk(void* input){
    int x80=*(int*)input;
    int32_t x85 = x80 * x7;
    int32_t x86 = x85 / x8;
    int32_t x92 = 3 + x80;
    int32_t x93 = x92 + x8;
    float x94 = x0[x93];
    int32_t x95 = x94 + 3;
    int32_t x96 = x95 + x15;
    int32_t x97 = x96 + x5;
    //#parallel chunk
    // generated code for parallel chunk
    int32_t x81 = 0;
    for(int x84=0; x84 < x82; x84++) {
      int32_t x87 = x84 + x86;
      int32_t x88 = x87 * 4;
      int32_t x89 = x16 + x88;
      float x90 = x0[x89];
      bool x91 = x90 >= x6;
      if (x91) {
        int32_t x98 = x81;
        int32_t x99 = x97 + x98;
        x0[x99] = x90;
        x81 += 1;
      } else {
      }
      int32_t x104 = x27 + x88;
      float x105 = x0[x104];
      bool x106 = x105 >= x6;
      if (x106) {
        int32_t x107 = x81;
        int32_t x108 = x97 + x107;
        x0[x108] = x105;
        x81 += 1;
      } else {
      }
      int32_t x113 = x34 + x88;
      float x114 = x0[x113];
      bool x115 = x114 >= x6;
      if (x115) {
        int32_t x116 = x81;
        int32_t x117 = x97 + x116;
        x0[x117] = x114;
        x81 += 1;
      } else {
      }
      int32_t x122 = x41 + x88;
      float x123 = x0[x122];
      bool x124 = x123 >= x6;
      if (x124) {
        int32_t x125 = x81;
        int32_t x126 = x97 + x125;
        x0[x126] = x123;
        x81 += 1;
      } else {
      }
    }
    //#parallel chunk
  }
  for(int x80=0; x80 < x8; x80++) {
  	pthread_create(&threads[x80], NULL, parallelChunk, (void *)&inputArray[x80]);
  }
  for(int x80=0; x80 < x8; x80++) {
	pthread_join(threads[x80], NULL);
  }
  if (x138) {
    int32_t x139 = x137 + 3;
    int32_t x140 = x139 + x15;
    int32_t x141 = x5 + 3;
    int32_t x142 = x141 + x15;
    for(int x144=x140; x144 < x142; x144++) {
      float x145 = x0[x144];
      bool x146 = x145 >= x6;
      //#run residue instructions after unroll
      // generated code for run residue instructions after unroll
      if (x146) {
        int32_t x147 = x4;
        int32_t x148 = x147 + x5;
        int32_t x149 = x148 + x15;
        int32_t x150 = x149 + 3;
        x0[x150] = x145;
        x4 += 1;
      } else {
      }
      //#run residue instructions after unroll
    }
  } else {
  }
  printf("%s\n","Output array: ");
  int32_t x162 = x4;
  for(int x164=0; x164 < x162; x164++) {
    int32_t x165 = x164 + 3;
    int32_t x166 = x165 + x15;
    int32_t x167 = x166 + x5;
    float x168 = x0[x167];
    printf("%f\n",x168);
  }
  bool x172 = x162 == 0;
  if (x172) {
    printf("%s\n","No results found.");
  } else {
  }
  //#Scan Variants- timer goes here
}
/*****************************************
End of C Generated Code
*******************************************/
