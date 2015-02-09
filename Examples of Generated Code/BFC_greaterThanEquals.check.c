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
  float x6 = x0[0];
  //#Scan Variants- timer goes here
  // generated code for Scan Variants- timer goes here
  int32_t x1 = 0;
  int32_t x2 = 1;
  int32_t x3 = 3;
  int32_t x4 = 0;
  for(int x8=0; x8 < x5; x8++) {
    int32_t x9 = x8 + 3;
    float x13 = x0[x9];
    bool x15 = x13 >= x6;
    //#run instruction
    // generated code for run instruction
    int32_t x10 = x4;
    int32_t x11 = x10 + x5;
    int32_t x12 = x11 + 3;
    x0[x12] = x13;
    x4 += x15;
    //#run instruction
  }
  printf("%s\n","Output array: ");
  int32_t x22 = x4;
  for(int x24=0; x24 < x22; x24++) {
    int32_t x25 = x24 + 3;
    int32_t x26 = x25 + x5;
    float x27 = x0[x26];
    printf("%f\n",x27);
  }
  bool x31 = x22 == 0;
  if (x31) {
    printf("%s\n","No results found.");
  } else {
  }
  //#Scan Variants- timer goes here
}
/*****************************************
End of C Generated Code
*******************************************/
