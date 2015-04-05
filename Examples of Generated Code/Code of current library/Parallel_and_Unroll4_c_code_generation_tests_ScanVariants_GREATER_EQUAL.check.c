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
  bool x52 = x5 > 0;
  int32_t x96 = x7 + 1;
  int32_t x107 = x7 + 2;
  int32_t x118 = x7 + 3;
  int32_t x135 = x11 * 4;
  int32_t x136 = x135 * x5;
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
    int32_t x45 = 3 + x9;
    //#parallel prefix sum
    // generated code for parallel prefix sum
    int32_t x10 = 0;
    for(int x16=0; x16 < x11; x16++) {
      int32_t x17 = x16 * 4;
      int32_t x18 = x14 + x17;
      float x19 = x0[x18];
      bool x20 = x19 >= x2;
      if (x20) {
        x10 += 1;
      } else {
      }
      int32_t x24 = x18 + 1;
      float x25 = x0[x24];
      bool x26 = x25 >= x2;
      if (x26) {
        x10 += 1;
      } else {
      }
      int32_t x30 = x24 + 1;
      float x31 = x0[x30];
      bool x32 = x31 >= x2;
      if (x32) {
        x10 += 1;
      } else {
      }
      int32_t x36 = x30 + 1;
      float x37 = x0[x36];
      bool x38 = x37 >= x2;
      if (x38) {
        x10 += 1;
      } else {
      }
    }
    int32_t x46 = x10;
    x0[x45] = x46;
    //#parallel prefix sum
  }
  for(int x9=0; x9 < x5; x9++) {
	inputArray[x9]=x9;
	pthread_create(&threads[x9], NULL, parallelPrefixSum, (void *)&inputArray[x9]);
  }
  for(int x9=0; x9 < x5; x9++) {
	pthread_join(threads[x9], NULL);
  }
  if (x52) {
    int32_t x53 = 3 + x5;
    x0[x53] = 0;
    for(int x56=1; x56 < x5; x56++) {
      int32_t x57 = x53 + x56;
      int32_t x58 = 3 + x56;
      int32_t x59 = x58 - 1;
      float x60 = x0[x59];
      int32_t x61 = x57 - 1;
      float x62 = x0[x61];
      float x63 = x60 + x62;
      x0[x57] = x63;
    }
    int32_t x67 = x53 - 1;
    float x68 = x0[x67];
    int32_t x69 = x67 + x5;
    float x70 = x0[x69];
    float x71 = x68 + x70;
    x1 = x71;
  } else {
  }
  void* parallelWriting(void* input){
    int x75=*(int*)input;
    int32_t x78 = x11 * x75;
    int32_t x84 = 3 + x75;
    int32_t x85 = x84 + x5;
    float x86 = x0[x85];
    int32_t x87 = x86 + x7;
    int32_t x88 = x87 + x3;
    //#parallel writing
    // generated code for parallel writing
    int32_t x76 = 0;
    for(int x77=0; x77 < x11; x77++) {
      int32_t x79 = x77 + x78;
      int32_t x80 = x79 * 4;
      int32_t x81 = x7 + x80;
      float x82 = x0[x81];
      bool x83 = x82 >= x2;
      if (x83) {
        int32_t x89 = x76;
        int32_t x90 = x88 + x89;
        int32_t x91 = x81 - x7;
        x0[x90] = x91;
        x76 += 1;
      } else {
      }
      int32_t x97 = x96 + x80;
      float x98 = x0[x97];
      bool x99 = x98 >= x2;
      if (x99) {
        int32_t x100 = x76;
        int32_t x101 = x88 + x100;
        int32_t x102 = x97 - x7;
        x0[x101] = x102;
        x76 += 1;
      } else {
      }
      int32_t x108 = x107 + x80;
      float x109 = x0[x108];
      bool x110 = x109 >= x2;
      if (x110) {
        int32_t x111 = x76;
        int32_t x112 = x88 + x111;
        int32_t x113 = x108 - x7;
        x0[x112] = x113;
        x76 += 1;
      } else {
      }
      int32_t x119 = x118 + x80;
      float x120 = x0[x119];
      bool x121 = x120 >= x2;
      if (x121) {
        int32_t x122 = x76;
        int32_t x123 = x88 + x122;
        int32_t x124 = x119 - x7;
        x0[x123] = x124;
        x76 += 1;
      } else {
      }
    }
    //#parallel writing
  }
  for(int x75=0; x75 < x5; x75++) {
  	pthread_create(&threads[x75], NULL, parallelWriting, (void *)&inputArray[x75]);
  }
  for(int x75=0; x75 < x5; x75++) {
	pthread_join(threads[x75], NULL);
  }
  for(int x138=x136; x138 < x3; x138++) {
    int32_t x139 = x138 + x7;
    float x140 = x0[x139];
    bool x141 = x140 >= x2;
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x141) {
      int32_t x142 = x1;
      int32_t x143 = x142 + x7;
      int32_t x144 = x143 + x3;
      x0[x144] = x138;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
  }
  int32_t x155 = x1;
  printf("%s\n","Number of tuples found: ");
  printf("%d\n",x155);
  bool x159 = x155 == 0;
  if (x159) {
  } else {
    printf("%s\n","Output array: ");
    int32_t x161 = x7 + x3;
    for(int x163=0; x163 < x155; x163++) {
      int32_t x164 = x163 + x161;
      float x165 = x0[x164];
      printf("%f\n",x165);
    }
  }
  //#Scan Variants
}
/*****************************************
End of C Generated Code
*******************************************/
