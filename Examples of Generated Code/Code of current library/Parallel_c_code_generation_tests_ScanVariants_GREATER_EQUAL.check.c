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
  float x4 = x0[2];
  float x3 = x0[1];
  int32_t x10 = x3 / x4;
  float x2 = x0[0];
  int32_t x5 = 2 * x4;
  int32_t x6 = 3 + x5;
  bool x31 = x4 > 0;
  int32_t x80 = x10 * x4;
  //#Scan Variants
  // generated code for Scan Variants
  int32_t x1 = 0;
  pthread_t threads[(int)x4];
  int *inputArray;
  inputArray=(int*)malloc(x4*sizeof(int));
  void* parallelPrefixSum(void* input){
    int x8=*(int*)input;
    int32_t x11 = x10 * x8;
    int32_t x12 = x6 + x11;
    int32_t x24 = 3 + x8;
    //#parallel prefix sum
    // generated code for parallel prefix sum
    int32_t x9 = 0;
    for(int x14=0; x14 < x10; x14++) {
      int32_t x15 = x12 + x14;
      float x16 = x0[x15];
      bool x17 = x16 >= x2;
      if (x17) {
        x9 += 1;
      } else {
      }
    }
    int32_t x25 = x9;
    x0[x24] = x25;
    //#parallel prefix sum
  }
  for(int x8=0; x8 < x4; x8++) {
	inputArray[x8]=x8;
	pthread_create(&threads[x8], NULL, parallelPrefixSum, (void *)&inputArray[x8]);
  }
  for(int x8=0; x8 < x4; x8++) {
	pthread_join(threads[x8], NULL);
  }
  if (x31) {
    int32_t x32 = 3 + x4;
    x0[x32] = 0;
    for(int x35=1; x35 < x4; x35++) {
      int32_t x36 = x32 + x35;
      int32_t x37 = 3 + x35;
      int32_t x38 = x37 - 1;
      float x39 = x0[x38];
      int32_t x40 = x36 - 1;
      float x41 = x0[x40];
      float x42 = x39 + x41;
      x0[x36] = x42;
    }
    int32_t x46 = x32 - 1;
    float x47 = x0[x46];
    int32_t x48 = x46 + x4;
    float x49 = x0[x48];
    float x50 = x47 + x49;
    x1 = x50;
  } else {
  }
  void* parallelWriting(void* input){
    int x54=*(int*)input;
    int32_t x57 = x10 * x54;
    int32_t x62 = 3 + x54;
    int32_t x63 = x62 + x4;
    float x64 = x0[x63];
    int32_t x65 = x64 + x6;
    int32_t x66 = x65 + x3;
    //#parallel writing
    // generated code for parallel writing
    int32_t x55 = 0;
    for(int x56=0; x56 < x10; x56++) {
      int32_t x58 = x56 + x57;
      int32_t x59 = x6 + x58;
      float x60 = x0[x59];
      bool x61 = x60 >= x2;
      if (x61) {
        int32_t x67 = x55;
        int32_t x68 = x66 + x67;
        int32_t x69 = x59 - x6;
        x0[x68] = x69;
        x55 += 1;
      } else {
      }
    }
    //#parallel writing
  }
  for(int x54=0; x54 < x4; x54++) {
  	pthread_create(&threads[x54], NULL, parallelWriting, (void *)&inputArray[x54]);
  }
  for(int x54=0; x54 < x4; x54++) {
	pthread_join(threads[x54], NULL);
  }
  for(int x82=x80; x82 < x3; x82++) {
    int32_t x83 = x82 + x6;
    float x84 = x0[x83];
    bool x85 = x84 >= x2;
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x85) {
      int32_t x86 = x1;
      int32_t x87 = x86 + x6;
      int32_t x88 = x87 + x3;
      x0[x88] = x82;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
  }
  int32_t x99 = x1;
  printf("%s\n","Number of tuples found: ");
  printf("%d\n",x99);
  bool x103 = x99 == 0;
  if (x103) {
  } else {
    printf("%s\n","Output array: ");
    int32_t x105 = x6 + x3;
    for(int x107=0; x107 < x99; x107++) {
      int32_t x108 = x107 + x105;
      float x109 = x0[x108];
      printf("%f\n",x109);
    }
  }
  //#Scan Variants
}
/*****************************************
End of C Generated Code
*******************************************/
