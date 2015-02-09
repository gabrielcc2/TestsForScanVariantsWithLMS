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
  float x5 = x0[1];
  int32_t x7 = x5 / 4;
  float x6 = x0[0];
  int32_t x49 = x7 * 4;
  bool x50 = x49 < x5;
  //#Scan Variants- timer goes here
  // generated code for Scan Variants- timer goes here
  int32_t x1 = 0;
  int32_t x2 = 1;
  int32_t x3 = 3;
  int32_t x4 = 0;
  for(int x9=0; x9 < x7; x9++) {
    int32_t x10 = x9 * 4;
    int32_t x11 = x10 + 3;
    float x15 = x0[x11];
    bool x17 = x15 >= x6;
    int32_t x19 = x10 + 1;
    int32_t x20 = x19 + 3;
    float x24 = x0[x20];
    bool x26 = x24 >= x6;
    int32_t x28 = x10 + 2;
    int32_t x29 = x28 + 3;
    float x33 = x0[x29];
    bool x35 = x33 >= x6;
    int32_t x37 = x11 + 3;
    float x41 = x0[x37];
    bool x43 = x41 >= x6;
    //#run instruction
    // generated code for run instruction
    int32_t x12 = x4;
    int32_t x13 = x12 + x5;
    int32_t x14 = x13 + 3;
    x0[x14] = x15;
    x4 += x17;
    int32_t x21 = x4;
    int32_t x22 = x21 + x5;
    int32_t x23 = x22 + 3;
    x0[x23] = x24;
    x4 += x26;
    int32_t x30 = x4;
    int32_t x31 = x30 + x5;
    int32_t x32 = x31 + 3;
    x0[x32] = x33;
    x4 += x35;
    int32_t x38 = x4;
    int32_t x39 = x38 + x5;
    int32_t x40 = x39 + 3;
    x0[x40] = x41;
    x4 += x43;
    //#run instruction
  }
  if (x50) {
    int32_t x51 = x49 + 3;
    int32_t x52 = x5 + 3;
    for(int x54=x51; x54 < x52; x54++) {
      float x58 = x0[x54];
      bool x60 = x58 >= x6;
      //#run residue instructions after unroll
      // generated code for run residue instructions after unroll
      int32_t x55 = x4;
      int32_t x56 = x55 + x5;
      int32_t x57 = x56 + 3;
      x0[x57] = x58;
      x4 += x60;
      //#run residue instructions after unroll
    }
  } else {
  }
  printf("%s\n","Output array: ");
  int32_t x69 = x4;
  for(int x71=0; x71 < x69; x71++) {
    int32_t x72 = x71 + 3;
    int32_t x73 = x72 + x5;
    float x74 = x0[x73];
    printf("%f\n",x74);
  }
  bool x78 = x69 == 0;
  if (x78) {
    printf("%s\n","No results found.");
  } else {
  }
  //#Scan Variants- timer goes here
}
/*****************************************
End of C Generated Code
*******************************************/
