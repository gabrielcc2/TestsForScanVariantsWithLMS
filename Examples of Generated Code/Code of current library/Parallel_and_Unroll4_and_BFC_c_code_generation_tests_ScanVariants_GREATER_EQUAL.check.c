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
  float x3 = x0[1];
  float x4 = x3 / 4.0f;
  int32_t x11 = x4 / x5;
  float x2 = x0[0];
  int32_t x6 = 2 * x5;
  int32_t x7 = 3 + x6;
  bool x44 = x5 > 0;
  int32_t x86 = x7 + 1;
  int32_t x95 = x7 + 2;
  int32_t x104 = x7 + 3;
  int32_t x119 = x11 * 4;
  int32_t x120 = x119 * x5;
  //#Scan Variants
  // generated code for Scan Variants
  int32_t x1 = 0;
  pthread_t threads[(int)x5];
  int *inputArray;
  inputArray=(int*)malloc(x5*sizeof(int));
  void* parallelPrefixSum(void* input){
    int x9=*(int*)input;
    int32_t x12 = x11 * x9;
    int32_t x13 = 4 * x12;
    int32_t x14 = x7 + x13;
    int32_t x37 = 3 + x9;
    //#parallel prefix sum
    // generated code for parallel prefix sum
    int32_t x10 = 0;
    for(int x16=0; x16 < x11; x16++) {
      int32_t x17 = x16 * 4;
      int32_t x18 = x14 + x17;
      float x19 = x0[x18];
      bool x20 = x19 >= x2;
      x10 += x20;
      int32_t x22 = x18 + 1;
      float x23 = x0[x22];
      bool x24 = x23 >= x2;
      x10 += x24;
      int32_t x26 = x22 + 1;
      float x27 = x0[x26];
      bool x28 = x27 >= x2;
      x10 += x28;
      int32_t x30 = x26 + 1;
      float x31 = x0[x30];
      bool x32 = x31 >= x2;
      x10 += x32;
    }
    int32_t x38 = x10;
    x0[x37] = x38;
    //#parallel prefix sum
  }
  for(int x9=0; x9 < x5; x9++) {
	inputArray[x9]=x9;
	pthread_create(&threads[x9], NULL, parallelPrefixSum, (void *)&inputArray[x9]);
  }
  for(int x9=0; x9 < x5; x9++) {
	pthread_join(threads[x9], NULL);
  }
  if (x44) {
    int32_t x45 = 3 + x5;
    x0[x45] = 0;
    for(int x48=1; x48 < x5; x48++) {
      int32_t x49 = x45 + x48;
      int32_t x50 = 3 + x48;
      int32_t x51 = x50 - 1;
      float x52 = x0[x51];
      int32_t x53 = x49 - 1;
      float x54 = x0[x53];
      float x55 = x52 + x54;
      x0[x49] = x55;
    }
    int32_t x59 = x45 - 1;
    float x60 = x0[x59];
    int32_t x61 = x59 + x5;
    float x62 = x0[x61];
    float x63 = x60 + x62;
    x1 = x63;
  } else {
  }
  void* parallelWriting(void* input){
    int x67=*(int*)input;
    int32_t x74 = 3 + x67;
    int32_t x75 = x74 + x5;
    float x76 = x0[x75];
    int32_t x77 = x76 + x7;
    int32_t x78 = x77 + x3;
    int32_t x70 = x11 * x67;
    //#parallel writing
    // generated code for parallel writing
    int32_t x68 = 0;
    for(int x69=0; x69 < x11; x69++) {
      int32_t x79 = x68;
      int32_t x80 = x78 + x79;
      int32_t x71 = x69 + x70;
      int32_t x72 = x71 * 4;
      int32_t x73 = x7 + x72;
      int32_t x81 = x73 - x7;
      x0[x80] = x81;
      float x83 = x0[x73];
      bool x84 = x83 >= x2;
      x68 += x84;
      int32_t x88 = x68;
      int32_t x89 = x78 + x88;
      int32_t x87 = x86 + x72;
      int32_t x90 = x87 - x7;
      x0[x89] = x90;
      float x92 = x0[x87];
      bool x93 = x92 >= x2;
      x68 += x93;
      int32_t x97 = x68;
      int32_t x98 = x78 + x97;
      int32_t x96 = x95 + x72;
      int32_t x99 = x96 - x7;
      x0[x98] = x99;
      float x101 = x0[x96];
      bool x102 = x101 >= x2;
      x68 += x102;
      int32_t x106 = x68;
      int32_t x107 = x78 + x106;
      int32_t x105 = x104 + x72;
      int32_t x108 = x105 - x7;
      x0[x107] = x108;
      float x110 = x0[x105];
      bool x111 = x110 >= x2;
      x68 += x111;
    }
    //#parallel writing
  }
  for(int x67=0; x67 < x5; x67++) {
  	pthread_create(&threads[x67], NULL, parallelWriting, (void *)&inputArray[x67]);
  }
  for(int x67=0; x67 < x5; x67++) {
	pthread_join(threads[x67], NULL);
  }
  for(int x122=x120; x122 < x3; x122++) {
    int32_t x123 = x122 + x7;
    float x128 = x0[x123];
    bool x129 = x128 >= x2;
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction without branching
    // generated code for run instruction without branching
    int32_t x124 = x1;
    int32_t x125 = x124 + x7;
    int32_t x126 = x125 + x3;
    x0[x126] = x122;
    x1 += x129;
    //#run instruction without branching
    //#decorated instruction
  }
  int32_t x137 = x1;
  printf("%s\n","Number of tuples found: ");
  printf("%d\n",x137);
  bool x141 = x137 == 0;
  if (x141) {
  } else {
    printf("%s\n","Output array: ");
    int32_t x143 = x7 + x3;
    for(int x145=0; x145 < x137; x145++) {
      int32_t x146 = x145 + x143;
      float x147 = x0[x146];
      printf("%f\n",x147);
    }
  }
  //#Scan Variants
}
/*****************************************
End of C Generated Code
*******************************************/
