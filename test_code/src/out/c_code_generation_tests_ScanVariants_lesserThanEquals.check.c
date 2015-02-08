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
  float x5 = x0[1];
  int32_t x7 = x5 / 5;
  float x6 = x0[0];
  int32_t x68 = x7 * 5;
  bool x69 = x68 < x5;
  //#Scan Variants- timer goes here
  // generated code for Scan Variants- timer goes here
  int32_t x1 = 0;
  int32_t x2 = 1;
  int32_t x3 = 3;
  int32_t x4 = 0;
  for(int x9=0; x9 < x7; x9++) {
    int32_t x10 = x9 * 5;
    int32_t x11 = x10 + 3;
    float x12 = x0[x11];
    bool x13 = x12 <= x6;
    int32_t x21 = x10 + 1;
    int32_t x22 = x21 + 3;
    float x23 = x0[x22];
    bool x24 = x23 <= x6;
    int32_t x32 = x10 + 2;
    int32_t x33 = x32 + 3;
    float x34 = x0[x33];
    bool x35 = x34 <= x6;
    int32_t x43 = x11 + 3;
    float x44 = x0[x43];
    bool x45 = x44 <= x6;
    int32_t x53 = x10 + 4;
    int32_t x54 = x53 + 3;
    float x55 = x0[x54];
    bool x56 = x55 <= x6;
    //#run instruction
    // generated code for run instruction
    if (x13) {
      int32_t x14 = x4;
      int32_t x15 = x14 + x5;
      int32_t x16 = x15 + 3;
      x0[x16] = x12;
      x4 += 1;
    } else {
    }
    if (x24) {
      int32_t x25 = x4;
      int32_t x26 = x25 + x5;
      int32_t x27 = x26 + 3;
      x0[x27] = x23;
      x4 += 1;
    } else {
    }
    if (x35) {
      int32_t x36 = x4;
      int32_t x37 = x36 + x5;
      int32_t x38 = x37 + 3;
      x0[x38] = x34;
      x4 += 1;
    } else {
    }
    if (x45) {
      int32_t x46 = x4;
      int32_t x47 = x46 + x5;
      int32_t x48 = x47 + 3;
      x0[x48] = x44;
      x4 += 1;
    } else {
    }
    if (x56) {
      int32_t x57 = x4;
      int32_t x58 = x57 + x5;
      int32_t x59 = x58 + 3;
      x0[x59] = x55;
      x4 += 1;
    } else {
    }
    //#run instruction
  }
  if (x69) {
    int32_t x70 = x68 + 3;
    int32_t x71 = x5 + 3;
    for(int x73=x70; x73 < x71; x73++) {
      float x74 = x0[x73];
      bool x75 = x74 <= x6;
      //#run residue instructions after unroll
      // generated code for run residue instructions after unroll
      if (x75) {
        int32_t x76 = x4;
        int32_t x77 = x76 + x5;
        int32_t x78 = x77 + 3;
        x0[x78] = x74;
        x4 += 1;
      } else {
      }
      //#run residue instructions after unroll
    }
  } else {
  }
  printf("%s\n","Output array: ");
  int32_t x90 = x4;
  for(int x92=0; x92 < x90; x92++) {
    int32_t x93 = x92 + 3;
    int32_t x94 = x93 + x5;
    float x95 = x0[x94];
    printf("%f\n",x95);
  }
  bool x99 = x90 == 0;
  if (x99) {
    printf("%s\n","No results found.");
  } else {
  }
  //#Scan Variants- timer goes here
}
/*****************************************
End of C Generated Code
*******************************************/
