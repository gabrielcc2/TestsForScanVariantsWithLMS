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
  float x4 = x0[1];
  float x3 = x0[0];
  //#Scan Variants
  // generated code for Scan Variants
  int32_t x1 = 0;
  int32_t x2 = 1;
  for(int x6=0; x6 < x4; x6++) {
    int32_t x7 = x6 + 3;
    float x8 = x0[x7];
    bool x9 = x8 >= x3;
    //#run iteration as in simple loop
    // generated code for run iteration as in simple loop
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x9) {
      int32_t x10 = x1;
      int32_t x11 = x10 + 3;
      int32_t x12 = x11 + x4;
      x0[x12] = x6;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#run iteration as in simple loop
  }
  int32_t x23 = x1;
  printf("%s\n","Number of tuples found: ");
  printf("%d\n",x23);
  bool x27 = x23 == 0;
  if (x27) {
  } else {
    printf("%s\n","Output array: ");
    int32_t x29 = 3 + x4;
    for(int x31=0; x31 < x23; x31++) {
      int32_t x32 = x31 + x29;
      float x33 = x0[x32];
      printf("%f\n",x33);
    }
  }
  //#Scan Variants
}
/*****************************************
End of C Generated Code
*******************************************/
