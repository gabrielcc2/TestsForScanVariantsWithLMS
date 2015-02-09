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
  int32_t x7 = x5 / 4;
  float x6 = x0[0];
  int32_t x57 = x7 * 4;
  bool x58 = x57 < x5;
  //#Scan Variants- timer goes here
  // generated code for Scan Variants- timer goes here
  int32_t x1 = 0;
  int32_t x2 = 1;
  int32_t x3 = 3;
  int32_t x4 = 0;
  for(int x9=0; x9 < x7; x9++) {
    int32_t x10 = x9 * 4;
    int32_t x11 = x10 + 3;
    float x12 = x0[x11];
    bool x13 = x12 >= x6;
    int32_t x21 = x10 + 1;
    int32_t x22 = x21 + 3;
    float x23 = x0[x22];
    bool x24 = x23 >= x6;
    int32_t x32 = x10 + 2;
    int32_t x33 = x32 + 3;
    float x34 = x0[x33];
    bool x35 = x34 >= x6;
    int32_t x43 = x11 + 3;
    float x44 = x0[x43];
    bool x45 = x44 >= x6;
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
    //#run instruction
  }
  if (x58) {
    int32_t x59 = x57 + 3;
    int32_t x60 = x5 + 3;
    for(int x62=x59; x62 < x60; x62++) {
      float x63 = x0[x62];
      bool x64 = x63 >= x6;
      //#run residue instructions after unroll
      // generated code for run residue instructions after unroll
      if (x64) {
        int32_t x65 = x4;
        int32_t x66 = x65 + x5;
        int32_t x67 = x66 + 3;
        x0[x67] = x63;
        x4 += 1;
      } else {
      }
      //#run residue instructions after unroll
    }
  } else {
  }
  printf("%s\n","Output array: ");
  int32_t x79 = x4;
  for(int x81=0; x81 < x79; x81++) {
    int32_t x82 = x81 + 3;
    int32_t x83 = x82 + x5;
    float x84 = x0[x83];
    printf("%f\n",x84);
  }
  bool x88 = x79 == 0;
  if (x88) {
    printf("%s\n","No results found.");
  } else {
  }
  //#Scan Variants- timer goes here
}
/*****************************************
End of C Generated Code
*******************************************/
