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
  float x3 = x0[1];
  float x5 = x3 / 4.0f;
  int32_t x12 = x5 / x6;
  float x4 = x0[0];
  int32_t x10 = 2 * x6;
  int32_t x11 = 3 + x10;
  bool x45 = x6 > 0;
  int32_t x89 = x11 + 1;
  int32_t x99 = x11 + 2;
  int32_t x109 = x11 + 3;
  int32_t x125 = x12 * 4;
  int32_t x126 = x125 * x6;
  bool x127 = x126 < x3;
  int32_t x154 = x11 + x3;
  //#Scan Variants- timer goes here
  // generated code for Scan Variants- timer goes here
  int32_t x1 = 0;
  int32_t x2 = 0;
  pthread_t threads[(int)x6];
  int *inputArray;
  inputArray=(int*)malloc(x6*sizeof(int));
  void* parallelPrefixSum(void* input){
    int x8=*(int*)input;
    int32_t x13 = x12 * x8;
    int32_t x14 = 4 * x13;
    int32_t x15 = x11 + x14;
    int32_t x38 = 3 + x8;
    //#parallel prefix sum
    // generated code for parallel prefix sum
    int32_t x9 = 0;
    for(int x17=0; x17 < x12; x17++) {
      int32_t x18 = x17 * 4;
      int32_t x19 = x15 + x18;
      float x20 = x0[x19];
      bool x21 = x20 >= x4;
      x9 += x21;
      int32_t x23 = x19 + 1;
      float x24 = x0[x23];
      bool x25 = x24 >= x4;
      x9 += x25;
      int32_t x27 = x23 + 1;
      float x28 = x0[x27];
      bool x29 = x28 >= x4;
      x9 += x29;
      int32_t x31 = x27 + 1;
      float x32 = x0[x31];
      bool x33 = x32 >= x4;
      x9 += x33;
    }
    int32_t x39 = x9;
    x0[x38] = x39;
    //#parallel prefix sum
  }
  for(int x8=0; x8 < x6; x8++) {
	inputArray[x8]=x8;
	pthread_create(&threads[x8], NULL, parallelPrefixSum, (void *)&inputArray[x8]);
  }
  for(int x8=0; x8 < x6; x8++) {
	pthread_join(threads[x8], NULL);
  }
  if (x45) {
    int32_t x46 = 3 + x6;
    x0[x46] = 0;
    for(int x49=1; x49 < x6; x49++) {
      int32_t x50 = x46 + x49;
      int32_t x51 = 3 + x49;
      int32_t x52 = x51 - 1;
      float x53 = x0[x52];
      int32_t x54 = x50 - 1;
      float x55 = x0[x54];
      float x56 = x53 + x55;
      x0[x50] = x56;
    }
    int32_t x60 = x46 - 1;
    float x61 = x0[x60];
    int32_t x62 = x60 + x6;
    float x63 = x0[x62];
    float x64 = x61 + x63;
    x2 = x64;
  } else {
  }
  void* parallelChunk(void* input){
    int x68=*(int*)input;
    int32_t x75 = 3 + x68;
    int32_t x76 = x75 + x6;
    float x77 = x0[x76];
    int32_t x78 = x77 + 3;
    int32_t x79 = x78 + x10;
    int32_t x80 = x79 + x3;
    int32_t x71 = x12 * x68;
    //#parallel chunk
    // generated code for parallel chunk
    int32_t x69 = 0;
    for(int x70=0; x70 < x12; x70++) {
      int32_t x81 = x69;
      int32_t x82 = x80 + x81;
      int32_t x72 = x70 + x71;
      int32_t x73 = x72 * 4;
      int32_t x74 = x11 + x73;
      int32_t x83 = x74 - 3;
      int32_t x84 = x83 - x10;
      x0[x82] = x84;
      float x86 = x0[x74];
      bool x87 = x86 >= x4;
      x69 += x87;
      int32_t x91 = x69;
      int32_t x92 = x80 + x91;
      int32_t x90 = x89 + x73;
      int32_t x93 = x90 - 3;
      int32_t x94 = x93 - x10;
      x0[x92] = x94;
      float x96 = x0[x90];
      bool x97 = x96 >= x4;
      x69 += x97;
      int32_t x101 = x69;
      int32_t x102 = x80 + x101;
      int32_t x100 = x99 + x73;
      int32_t x103 = x100 - 3;
      int32_t x104 = x103 - x10;
      x0[x102] = x104;
      float x106 = x0[x100];
      bool x107 = x106 >= x4;
      x69 += x107;
      int32_t x111 = x69;
      int32_t x112 = x80 + x111;
      int32_t x110 = x109 + x73;
      int32_t x113 = x110 - 3;
      int32_t x114 = x113 - x10;
      x0[x112] = x114;
      float x116 = x0[x110];
      bool x117 = x116 >= x4;
      x69 += x117;
    }
    //#parallel chunk
  }
  for(int x68=0; x68 < x6; x68++) {
  	pthread_create(&threads[x68], NULL, parallelChunk, (void *)&inputArray[x68]);
  }
  for(int x68=0; x68 < x6; x68++) {
	pthread_join(threads[x68], NULL);
  }
  if (x127) {
    int32_t x128 = x126 + 3;
    int32_t x129 = x128 + x10;
    int32_t x130 = x3 + 3;
    int32_t x131 = x130 + x10;
    for(int x133=x129; x133 < x131; x133++) {
      int32_t x138 = x133 - 3;
      int32_t x139 = x138 - x10;
      float x141 = x0[x133];
      bool x142 = x141 >= x4;
      //#run residue instructions after unroll
      // generated code for run residue instructions after unroll
      int32_t x134 = x2;
      int32_t x135 = x134 + x3;
      int32_t x136 = x135 + x10;
      int32_t x137 = x136 + 3;
      x0[x137] = x139;
      x2 += x142;
      //#run residue instructions after unroll
    }
  } else {
  }
  printf("%s\n","Number of tuples: ");
  int32_t x151 = x2;
  printf("%d\n",x151);
  printf("%s\n","Output array: ");
  for(int x156=0; x156 < x151; x156++) {
    int32_t x157 = x156 + x154;
    float x158 = x0[x157];
    printf("%f\n",x158);
  }
  bool x162 = x151 == 0;
  if (x162) {
    printf("%s\n","No results found.");
  } else {
  }
  //#Scan Variants- timer goes here
}
/*****************************************
End of C Generated Code
*******************************************/
