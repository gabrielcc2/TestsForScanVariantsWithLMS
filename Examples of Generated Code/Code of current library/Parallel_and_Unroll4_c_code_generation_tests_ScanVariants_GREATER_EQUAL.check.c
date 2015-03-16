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
#include <emmintrin.h>
#include <mmintrin.h>
void Scan(float*);
int main(int argc, char *argv[])
{
  if (argc < 5) {
    printf("Missing arguments. Usage: filename numberOfTuples compareValue numThreads\n");
    return 0;
  }
  printf("Usage: filename numberOfTuples compareValue numThreads\n");
  FILE *ptr_file;
  char buf[1000];
  int numTuples=atoi(argv[2]);
  float compareValue=atof(argv[3]);
  int numThreads=atoi(argv[4]);
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
  if (numThreads<=0){
    printf("Error. Please pass a valid number of threads.\n");
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
  float x6 = x0[2];
  float x4 = x0[1];
  float x5 = x4 / 4.0f;
  int32_t x12 = x5 / x6;
  float x3 = x0[0];
  int32_t x10 = 2 * x6;
  int32_t x11 = 3 + x10;
  bool x53 = x6 > 0;
  int32_t x97 = x11 + 1;
  int32_t x108 = x11 + 2;
  int32_t x119 = x11 + 3;
  int32_t x136 = x12 * 4;
  int32_t x137 = x136 * x6;
  bool x138 = x137 < x4;
  //#Scan Variants
  // generated code for Scan Variants
  int32_t x1 = 0;
  int32_t x2 = 1;
  pthread_t threads[(int)x6];
  int *inputArray;
  inputArray=(int*)malloc(x6*sizeof(int));
  void* parallelPrefixSum(void* input){
    int x8=*(int*)input;
    int32_t x13 = x12 * x8;
    int32_t x14 = 4 * x13;
    int32_t x15 = x11 + x14;
    int32_t x46 = 3 + x8;
    //#parallel prefix sum
    // generated code for parallel prefix sum
    int32_t x9 = 0;
    for(int x17=0; x17 < x12; x17++) {
      int32_t x18 = x17 * 4;
      int32_t x19 = x15 + x18;
      float x20 = x0[x19];
      bool x21 = x20 >= x3;
      if (x21) {
        x9 += 1;
      } else {
      }
      int32_t x25 = x19 + 1;
      float x26 = x0[x25];
      bool x27 = x26 >= x3;
      if (x27) {
        x9 += 1;
      } else {
      }
      int32_t x31 = x25 + 1;
      float x32 = x0[x31];
      bool x33 = x32 >= x3;
      if (x33) {
        x9 += 1;
      } else {
      }
      int32_t x37 = x31 + 1;
      float x38 = x0[x37];
      bool x39 = x38 >= x3;
      if (x39) {
        x9 += 1;
      } else {
      }
    }
    int32_t x47 = x9;
    x0[x46] = x47;
    //#parallel prefix sum
  }
  for(int x8=0; x8 < x6; x8++) {
	inputArray[x8]=x8;
	pthread_create(&threads[x8], NULL, parallelPrefixSum, (void *)&inputArray[x8]);
  }
  for(int x8=0; x8 < x6; x8++) {
	pthread_join(threads[x8], NULL);
  }
  if (x53) {
    int32_t x54 = 3 + x6;
    x0[x54] = 0;
    for(int x57=1; x57 < x6; x57++) {
      int32_t x58 = x54 + x57;
      int32_t x59 = 3 + x57;
      int32_t x60 = x59 - 1;
      float x61 = x0[x60];
      int32_t x62 = x58 - 1;
      float x63 = x0[x62];
      float x64 = x61 + x63;
      x0[x58] = x64;
    }
    int32_t x68 = x54 - 1;
    float x69 = x0[x68];
    int32_t x70 = x68 + x6;
    float x71 = x0[x70];
    float x72 = x69 + x71;
    x1 = x72;
  } else {
  }
  void* parallelChunk(void* input){
    int x76=*(int*)input;
    int32_t x79 = x12 * x76;
    int32_t x85 = 3 + x76;
    int32_t x86 = x85 + x6;
    float x87 = x0[x86];
    int32_t x88 = x87 + x11;
    int32_t x89 = x88 + x4;
    //#parallel chunk
    // generated code for parallel chunk
    int32_t x77 = 0;
    for(int x78=0; x78 < x12; x78++) {
      int32_t x80 = x78 + x79;
      int32_t x81 = x80 * 4;
      int32_t x82 = x11 + x81;
      float x83 = x0[x82];
      bool x84 = x83 >= x3;
      if (x84) {
        int32_t x90 = x77;
        int32_t x91 = x89 + x90;
        int32_t x92 = x82 - x11;
        x0[x91] = x92;
        x77 += 1;
      } else {
      }
      int32_t x98 = x97 + x81;
      float x99 = x0[x98];
      bool x100 = x99 >= x3;
      if (x100) {
        int32_t x101 = x77;
        int32_t x102 = x89 + x101;
        int32_t x103 = x98 - x11;
        x0[x102] = x103;
        x77 += 1;
      } else {
      }
      int32_t x109 = x108 + x81;
      float x110 = x0[x109];
      bool x111 = x110 >= x3;
      if (x111) {
        int32_t x112 = x77;
        int32_t x113 = x89 + x112;
        int32_t x114 = x109 - x11;
        x0[x113] = x114;
        x77 += 1;
      } else {
      }
      int32_t x120 = x119 + x81;
      float x121 = x0[x120];
      bool x122 = x121 >= x3;
      if (x122) {
        int32_t x123 = x77;
        int32_t x124 = x89 + x123;
        int32_t x125 = x120 - x11;
        x0[x124] = x125;
        x77 += 1;
      } else {
      }
    }
    //#parallel chunk
  }
  for(int x76=0; x76 < x6; x76++) {
  	pthread_create(&threads[x76], NULL, parallelChunk, (void *)&inputArray[x76]);
  }
  for(int x76=0; x76 < x6; x76++) {
	pthread_join(threads[x76], NULL);
  }
  if (x138) {
    for(int x140=x137; x140 < x4; x140++) {
      int32_t x141 = x140 + x11;
      float x142 = x0[x141];
      bool x143 = x142 >= x3;
      //#decorated instruction
      // generated code for decorated instruction
      //#decorated instruction
      // generated code for decorated instruction
      //#run instruction with branching
      // generated code for run instruction with branching
      if (x143) {
        int32_t x144 = x1;
        int32_t x145 = x144 + x11;
        int32_t x146 = x145 + x4;
        x0[x146] = x140;
        x1 += 1;
      } else {
      }
      //#run instruction with branching
      //#decorated instruction
      //#decorated instruction
    }
  } else {
  }
  int32_t x161 = x1;
  printf("%s\n","Number of tuples found: ");
  printf("%d\n",x161);
  bool x165 = x161 == 0;
  if (x165) {
  } else {
    printf("%s\n","Output array: ");
    int32_t x167 = x11 + x4;
    for(int x169=0; x169 < x161; x169++) {
      int32_t x170 = x169 + x167;
      float x171 = x0[x170];
      printf("%f\n",x171);
    }
  }
  //#Scan Variants
}
/*****************************************
End of C Generated Code
*******************************************/
