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
  float x5 = x0[2];
  float x4 = x0[1];
  int32_t x11 = x4 / x5;
  float x3 = x0[0];
  int32_t x9 = 2 * x5;
  int32_t x10 = 3 + x9;
  bool x30 = x5 > 0;
  int32_t x77 = x11 * x5;
  bool x78 = x77 < x4;
  //#Scan Variants
  // generated code for Scan Variants
  int32_t x1 = 0;
  int32_t x2 = 1;
  pthread_t threads[(int)x5];
  int *inputArray;
  inputArray=(int*)malloc(x5*sizeof(int));
  void* parallelPrefixSum(void* input){
    int x7=*(int*)input;
    int32_t x12 = x11 * x7;
    int32_t x13 = x10 + x12;
    int32_t x23 = 3 + x7;
    //#parallel prefix sum
    // generated code for parallel prefix sum
    int32_t x8 = 0;
    for(int x15=0; x15 < x11; x15++) {
      int32_t x16 = x13 + x15;
      float x17 = x0[x16];
      bool x18 = x17 >= x3;
      x8 += x18;
    }
    int32_t x24 = x8;
    x0[x23] = x24;
    //#parallel prefix sum
  }
  for(int x7=0; x7 < x5; x7++) {
	inputArray[x7]=x7;
	pthread_create(&threads[x7], NULL, parallelPrefixSum, (void *)&inputArray[x7]);
  }
  for(int x7=0; x7 < x5; x7++) {
	pthread_join(threads[x7], NULL);
  }
  if (x30) {
    int32_t x31 = 3 + x5;
    x0[x31] = 0;
    for(int x34=1; x34 < x5; x34++) {
      int32_t x35 = x31 + x34;
      int32_t x36 = 3 + x34;
      int32_t x37 = x36 - 1;
      float x38 = x0[x37];
      int32_t x39 = x35 - 1;
      float x40 = x0[x39];
      float x41 = x38 + x40;
      x0[x35] = x41;
    }
    int32_t x45 = x31 - 1;
    float x46 = x0[x45];
    int32_t x47 = x45 + x5;
    float x48 = x0[x47];
    float x49 = x46 + x48;
    x1 = x49;
  } else {
  }
  void* parallelChunk(void* input){
    int x53=*(int*)input;
    int32_t x59 = 3 + x53;
    int32_t x60 = x59 + x5;
    float x61 = x0[x60];
    int32_t x62 = x61 + x10;
    int32_t x63 = x62 + x4;
    int32_t x56 = x11 * x53;
    //#parallel chunk
    // generated code for parallel chunk
    int32_t x54 = 0;
    for(int x55=0; x55 < x11; x55++) {
      int32_t x64 = x54;
      int32_t x65 = x63 + x64;
      int32_t x57 = x55 + x56;
      int32_t x58 = x10 + x57;
      int32_t x66 = x58 - x10;
      x0[x65] = x66;
      float x68 = x0[x58];
      bool x69 = x68 >= x3;
      x54 += x69;
    }
    //#parallel chunk
  }
  for(int x53=0; x53 < x5; x53++) {
  	pthread_create(&threads[x53], NULL, parallelChunk, (void *)&inputArray[x53]);
  }
  for(int x53=0; x53 < x5; x53++) {
	pthread_join(threads[x53], NULL);
  }
  if (x78) {
    for(int x80=x77; x80 < x4; x80++) {
      int32_t x81 = x80 + x10;
      float x86 = x0[x81];
      bool x87 = x86 >= x3;
      //#decorated instruction
      // generated code for decorated instruction
      //#run instruction without branching
      // generated code for run instruction without branching
      int32_t x82 = x1;
      int32_t x83 = x82 + x10;
      int32_t x84 = x83 + x4;
      x0[x84] = x80;
      x1 += x87;
      //#run instruction without branching
      //#decorated instruction
    }
  } else {
  }
  int32_t x97 = x1;
  printf("%s\n","Number of tuples found: ");
  printf("%d\n",x97);
  bool x101 = x97 == 0;
  if (x101) {
  } else {
    printf("%s\n","Output array: ");
    int32_t x103 = x10 + x4;
    for(int x105=0; x105 < x97; x105++) {
      int32_t x106 = x105 + x103;
      float x107 = x0[x106];
      printf("%f\n",x107);
    }
  }
  //#Scan Variants
}
/*****************************************
End of C Generated Code
*******************************************/
