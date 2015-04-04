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
  bool x32 = x5 > 0;
  int32_t x81 = x11 * x5;
  bool x82 = x81 < x4;
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
    int32_t x25 = 3 + x7;
    //#parallel prefix sum
    // generated code for parallel prefix sum
    int32_t x8 = 0;
    for(int x15=0; x15 < x11; x15++) {
      int32_t x16 = x13 + x15;
      float x17 = x0[x16];
      bool x18 = x17 >= x3;
      if (x18) {
        x8 += 1;
      } else {
      }
    }
    int32_t x26 = x8;
    x0[x25] = x26;
    //#parallel prefix sum
  }
  for(int x7=0; x7 < x5; x7++) {
	inputArray[x7]=x7;
	pthread_create(&threads[x7], NULL, parallelPrefixSum, (void *)&inputArray[x7]);
  }
  for(int x7=0; x7 < x5; x7++) {
	pthread_join(threads[x7], NULL);
  }
  if (x32) {
    int32_t x33 = 3 + x5;
    x0[x33] = 0;
    for(int x36=1; x36 < x5; x36++) {
      int32_t x37 = x33 + x36;
      int32_t x38 = 3 + x36;
      int32_t x39 = x38 - 1;
      float x40 = x0[x39];
      int32_t x41 = x37 - 1;
      float x42 = x0[x41];
      float x43 = x40 + x42;
      x0[x37] = x43;
    }
    int32_t x47 = x33 - 1;
    float x48 = x0[x47];
    int32_t x49 = x47 + x5;
    float x50 = x0[x49];
    float x51 = x48 + x50;
    x1 = x51;
  } else {
  }
  void* parallelWriting(void* input){
    int x55=*(int*)input;
    int32_t x58 = x11 * x55;
    int32_t x63 = 3 + x55;
    int32_t x64 = x63 + x5;
    float x65 = x0[x64];
    int32_t x66 = x65 + x10;
    int32_t x67 = x66 + x4;
    //#parallel writing
    // generated code for parallel writing
    int32_t x56 = 0;
    for(int x57=0; x57 < x11; x57++) {
      int32_t x59 = x57 + x58;
      int32_t x60 = x10 + x59;
      float x61 = x0[x60];
      bool x62 = x61 >= x3;
      if (x62) {
        int32_t x68 = x56;
        int32_t x69 = x67 + x68;
        int32_t x70 = x60 - x10;
        x0[x69] = x70;
        x56 += 1;
      } else {
      }
    }
    //#parallel writing
  }
  for(int x55=0; x55 < x5; x55++) {
  	pthread_create(&threads[x55], NULL, parallelWriting, (void *)&inputArray[x55]);
  }
  for(int x55=0; x55 < x5; x55++) {
	pthread_join(threads[x55], NULL);
  }
  if (x82) {
    for(int x84=x81; x84 < x4; x84++) {
      int32_t x85 = x84 + x10;
      float x86 = x0[x85];
      bool x87 = x86 >= x3;
      //#decorated instruction
      // generated code for decorated instruction
      //#run instruction with branching
      // generated code for run instruction with branching
      if (x87) {
        int32_t x88 = x1;
        int32_t x89 = x88 + x10;
        int32_t x90 = x89 + x4;
        x0[x90] = x84;
        x1 += 1;
      } else {
      }
      //#run instruction with branching
      //#decorated instruction
    }
  } else {
  }
  int32_t x103 = x1;
  printf("%s\n","Number of tuples found: ");
  printf("%d\n",x103);
  bool x107 = x103 == 0;
  if (x107) {
  } else {
    printf("%s\n","Output array: ");
    int32_t x109 = x10 + x4;
    for(int x111=0; x111 < x103; x111++) {
      int32_t x112 = x111 + x109;
      float x113 = x0[x112];
      printf("%f\n",x113);
    }
  }
  //#Scan Variants
}
/*****************************************
End of C Generated Code
*******************************************/
