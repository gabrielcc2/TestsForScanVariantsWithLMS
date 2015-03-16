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
  float x7 = x0[2];
  float x5 = x0[1];
  float x11 = x5 / x7;
  float x6 = x0[0];
  int32_t x14 = 2 * x7;
  int32_t x15 = 3 + x14;
  bool x32 = x7 > 0;
  int32_t x57 = x5 / x7;
  //#Scan Variants- timer goes here
  // generated code for Scan Variants- timer goes here
  int32_t x1 = 0;
  int32_t x2 = 1;
  int32_t x3 = 3;
  int32_t x4 = 0;
  pthread_t threads[(int)x7];
  int *inputArray;
  inputArray=(int*)malloc(x7*sizeof(int));
  void* parallelPrefixSum(void* input){
    int x9=*(int*)input;
    int32_t x16 = x9 * x5;
    int32_t x17 = x16 / x7;
    int32_t x25 = 3 + x9;
    //#parallel prefix sum
    // generated code for parallel prefix sum
    int32_t x10 = 0;
    for(int x13=0; x13 < x11; x13++) {
      int32_t x18 = x13 + x17;
      int32_t x19 = x15 + x18;
      float x20 = x0[x19];
      bool x21 = x20 >= x6;
      x10 += x21;
    }
    int32_t x26 = x10;
    x0[x25] = x26;
    //#parallel prefix sum
  }
  for(int x9=0; x9 < x7; x9++) {
	inputArray[x9]=x9;
	pthread_create(&threads[x9], NULL, parallelPrefixSum, (void *)&inputArray[x9]);
  }
  for(int x9=0; x9 < x7; x9++) {
	pthread_join(threads[x9], NULL);
  }
  if (x32) {
    int32_t x33 = 3 + x7;
    x0[x33] = 0;
    for(int x36=1; x36 < x7; x36++) {
      int32_t x37 = 3 + x36;
      int32_t x38 = x37 + x7;
      int32_t x39 = x37 - 1;
      float x40 = x0[x39];
      int32_t x41 = x39 + x7;
      float x42 = x0[x41];
      float x43 = x40 + x42;
      x0[x38] = x43;
    }
    int32_t x47 = x33 - 1;
    float x48 = x0[x47];
    int32_t x49 = x47 + x7;
    float x50 = x0[x49];
    float x51 = x48 + x50;
    x4 = x51;
  } else {
  }
  void* parallelChunk(void* input){
    int x55=*(int*)input;
    int32_t x64 = 3 + x55;
    int32_t x65 = x64 + x7;
    float x66 = x0[x65];
    int32_t x67 = x66 + 3;
    int32_t x68 = x67 + x14;
    int32_t x69 = x68 + x5;
    int32_t x60 = x55 * x5;
    int32_t x61 = x60 / x7;
    //#parallel chunk
    // generated code for parallel chunk
    int32_t x56 = 0;
    for(int x59=0; x59 < x57; x59++) {
      int32_t x70 = x56;
      int32_t x71 = x69 + x70;
      int32_t x62 = x59 + x61;
      int32_t x63 = x15 + x62;
      float x72 = x0[x63];
      x0[x71] = x72;
      bool x74 = x72 >= x6;
      x56 += x74;
    }
    //#parallel chunk
  }
  for(int x55=0; x55 < x7; x55++) {
  	pthread_create(&threads[x55], NULL, parallelChunk, (void *)&inputArray[x55]);
  }
  for(int x55=0; x55 < x7; x55++) {
	pthread_join(threads[x55], NULL);
  }
  printf("%s\n","Output array: ");
  int32_t x83 = x4;
  for(int x85=0; x85 < x83; x85++) {
    int32_t x86 = x85 + 3;
    int32_t x87 = x86 + x14;
    int32_t x88 = x87 + x5;
    float x89 = x0[x88];
    printf("%f\n",x89);
  }
  bool x93 = x83 == 0;
  if (x93) {
    printf("%s\n","No results found.");
  } else {
  }
  //#Scan Variants- timer goes here
}
/*****************************************
End of C Generated Code
*******************************************/
