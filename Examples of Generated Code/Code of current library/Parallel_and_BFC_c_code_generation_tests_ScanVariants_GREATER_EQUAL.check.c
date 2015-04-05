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
  bool x29 = x4 > 0;
  int32_t x76 = x10 * x4;
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
    int32_t x22 = 3 + x8;
    //#parallel prefix sum
    // generated code for parallel prefix sum
    int32_t x9 = 0;
    for(int x14=0; x14 < x10; x14++) {
      int32_t x15 = x12 + x14;
      float x16 = x0[x15];
      bool x17 = x16 >= x2;
      x9 += x17;
    }
    int32_t x23 = x9;
    x0[x22] = x23;
    //#parallel prefix sum
  }
  for(int x8=0; x8 < x4; x8++) {
	inputArray[x8]=x8;
	pthread_create(&threads[x8], NULL, parallelPrefixSum, (void *)&inputArray[x8]);
  }
  for(int x8=0; x8 < x4; x8++) {
	pthread_join(threads[x8], NULL);
  }
  if (x29) {
    int32_t x30 = 3 + x4;
    x0[x30] = 0;
    for(int x33=1; x33 < x4; x33++) {
      int32_t x34 = x30 + x33;
      int32_t x35 = 3 + x33;
      int32_t x36 = x35 - 1;
      float x37 = x0[x36];
      int32_t x38 = x34 - 1;
      float x39 = x0[x38];
      float x40 = x37 + x39;
      x0[x34] = x40;
    }
    int32_t x44 = x30 - 1;
    float x45 = x0[x44];
    int32_t x46 = x44 + x4;
    float x47 = x0[x46];
    float x48 = x45 + x47;
    x1 = x48;
  } else {
  }
  void* parallelWriting(void* input){
    int x52=*(int*)input;
    int32_t x58 = 3 + x52;
    int32_t x59 = x58 + x4;
    float x60 = x0[x59];
    int32_t x61 = x60 + x6;
    int32_t x62 = x61 + x3;
    int32_t x55 = x10 * x52;
    //#parallel writing
    // generated code for parallel writing
    int32_t x53 = 0;
    for(int x54=0; x54 < x10; x54++) {
      int32_t x63 = x53;
      int32_t x64 = x62 + x63;
      int32_t x56 = x54 + x55;
      int32_t x57 = x6 + x56;
      int32_t x65 = x57 - x6;
      x0[x64] = x65;
      float x67 = x0[x57];
      bool x68 = x67 >= x2;
      x53 += x68;
    }
    //#parallel writing
  }
  for(int x52=0; x52 < x4; x52++) {
  	pthread_create(&threads[x52], NULL, parallelWriting, (void *)&inputArray[x52]);
  }
  for(int x52=0; x52 < x4; x52++) {
	pthread_join(threads[x52], NULL);
  }
  for(int x78=x76; x78 < x3; x78++) {
    int32_t x79 = x78 + x6;
    float x84 = x0[x79];
    bool x85 = x84 >= x2;
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction without branching
    // generated code for run instruction without branching
    int32_t x80 = x1;
    int32_t x81 = x80 + x6;
    int32_t x82 = x81 + x3;
    x0[x82] = x78;
    x1 += x85;
    //#run instruction without branching
    //#decorated instruction
  }
  int32_t x93 = x1;
  printf("%s\n","Number of tuples found: ");
  printf("%d\n",x93);
  bool x97 = x93 == 0;
  if (x97) {
  } else {
    printf("%s\n","Output array: ");
    int32_t x99 = x6 + x3;
    for(int x101=0; x101 < x93; x101++) {
      int32_t x102 = x101 + x99;
      float x103 = x0[x102];
      printf("%f\n",x103);
    }
  }
  //#Scan Variants
}
/*****************************************
End of C Generated Code
*******************************************/
