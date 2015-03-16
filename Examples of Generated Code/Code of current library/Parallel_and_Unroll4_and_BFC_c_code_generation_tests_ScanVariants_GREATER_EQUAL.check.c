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
  bool x45 = x6 > 0;
  int32_t x87 = x11 + 1;
  int32_t x96 = x11 + 2;
  int32_t x105 = x11 + 3;
  int32_t x120 = x12 * 4;
  int32_t x121 = x120 * x6;
  bool x122 = x121 < x4;
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
    int32_t x38 = 3 + x8;
    //#parallel prefix sum
    // generated code for parallel prefix sum
    int32_t x9 = 0;
    for(int x17=0; x17 < x12; x17++) {
      int32_t x18 = x17 * 4;
      int32_t x19 = x15 + x18;
      float x20 = x0[x19];
      bool x21 = x20 >= x3;
      x9 += x21;
      int32_t x23 = x19 + 1;
      float x24 = x0[x23];
      bool x25 = x24 >= x3;
      x9 += x25;
      int32_t x27 = x23 + 1;
      float x28 = x0[x27];
      bool x29 = x28 >= x3;
      x9 += x29;
      int32_t x31 = x27 + 1;
      float x32 = x0[x31];
      bool x33 = x32 >= x3;
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
    x1 = x64;
  } else {
  }
  void* parallelChunk(void* input){
    int x68=*(int*)input;
    int32_t x75 = 3 + x68;
    int32_t x76 = x75 + x6;
    float x77 = x0[x76];
    int32_t x78 = x77 + x11;
    int32_t x79 = x78 + x4;
    int32_t x71 = x12 * x68;
    //#parallel chunk
    // generated code for parallel chunk
    int32_t x69 = 0;
    for(int x70=0; x70 < x12; x70++) {
      int32_t x80 = x69;
      int32_t x81 = x79 + x80;
      int32_t x72 = x70 + x71;
      int32_t x73 = x72 * 4;
      int32_t x74 = x11 + x73;
      int32_t x82 = x74 - x11;
      x0[x81] = x82;
      float x84 = x0[x74];
      bool x85 = x84 >= x3;
      x69 += x85;
      int32_t x89 = x69;
      int32_t x90 = x79 + x89;
      int32_t x88 = x87 + x73;
      int32_t x91 = x88 - x11;
      x0[x90] = x91;
      float x93 = x0[x88];
      bool x94 = x93 >= x3;
      x69 += x94;
      int32_t x98 = x69;
      int32_t x99 = x79 + x98;
      int32_t x97 = x96 + x73;
      int32_t x100 = x97 - x11;
      x0[x99] = x100;
      float x102 = x0[x97];
      bool x103 = x102 >= x3;
      x69 += x103;
      int32_t x107 = x69;
      int32_t x108 = x79 + x107;
      int32_t x106 = x105 + x73;
      int32_t x109 = x106 - x11;
      x0[x108] = x109;
      float x111 = x0[x106];
      bool x112 = x111 >= x3;
      x69 += x112;
    }
    //#parallel chunk
  }
  for(int x68=0; x68 < x6; x68++) {
  	pthread_create(&threads[x68], NULL, parallelChunk, (void *)&inputArray[x68]);
  }
  for(int x68=0; x68 < x6; x68++) {
	pthread_join(threads[x68], NULL);
  }
  if (x122) {
    for(int x124=x121; x124 < x4; x124++) {
      int32_t x125 = x124 + x11;
      float x130 = x0[x125];
      bool x131 = x130 >= x3;
      //#decorated instruction
      // generated code for decorated instruction
      //#decorated instruction
      // generated code for decorated instruction
      //#run instruction without branching
      // generated code for run instruction without branching
      int32_t x126 = x1;
      int32_t x127 = x126 + x11;
      int32_t x128 = x127 + x4;
      x0[x128] = x124;
      x1 += x131;
      //#run instruction without branching
      //#decorated instruction
      //#decorated instruction
    }
  } else {
  }
  int32_t x143 = x1;
  printf("%s\n","Number of tuples found: ");
  printf("%d\n",x143);
  bool x147 = x143 == 0;
  if (x147) {
  } else {
    printf("%s\n","Output array: ");
    int32_t x149 = x11 + x4;
    for(int x151=0; x151 < x143; x151++) {
      int32_t x152 = x151 + x149;
      float x153 = x0[x152];
      printf("%f\n",x153);
    }
  }
  //#Scan Variants
}
/*****************************************
End of C Generated Code
*******************************************/
