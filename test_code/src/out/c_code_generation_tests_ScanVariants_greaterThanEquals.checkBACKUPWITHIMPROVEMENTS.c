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
  if (argc < 5) {
    printf("Missing arguments. Usage: filename numberOfTuples compareValue numThreads\n");
    return 0;
  }
  printf("argc: %i\n", argc); 
  printf("Usage: filename numberOfTuples compareValue numThreads\n");
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
  float x6 = x0[2];
  float x3 = x0[1];
  int32_t x5 = x3 / 4;
  int32_t x16 = x5 / x6;
  float x4 = x0[0];
  int32_t x10 = 2 * x6;
  int32_t x11 = 3 + x10;
  bool x46 = x6 > 0;
  int32_t x91 = x11 + 1;
  int32_t x101 = x11 + 2;
  int32_t x111 = x11 + 3;
  int32_t x127 = x16 * 4;
  int32_t x128 = x127 * x6;
  bool x129 = x128 < x3;
  int32_t x156 = x11 + x3;
  //#Scan Variants- timer goes here
  // generated code for Scan Variants- timer goes here
  int32_t x1 = 0;
  int32_t x2 = 0;
  pthread_t threads[(int)x6];
  int *inputArray;
  inputArray=(int*)malloc(x6*sizeof(int));
  void* parallelPrefixSum(void* input){
    int x8=*(int*)input;
    int32_t x12 = x8 * x5;
    int32_t x13 = x8*(int)((float)x5 / x6);
    int32_t x14 = x13 * 4;
    int32_t x15 = x11 + x14;
    int32_t x39 = 3 + x8;
    //#parallel prefix sum
    // generated code for parallel prefix sum
    int32_t x9 = 0;
    for(int x18=0; x18 < x16; x18++) {
      int32_t x19 = x18 * 4;
      int32_t x20 = x15 + x19;
      float x21 = x0[x20];
      bool x22 = x21 >= x4;
      x9 += x22;
      int32_t x24 = x20 + 1;
      float x25 = x0[x24];
      bool x26 = x25 >= x4;
      x9 += x26;
      int32_t x28 = x24 + 1;
      float x29 = x0[x28];
      bool x30 = x29 >= x4;
      x9 += x30;
      int32_t x32 = x28 + 1;
      float x33 = x0[x32];
      bool x34 = x33 >= x4;
      x9 += x34;
      if (x22){
	printf("Thread %i found %f\n", x8, x21);
      }
      if (x26){
	printf("Thread %i found %f\n", x8, x25);
      }
      if (x30){
	printf("Thread %i found %f\n", x8, x29);
      }
      if (x34){
	printf("Thread %i found %f\n", x8, x33);
      }
    }
    int32_t x40 = x9;
    x0[x39] = x40;
    //#parallel prefix sum
  }
  for(int x8=0; x8 < x6; x8++) {
	inputArray[x8]=x8;
	pthread_create(&threads[x8], NULL, parallelPrefixSum, (void *)&inputArray[x8]);
  }
  for(int x8=0; x8 < x6; x8++) {
	pthread_join(threads[x8], NULL);
  }
  if (x46) {
    int32_t x47 = 3 + x6;
    x0[x47] = 0;
    for(int x50=1; x50 < x6; x50++) {
      int32_t x51 = x47 + x50;
      int32_t x52 = 3 + x50;
      int32_t x53 = x52 - 1;
      float x54 = x0[x53];
      int32_t x55 = x51 - 1;
      float x56 = x0[x55];
      float x57 = x54 + x56;
      x0[x51] = x57;
    }
    int32_t x61 = x47 - 1;
    float x62 = x0[x61];
    int32_t x63 = x61 + x6;
    float x64 = x0[x63];
    float x65 = x62 + x64;
    x2 = x65;
  } else {
  }
  void* parallelChunk(void* input){
    int x69=*(int*)input;
    int32_t x77 = 3 + x69;
    int32_t x78 = x77 + x6;
    float x79 = x0[x78];
    int32_t x80 = x79 + 3;
    int32_t x81 = x80 + x10;
    int32_t x82 = x81 + x3;
    int32_t x72 = x69 *  x5;
    int32_t x73 = x69*(int)((float) x5 / x6);
    //#parallel chunk
    // generated code for parallel chunk
    int32_t x70 = 0;
    printf("Second part- Thread %i entered and did 4 times %i instructions.\n", x69, x16);
    for(int x71=0; x71 < x16; x71++) {
      int32_t x83 = x70;
      int32_t x84 = x82 + x83;
      int32_t x74 = x71 + x73;
      int32_t x75 = x74 * 4;
      int32_t x76 = x11 + x75;
      int32_t x85 = x76 - 3;
      int32_t x86 = x85 - x10;
      x0[x84] = x86;
      float x88 = x0[x76];
      bool x89 = x88 >= x4;
      x70 += x89;
      int32_t x93 = x70;
      int32_t x94 = x82 + x93;
      int32_t x92 = x91 + x75;
      int32_t x95 = x92 - 3;
      int32_t x96 = x95 - x10;
      x0[x94] = x96;
      float x98 = x0[x92];
      bool x99 = x98 >= x4;
      x70 += x99;
      int32_t x103 = x70;
      int32_t x104 = x82 + x103;
      int32_t x102 = x101 + x75;
      int32_t x105 = x102 - 3;
      int32_t x106 = x105 - x10;
      x0[x104] = x106;
      float x108 = x0[x102];
      bool x109 = x108 >= x4;
      x70 += x109;
      int32_t x113 = x70;
      int32_t x114 = x82 + x113;
      int32_t x112 = x111 + x75;
      int32_t x115 = x112 - 3;
      int32_t x116 = x115 - x10;
      x0[x114] = x116;
      float x118 = x0[x112];
      bool x119 = x118 >= x4;
      x70 += x119;
      printf("Thread %i starts from %i = %i + (4*(%i+{(%i * %i)/%f})\n", x69, x76, x11, x71, x69, x5, x6);
      if (x89){
	printf("Thread %i wrote %f\n", x69, x88);
      }
      if (x99){
	printf("Thread %i wrote %f\n", x69, x98);
      }
      if (x109){
	printf("Thread %i wrote %f\n", x69, x108);
      }
      if (x119){
	printf("Thread %i wrote %f\n", x69, x118);
      }
    }
    //#parallel chunk
  }
  for(int x69=0; x69 < x6; x69++) {
  	pthread_create(&threads[x69], NULL, parallelChunk, (void *)&inputArray[x69]);
  }
  for(int x69=0; x69 < x6; x69++) {
	pthread_join(threads[x69], NULL);
  }
  if (x129) {
    int32_t x130 = x128 + 3;
    int32_t x131 = x130 + x10;
    int32_t x132 = x3 + 3;
    int32_t x133 = x132 + x10;
    printf("\n We go from %i to %i.", x131, x133);
    for(int x135=x131; x135 < x133; x135++) {
      int32_t x140 = x135 - 3;
      int32_t x141 = x140 - x10;
      float x143 = x0[x135];
      bool x144 = x143 >= x4;
      //#run residue instructions after unroll
      // generated code for run residue instructions after unroll
      int32_t x136 = x2;
      int32_t x137 = x136 + x3;
      int32_t x138 = x137 + x10;
      int32_t x139 = x138 + 3;
      x0[x139] = x141;
      x2 += x144;
      //#run residue instructions after unroll
    }

  } else {
  }
  printf("%s\n","Number of tuples: ");
  int32_t x153 = x2;
  printf("%d\n",x153);
  printf("%s\n","Output array: ");
  for(int x158=0; x158 < x153; x158++) {
    int32_t x159 = x158 + x156;
    float x160 = x0[x159];
    printf("%f\n",x160);
  }
  bool x164 = x153 == 0;
  if (x164) {
    printf("%s\n","No results found.");
  } else {
  }
  //#Scan Variants- timer goes here
}
/*****************************************
End of C Generated Code
*******************************************/
