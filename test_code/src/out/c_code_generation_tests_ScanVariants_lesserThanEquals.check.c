#include <fcntl.h>
#include <errno.h>
#include <err.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
void Scan(float*);
int main(int argc, char *argv[])
{
  if (argc < 4) {
    printf("Missing arguments. Usage: filename numberOfTuples compareValue numThreads (min=1)\n");
    return 0;
  }
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
  array=(float*)malloc(((2*numTuples)+3+numThreads)*sizeof(float));
  array[0]=compareValue;
  array[1]=(float)numTuples;
  array[2]=(float)numThreads;
  for (int i=0; i<numThreads; i++){
    array[3+i]=(float)0;
  }
  while (fgets(buf,1000, ptr_file)!=NULL && numReadTuples<numTuples){
    array[numReadTuples+3+numThreads]=atof(buf);
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
  float x7 = x0[2];
  float x5 = x0[1];
  float x17 = x5 / x7;
  float x6 = x0[0];
  int32_t x20 = 3 + x7;
  int32_t x43 = x5 * 2;
  int32_t x44 = x43 + 3;
  int32_t x45 = x44 + x7;
  //#Scan Variants- timer goes here
  // generated code for Scan Variants- timer goes here
  int32_t x1 = 0;
  int32_t x2 = 1;
  int32_t x3 = 3;
  int32_t x4 = 3;
  for(int x9=0; x9 < x7; x9++) {
    int32_t x11 = x1;
    int32_t x10 = 3 + x9;
    x0[x10] = x11;
  }
  for(int x15=0; x15 < x7; x15++) {
    int32_t x21 = x15 * x5;
    int32_t x22 = x21 / x7;
    int32_t x28 = 3 + x15;
    float x29 = x0[x28];
    float x30 = x29 + 1.0f;
    //#parallel prefix sum
    // generated code for parallel prefix sum
    int32_t x16 = x1;
    for(int x19=0; x19 < x17; x19++) {
      int32_t x23 = x19 + x22;
      int32_t x24 = x20 + x23;
      float x25 = x0[x24];
      bool x26 = x25 <= x6;
      if (x26) {
        x0[x28] = x30;
      } else {
      }
    }
    int32_t x27 = x16 + 1;
    int32_t x34 = x16 + x27;
    x0[x28] = x34;
    //#parallel prefix sum
  }
  printf("%s\n","Output array: ");
  for(int x47=0; x47 < x45; x47++) {
    float x48 = x0[x47];
    printf("%f\n",x48);
  }
  //#Scan Variants- timer goes here
}
/*****************************************
End of C Generated Code
*******************************************/
