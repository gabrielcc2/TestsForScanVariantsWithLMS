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
  float x4 = x0[2];
  int32_t x5 = 2 * x4;
  int32_t x6 = 3 + x5;
  float x2 = x0[0];
  float x3 = x0[1];
  int32_t x43 = 4 * x4;
  //#Scan Variants
  // generated code for Scan Variants
  int32_t x1 = 0;
  int32_t x7 = sizeof(float);
  int32_t x8 = sizeof(__m128);
  __m128* x10 = (__m128*) &(x0[x6]);
  int32_t x11 = (intptr_t)(x10);
  int32_t x12 = x11 % x8;
  int32_t x13 = x8 - x12;
  int32_t x14 = x13 / x7;
  int32_t x18 = x14;
  char* tmp_array= (char*) x10;
  tmp_array+=x13;
  x10=(__m128*)(tmp_array);
  bool x20 = x13 == 0;
  if (x20) {
  } else {
    for(int x22=0; x22 < x14; x22++) {
      int32_t x23 = x22 + x6;
      float x24 = x0[x23];
      bool x25 = x24 >= x2;
      //#decorated instruction
      // generated code for decorated instruction
      //#decorated instruction
      // generated code for decorated instruction
      //#run instruction with branching
      // generated code for run instruction with branching
      if (x25) {
        int32_t x26 = x1;
        int32_t x27 = x26 + x6;
        int32_t x28 = x27 + x3;
        x0[x28] = x22;
        x1 += 1;
      } else {
      }
      //#run instruction with branching
      //#decorated instruction
      //#decorated instruction
    }
  }
  int32_t x44 = x14 + x43;
  bool x45 = x44 < x3;
  if (x45) {
    int32_t x15 = x3 - x14;
    int32_t x16 = x15 * x7;
    int32_t x17 = x16 / x8;
    int32_t x52 = x17 / x4;
  pthread_t threads[(int)x4];
  int *inputArray;
  inputArray=(int*)malloc(x4*sizeof(int));
  void* parallelPrefixSum(void* input){
    int x47=*(int*)input;
      int32_t x55 = 4 * x47;
      int32_t x56 = x55 / x4;
      int32_t x84 = 3 + x47;
      //#parallel prefix sum
      // generated code for parallel prefix sum
      int32_t x48 = 0;
      __m128 x49 = _mm_set1_ps(x2);
      __m128 x50 = _mm_set1_ps(0);
      __m128 x51 = _mm_set1_ps(0);
      for(int x54=0; x54 < x52; x54++) {
        int32_t x57 = x54 + x56;
        float* x59 = (float*) &(x10[x57]);
        __m128 x60 = _mm_load_ps(x59);
        __m128 x61 = _mm_cmpge_ps(x60,x49);
        int32_t x62 = _mm_movemask_ps((__m128)x61);
        int32_t x63 = (x62 & 1);
        if (x63) {
          x48 += 1;
        } else {
        }
        int32_t x67 = (x62 >> 1);
        int32_t x68 = (x67 & 1);
        if (x68) {
          x48 += 1;
        } else {
        }
        int32_t x72 = (x62 >> 2);
        int32_t x73 = (x72 & 1);
        if (x73) {
          x48 += 1;
        } else {
        }
        int32_t x77 = (x62 >> 3);
        int32_t x78 = (x77 & 1);
        if (x78) {
          x48 += 1;
        } else {
        }
      }
      int32_t x85 = x48;
      x0[x84] = x85;
      //#parallel prefix sum
  }
    for(int x47=0; x47 < x4; x47++) {
	inputArray[x47]=x47;
	pthread_create(&threads[x47], NULL, parallelPrefixSum, (void *)&inputArray[x47]);
  }
    for(int x47=0; x47 < x4; x47++) {
	pthread_join(threads[x47], NULL);
  }
    int32_t x92 = x1;
    int32_t x91 = 3 + x4;
    x0[x91] = x92;
    for(int x95=1; x95 < x4; x95++) {
      int32_t x96 = x91 + x95;
      int32_t x97 = 3 + x95;
      int32_t x98 = x97 - 1;
      float x99 = x0[x98];
      int32_t x100 = x96 - 1;
      float x101 = x0[x100];
      float x102 = x99 + x101;
      x0[x96] = x102;
    }
    int32_t x106 = x91 - 1;
    float x107 = x0[x106];
    int32_t x108 = x106 + x4;
    float x109 = x0[x108];
    float x110 = x107 + x109;
    x1 = x110;
    int32_t x121 = x6 + x3;
  void* parallelWriting(void* input){
    int x112=*(int*)input;
      int32_t x113 = x112 * x4;
      int32_t x114 = x113 * x17;
      int32_t x115 = x114 / x4;
      int32_t x116 = x14 + x115;
      int32_t x124 = 4 * x112;
      int32_t x125 = x124 / x4;
      int32_t x133 = 3 + x112;
      int32_t x134 = x133 + x4;
      float x135 = x0[x134];
      //#parallel writing
      // generated code for parallel writing
      int32_t x117 = 0;
      __m128 x118 = _mm_set1_ps(x2);
      __m128 x119 = _mm_set1_ps(0);
      __m128 x120 = _mm_set1_ps(0);
      int32_t x122 = x116;
      for(int x123=0; x123 < x52; x123++) {
        int32_t x126 = x123 + x125;
        float* x128 = (float*) &(x10[x126]);
        __m128 x129 = _mm_load_ps(x128);
        __m128 x130 = _mm_cmpge_ps(x129,x118);
        int32_t x131 = _mm_movemask_ps((__m128)x130);
        int32_t x132 = (x131 & 1);
        if (x132) {
          int32_t x136 = x117;
          int32_t x139 = x122;
          int32_t x137 = x135 + x136;
          int32_t x138 = x137 + x121;
          x0[x138] = x139;
          x117 += 1;
        } else {
        }
        x122 += 1;
        int32_t x145 = (x131 >> 1);
        int32_t x146 = (x145 & 1);
        if (x146) {
          int32_t x147 = x117;
          int32_t x150 = x122;
          int32_t x148 = x135 + x147;
          int32_t x149 = x148 + x121;
          x0[x149] = x150;
          x117 += 1;
        } else {
        }
        x122 += 1;
        int32_t x156 = (x131 >> 2);
        int32_t x157 = (x156 & 1);
        if (x157) {
          int32_t x158 = x117;
          int32_t x161 = x122;
          int32_t x159 = x135 + x158;
          int32_t x160 = x159 + x121;
          x0[x160] = x161;
          x117 += 1;
        } else {
        }
        x122 += 1;
        int32_t x167 = (x131 >> 3);
        int32_t x168 = (x167 & 1);
        if (x168) {
          int32_t x169 = x117;
          int32_t x172 = x122;
          int32_t x170 = x135 + x169;
          int32_t x171 = x170 + x121;
          x0[x171] = x172;
          x117 += 1;
        } else {
        }
        x122 += 1;
      }
      //#parallel writing
  }
    for(int x112=0; x112 < x4; x112++) {
  	pthread_create(&threads[x112], NULL, parallelWriting, (void *)&inputArray[x112]);
  }
    for(int x112=0; x112 < x4; x112++) {
	pthread_join(threads[x112], NULL);
  }
    double x184 = x17 / x4;
    bool x186 = x184 == x52;
    if (x186) {
      int32_t x187 = x17 * x8;
      int32_t x188 = x187 / x7;
      x18 += x188;
    } else {
      int32_t x191 = x17 * x4;
      x18 += x191;
    }
  } else {
  }
  int32_t x197 = x18;
  for(int x199=x197; x199 < x3; x199++) {
    int32_t x200 = x199 + x6;
    float x201 = x0[x200];
    bool x202 = x201 >= x2;
    //#decorated instruction
    // generated code for decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x202) {
      int32_t x203 = x1;
      int32_t x204 = x203 + x6;
      int32_t x205 = x204 + x3;
      x0[x205] = x199;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
    //#decorated instruction
  }
  int32_t x218 = x1;
  printf("%s\n","Number of tuples found: ");
  printf("%d\n",x218);
  bool x222 = x218 == 0;
  if (x222) {
  } else {
    printf("%s\n","Output array: ");
    int32_t x121 = x6 + x3;
    for(int x225=0; x225 < x218; x225++) {
      int32_t x226 = x225 + x121;
      float x227 = x0[x226];
      printf("%f\n",x227);
    }
  }
  //#Scan Variants
}
/*****************************************
End of C Generated Code
*******************************************/
