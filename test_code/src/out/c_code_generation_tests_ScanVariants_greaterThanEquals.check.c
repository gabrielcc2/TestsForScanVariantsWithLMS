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
  int32_t x5 = x3 / 4;
  float x16 = x5 / x6;
  float x4 = x0[0];
  int32_t x10 = 2 * x6;
  int32_t x11 = 3 + x10;
  bool x46 = x6 > 0;
  int32_t x72 = x5 / x6;
  int32_t x95 = x11 + 1;
  int32_t x106 = x11 + 2;
  int32_t x117 = x11 + 3;
  int32_t x134 = x5 * 4;
  bool x135 = x134 < x3;
  int32_t x162 = x11 + x3;
  //#Scan Variants- timer goes here
  // generated code for Scan Variants- timer goes here
  int32_t x1 = 0;
  int32_t x2 = 0;
  pthread_t threads[(int)x6];
  int *inputArray;
  inputArray=(int*)malloc(x6*sizeof(int));
  void* parallelPrefixSum(void* input){
    int x8=*(int*)input;
    int32_t x12 = x8 * x5;
    int32_t x13 = x12 / x6;
    int32_t x14 = x13 * 4;
    int32_t x15 = x11 + x14;
    int32_t x39 = 3 + x8;
    //#parallel prefix sum
    // generated code for parallel prefix sum
    int32_t x9 = 0;
    for(int x18=0; x18 < x16; x18++) {
      int32_t x19 = x18 * 4;
      int32_t x20 = x15 + x19;
      float x21 = x0[x20];
      bool x22 = x21 >= x4;
      x9 += x22;
      int32_t x24 = x20 + 1;
      float x25 = x0[x24];
      bool x26 = x25 >= x4;
      x9 += x26;
      int32_t x28 = x24 + 1;
      float x29 = x0[x28];
      bool x30 = x29 >= x4;
      x9 += x30;
      int32_t x32 = x28 + 1;
      float x33 = x0[x32];
      bool x34 = x33 >= x4;
      x9 += x34;
    }
    int32_t x40 = x9;
    x0[x39] = x40;
    //#parallel prefix sum
  }
  for(int x8=0; x8 < x6; x8++) {
	inputArray[x8]=x8;
	pthread_create(&threads[x8], NULL, parallelPrefixSum, (void *)&inputArray[x8]);
  }
  for(int x8=0; x8 < x6; x8++) {
	pthread_join(threads[x8], NULL);
  }
  if (x46) {
    int32_t x47 = 3 + x6;
    x0[x47] = 0;
    for(int x50=1; x50 < x6; x50++) {
      int32_t x51 = x47 + x50;
      int32_t x52 = 3 + x50;
      int32_t x53 = x52 - 1;
      float x54 = x0[x53];
      int32_t x55 = x51 - 1;
      float x56 = x0[x55];
      float x57 = x54 + x56;
      x0[x51] = x57;
    }
    int32_t x61 = x47 - 1;
    float x62 = x0[x61];
    int32_t x63 = x61 + x6;
    float x64 = x0[x63];
    float x65 = x62 + x64;
    x2 = x65;
  } else {
  }
  void* parallelChunk(void* input){
    int x69=*(int*)input;
    int32_t x81 = 3 + x69;
    int32_t x82 = x81 + x6;
    float x83 = x0[x82];
    int32_t x84 = x83 + 3;
    int32_t x85 = x84 + x10;
    int32_t x86 = x85 + x3;
    int32_t x76 = x69 * x5;
    int32_t x77 = x76 / x6;
    //#parallel chunk
    // generated code for parallel chunk
    int32_t x70 = 0;
    int32_t x71 = 0;
    for(int x74=0; x74 < x72; x74++) {
      x71 += 0;
      int32_t x87 = x70;
      int32_t x88 = x86 + x87;
      int32_t x78 = x74 + x77;
      int32_t x79 = x78 * 4;
      int32_t x80 = x11 + x79;
      int32_t x89 = x80 - 3;
      int32_t x90 = x89 - x10;
      x0[x88] = x90;
      float x92 = x0[x80];
      bool x93 = x92 >= x4;
      x70 += x93;
      x71 += 1;
      int32_t x98 = x70;
      int32_t x99 = x86 + x98;
      int32_t x97 = x95 + x79;
      int32_t x100 = x97 - 3;
      int32_t x101 = x100 - x10;
      x0[x99] = x101;
      float x103 = x0[x97];
      bool x104 = x103 >= x4;
      x70 += x104;
      x71 += 2;
      int32_t x109 = x70;
      int32_t x110 = x86 + x109;
      int32_t x108 = x106 + x79;
      int32_t x111 = x108 - 3;
      int32_t x112 = x111 - x10;
      x0[x110] = x112;
      float x114 = x0[x108];
      bool x115 = x114 >= x4;
      x70 += x115;
      x71 += 3;
      int32_t x120 = x70;
      int32_t x121 = x86 + x120;
      int32_t x119 = x117 + x79;
      int32_t x122 = x119 - 3;
      int32_t x123 = x122 - x10;
      x0[x121] = x123;
      float x125 = x0[x119];
      bool x126 = x125 >= x4;
      x70 += x126;
    }
    //#parallel chunk
  }
  for(int x69=0; x69 < x6; x69++) {
  	pthread_create(&threads[x69], NULL, parallelChunk, (void *)&inputArray[x69]);
  }
  for(int x69=0; x69 < x6; x69++) {
	pthread_join(threads[x69], NULL);
  }
  if (x135) {
    int32_t x136 = x134 + 3;
    int32_t x137 = x136 + x10;
    int32_t x138 = x3 + 3;
    int32_t x139 = x138 + x10;
    for(int x141=x137; x141 < x139; x141++) {
      int32_t x146 = x141 - 3;
      int32_t x147 = x146 - x10;
      float x149 = x0[x141];
      bool x150 = x149 >= x4;
      //#run residue instructions after unroll
      // generated code for run residue instructions after unroll
      int32_t x142 = x2;
      int32_t x143 = x142 + x3;
      int32_t x144 = x143 + x10;
      int32_t x145 = x144 + 3;
      x0[x145] = x147;
      x2 += x150;
      //#run residue instructions after unroll
    }
  } else {
  }
  printf("%s\n","Number of tuples: ");
  int32_t x159 = x2;
  printf("%d\n",x159);
  printf("%s\n","Output array: ");
  for(int x164=0; x164 < x159; x164++) {
    int32_t x165 = x164 + x162;
    float x166 = x0[x165];
    printf("%f\n",x166);
  }
  bool x170 = x159 == 0;
  if (x170) {
    printf("%s\n","No results found.");
  } else {
  }
  //#Scan Variants- timer goes here
}
/*****************************************
End of C Generated Code
*******************************************/
