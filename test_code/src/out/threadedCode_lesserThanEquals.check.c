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
//Here I should declare the functions that I will use for parallelChunk and prefixSum
void Scan(float*  x0) {
  float x7 = x0[2];
  float x5 = x0[1];
  float x11 = x5 / x7;
  float x6 = x0[0];
  int32_t x14 = 2 * x7;
  int32_t x15 = 3 + x14;
  bool x34 = x7 > 0;
  int32_t x59 = x5 / x7;
  //#Scan Variants- timer goes here
  // generated code for Scan Variants- timer goes here
  int32_t x1 = 0;
  int32_t x2 = 1;
  int32_t x3 = 3;
  int32_t x4 = 0;
  //Declare threads and functions:
  pthread_t threads[(int)x7];
  void* parallelPrefixSum(void* input){
    int32_t x9=*(int32_t*)input;
    int32_t x16 = x9 * x5; //X5 needed
    int32_t x17 = x16 / x7; //X7 needed
    int32_t x27 = 3 + x9;
    //#parallel prefix sum
    // generated code for parallel prefix sum
    int32_t x10 = 0;
    for(int x13=0; x13 < x11; x13++) { //x11 needed
      int32_t x18 = x13 + x17; //x15 needed
      int32_t x19 = x15 + x18;
      float x20 = x0[x19]; //x0 needed
      bool x21 = x20 <= x6; //x6 needed
      if (x21) {
        x10 += 1;
      } else {
      }
    }
    int32_t x28 = x10;
    x0[x27] = x28;
    //#parallel prefix sum
  }
  void* parallelChunk(void* input){
    int32_t x57=*(int32_t*)input;
    int32_t x62 = x57 * x5;//x5 needed
    int32_t x63 = x62 / x7; //x7 needed
    int32_t x68 = 3 + x57;
    int32_t x69 = x68 + x7;
    float x70 = x0[x69];//x0 needed
    int32_t x71 = x70 + 3;
    int32_t x72 = x71 + x14;//x14 needed
    int32_t x73 = x72 + x5;
    //#parallel chunk
    // generated code for parallel chunk
    int32_t x58 = 0;
    for(int x61=0; x61 < x59; x61++) {
      int32_t x64 = x61 + x63;
      int32_t x65 = x15 + x64;
      float x66 = x0[x65];
      bool x67 = x66 <= x6;
      if (x67) {
        int32_t x74 = x58;
        int32_t x75 = x73 + x74;
        x0[x75] = x66;
        x58 += 1;
      } else {
      }
    }
    //#parallel chunk 
  }
  int32_t *inputArray;
  inputArray=(int32_t*)malloc(x7*sizeof(int32_t));
  for(int x9=0; x9 < x7; x9++) {
	inputArray[x9]=x9;
	pthread_create(&threads[x9], NULL, parallelPrefixSum, (void *)&inputArray[x9]);
  }
  for(int x9=0; x9 < x7; x9++) {
	pthread_join(threads[x9], NULL);
  }

  if (x34) {
    int32_t x35 = 3 + x7;
    x0[x35] = 0;
    for(int x38=1; x38 < x7; x38++) {
      int32_t x39 = 3 + x38;
      int32_t x40 = x39 + x7;
      int32_t x41 = x39 - 1;
      float x42 = x0[x41];
      int32_t x43 = x41 + x7;
      float x44 = x0[x43];
      float x45 = x42 + x44;
      x0[x40] = x45;
    }
    int32_t x49 = x35 - 1;
    float x50 = x0[x49];
    int32_t x51 = x49 + x7;
    float x52 = x0[x51];
    float x53 = x50 + x52;
    x4 = x53;
  } else {
  }

  for(int x57=0; x57 < x7; x57++) {
   	pthread_create(&threads[x57], NULL, parallelChunk, (void *)&inputArray[x57]); 
  }
  for(int x57=0; x57 < x7; x57++) {
	pthread_join(threads[x57], NULL);
  }
  printf("%s\n","Output array: ");
  int32_t x87 = x4;
  for(int x89=0; x89 < x87; x89++) {
    int32_t x90 = x89 + 3;
    int32_t x91 = x90 + x14;
    int32_t x92 = x91 + x5;
    float x93 = x0[x92];
    printf("%f\n",x93);
  }
  //#Scan Variants- timer goes here
}
/*****************************************
End of C Generated Code
*******************************************/
