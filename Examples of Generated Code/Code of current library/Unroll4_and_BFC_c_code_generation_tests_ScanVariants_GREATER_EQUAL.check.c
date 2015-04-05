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
  float x4 = x3 / 4.0f;
  float x2 = x0[0];
  int32_t x55 = x4 * 4;
  //#Scan Variants
  // generated code for Scan Variants
  int32_t x1 = 0;
  for(int x6=0; x6 < x4; x6++) {
    int32_t x7 = x6 * 4;
    int32_t x8 = x7 + 3;
    float x13 = x0[x8];
    bool x14 = x13 >= x2;
    int32_t x18 = 1 + x7;
    int32_t x19 = x18 + 3;
    float x24 = x0[x19];
    bool x25 = x24 >= x2;
    int32_t x29 = 2 + x7;
    int32_t x30 = x29 + 3;
    float x35 = x0[x30];
    bool x36 = x35 >= x2;
    int32_t x40 = 3 + x7;
    int32_t x41 = x40 + 3;
    float x46 = x0[x41];
    bool x47 = x46 >= x2;
    //#run iteration from unrolled loop
    // generated code for run iteration from unrolled loop
    //#run instruction without branching
    // generated code for run instruction without branching
    int32_t x9 = x1;
    int32_t x10 = x9 + 3;
    int32_t x11 = x10 + x3;
    x0[x11] = x7;
    x1 += x14;
    //#run instruction without branching
    //#run instruction without branching
    // generated code for run instruction without branching
    int32_t x20 = x1;
    int32_t x21 = x20 + 3;
    int32_t x22 = x21 + x3;
    x0[x22] = x18;
    x1 += x25;
    //#run instruction without branching
    //#run instruction without branching
    // generated code for run instruction without branching
    int32_t x31 = x1;
    int32_t x32 = x31 + 3;
    int32_t x33 = x32 + x3;
    x0[x33] = x29;
    x1 += x36;
    //#run instruction without branching
    //#run instruction without branching
    // generated code for run instruction without branching
    int32_t x42 = x1;
    int32_t x43 = x42 + 3;
    int32_t x44 = x43 + x3;
    x0[x44] = x40;
    x1 += x47;
    //#run instruction without branching
    //#run iteration from unrolled loop
  }
  for(int x57=x55; x57 < x3; x57++) {
    int32_t x58 = x57 + 3;
    float x63 = x0[x58];
    bool x64 = x63 >= x2;
    //#run instruction without branching
    // generated code for run instruction without branching
    int32_t x59 = x1;
    int32_t x60 = x59 + 3;
    int32_t x61 = x60 + x3;
    x0[x61] = x57;
    x1 += x64;
    //#run instruction without branching
  }
  int32_t x70 = x1;
  printf("%s\n","Number of tuples found: ");
  printf("%d\n",x70);
  bool x74 = x70 == 0;
  if (x74) {
  } else {
    printf("%s\n","Output array: ");
    int32_t x76 = 3 + x3;
    for(int x78=0; x78 < x70; x78++) {
      int32_t x79 = x78 + x76;
      float x80 = x0[x79];
      printf("%f\n",x80);
    }
  }
  //#Scan Variants
}
/*****************************************
End of C Generated Code
*******************************************/
