#include <fcntl.h>
#include <errno.h>
#include <err.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <emmintrin.h>
#include <mmintrin.h>
void Scan(float*);
int main(int argc, char *argv[])
{
  if (argc < 4) {
    printf("Missing arguments. Usage: filename numberOfTuples compareValue\n");
    return 0;
  }
  FILE *ptr_file;
  char buf[1000];
  int numTuples=atoi(argv[2]);
  float compareValue=atof(argv[3]);
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
  float *array;
  array=(float*)malloc(((2*numTuples)+3)*sizeof(float));
  array[0]=compareValue;
  array[1]=(float)numTuples;
  array[2]=(float)0; //Unused
  while (fgets(buf,1000, ptr_file)!=NULL && numReadTuples<numTuples){
    array[numReadTuples+3]=atof(buf);
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
  float x3 = x0[1];
  float x2 = x0[0];
  //#Scan Variants
  // generated code for Scan Variants
  int32_t x1 = 0;
  for(int x5=0; x5 < x3; x5++) {
    int32_t x6 = x5 + 3;
    float x7 = x0[x6];
    bool x8 = x7 >= x2;
    //#run iteration as in simple loop
    // generated code for run iteration as in simple loop
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x8) {
      int32_t x9 = x1;
      int32_t x10 = x9 + 3;
      int32_t x11 = x10 + x3;
      x0[x11] = x5;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#run iteration as in simple loop
  }
  int32_t x22 = x1;
  printf("%s\n","Number of tuples found: ");
  printf("%d\n",x22);
  bool x26 = x22 == 0;
  if (x26) {
  } else {
    printf("%s\n","Output array: ");
    int32_t x28 = 3 + x3;
    for(int x30=0; x30 < x22; x30++) {
      int32_t x31 = x30 + x28;
      float x32 = x0[x31];
      printf("%f\n",x32);
    }
  }
  //#Scan Variants
}
/*****************************************
End of C Generated Code
*******************************************/
