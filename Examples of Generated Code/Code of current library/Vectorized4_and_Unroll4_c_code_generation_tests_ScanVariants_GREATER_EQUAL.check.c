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
  float x2 = x0[0];
  float x3 = x0[1];
  int32_t x43 = 3 + x3;
  //#Scan Variants
  // generated code for Scan Variants
  int32_t x1 = 0;
  int32_t x5 = sizeof(float);
  int32_t x6 = sizeof(__m128);
  __m128* x8 = (__m128*) &(x0[3]);
  int32_t x9 = (intptr_t)(x8);
  int32_t x10 = x9 % x6;
  int32_t x11 = x6 - x10;
  int32_t x12 = x11 / x5;
  int32_t x16 = x12;
  char* tmp_array= (char*) x8;
  tmp_array+=x11;
  x8=(__m128*)(tmp_array);
  bool x18 = x11 == 0;
  if (x18) {
  } else {
    for(int x20=0; x20 < x12; x20++) {
      int32_t x21 = x20 + 3;
      float x22 = x0[x21];
      bool x23 = x22 >= x2;
      //#decorated instruction
      // generated code for decorated instruction
      //#run instruction with branching
      // generated code for run instruction with branching
      if (x23) {
        int32_t x24 = x1;
        int32_t x25 = x24 + 3;
        int32_t x26 = x25 + x3;
        x0[x26] = x20;
        x1 += 1;
      } else {
      }
      //#run instruction with branching
      //#decorated instruction
    }
  }
  int32_t x39 = x12;
  __m128 x40 = _mm_set1_ps(x2);
  __m128 x41 = _mm_set1_ps(0);
  __m128 x42 = _mm_set1_ps(0);
  int32_t x13 = x3 - x12;
  int32_t x14 = x13 * x5;
  int32_t x15 = x14 / x6;
  int32_t x44 = x15 / 4;
  for(int x46=0; x46 < x44; x46++) {
    int32_t x47 = 4 * x46;
    float* x49 = (float*) &(x8[x47]);
    __m128 x50 = _mm_load_ps(x49);
    __m128 x51 = _mm_cmpge_ps(x50,x40);
    int32_t x52 = _mm_movemask_ps((__m128)x51);
    int32_t x53 = x39;
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x54 = (x52 & 1);
    if (x54) {
      int32_t x55 = x1;
      int32_t x56 = x55 + x43;
      x0[x56] = x53;
      x1 += 1;
    } else {
    }
    //#decorated instruction for vectorization
    x39 += 1;
    int32_t x64 = x39;
    int32_t x65 = (x52 >> 1);
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x66 = (x65 & 1);
    if (x66) {
      int32_t x67 = x1;
      int32_t x68 = x67 + x43;
      x0[x68] = x64;
      x1 += 1;
    } else {
    }
    //#decorated instruction for vectorization
    x39 += 1;
    int32_t x76 = x39;
    int32_t x77 = (x52 >> 2);
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x78 = (x77 & 1);
    if (x78) {
      int32_t x79 = x1;
      int32_t x80 = x79 + x43;
      x0[x80] = x76;
      x1 += 1;
    } else {
    }
    //#decorated instruction for vectorization
    x39 += 1;
    int32_t x88 = x39;
    int32_t x89 = (x52 >> 3);
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x90 = (x89 & 1);
    if (x90) {
      int32_t x91 = x1;
      int32_t x92 = x91 + x43;
      x0[x92] = x88;
      x1 += 1;
    } else {
    }
    //#decorated instruction for vectorization
    x39 += 1;
    int32_t x100 = x47 + 1;
    float* x102 = (float*) &(x8[x100]);
    __m128 x103 = _mm_load_ps(x102);
    __m128 x104 = _mm_cmpge_ps(x103,x40);
    int32_t x105 = _mm_movemask_ps((__m128)x104);
    int32_t x106 = x39;
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x107 = (x105 & 1);
    if (x107) {
      int32_t x108 = x1;
      int32_t x109 = x108 + x43;
      x0[x109] = x106;
      x1 += 1;
    } else {
    }
    //#decorated instruction for vectorization
    x39 += 1;
    int32_t x117 = x39;
    int32_t x118 = (x105 >> 1);
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x119 = (x118 & 1);
    if (x119) {
      int32_t x120 = x1;
      int32_t x121 = x120 + x43;
      x0[x121] = x117;
      x1 += 1;
    } else {
    }
    //#decorated instruction for vectorization
    x39 += 1;
    int32_t x129 = x39;
    int32_t x130 = (x105 >> 2);
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x131 = (x130 & 1);
    if (x131) {
      int32_t x132 = x1;
      int32_t x133 = x132 + x43;
      x0[x133] = x129;
      x1 += 1;
    } else {
    }
    //#decorated instruction for vectorization
    x39 += 1;
    int32_t x141 = x39;
    int32_t x142 = (x105 >> 3);
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x143 = (x142 & 1);
    if (x143) {
      int32_t x144 = x1;
      int32_t x145 = x144 + x43;
      x0[x145] = x141;
      x1 += 1;
    } else {
    }
    //#decorated instruction for vectorization
    x39 += 1;
    int32_t x153 = x47 + 2;
    float* x155 = (float*) &(x8[x153]);
    __m128 x156 = _mm_load_ps(x155);
    __m128 x157 = _mm_cmpge_ps(x156,x40);
    int32_t x158 = _mm_movemask_ps((__m128)x157);
    int32_t x159 = x39;
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x160 = (x158 & 1);
    if (x160) {
      int32_t x161 = x1;
      int32_t x162 = x161 + x43;
      x0[x162] = x159;
      x1 += 1;
    } else {
    }
    //#decorated instruction for vectorization
    x39 += 1;
    int32_t x170 = x39;
    int32_t x171 = (x158 >> 1);
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x172 = (x171 & 1);
    if (x172) {
      int32_t x173 = x1;
      int32_t x174 = x173 + x43;
      x0[x174] = x170;
      x1 += 1;
    } else {
    }
    //#decorated instruction for vectorization
    x39 += 1;
    int32_t x182 = x39;
    int32_t x183 = (x158 >> 2);
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x184 = (x183 & 1);
    if (x184) {
      int32_t x185 = x1;
      int32_t x186 = x185 + x43;
      x0[x186] = x182;
      x1 += 1;
    } else {
    }
    //#decorated instruction for vectorization
    x39 += 1;
    int32_t x194 = x39;
    int32_t x195 = (x158 >> 3);
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x196 = (x195 & 1);
    if (x196) {
      int32_t x197 = x1;
      int32_t x198 = x197 + x43;
      x0[x198] = x194;
      x1 += 1;
    } else {
    }
    //#decorated instruction for vectorization
    x39 += 1;
    int32_t x206 = x47 + 3;
    float* x208 = (float*) &(x8[x206]);
    __m128 x209 = _mm_load_ps(x208);
    __m128 x210 = _mm_cmpge_ps(x209,x40);
    int32_t x211 = _mm_movemask_ps((__m128)x210);
    int32_t x212 = x39;
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x213 = (x211 & 1);
    if (x213) {
      int32_t x214 = x1;
      int32_t x215 = x214 + x43;
      x0[x215] = x212;
      x1 += 1;
    } else {
    }
    //#decorated instruction for vectorization
    x39 += 1;
    int32_t x223 = x39;
    int32_t x224 = (x211 >> 1);
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x225 = (x224 & 1);
    if (x225) {
      int32_t x226 = x1;
      int32_t x227 = x226 + x43;
      x0[x227] = x223;
      x1 += 1;
    } else {
    }
    //#decorated instruction for vectorization
    x39 += 1;
    int32_t x235 = x39;
    int32_t x236 = (x211 >> 2);
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x237 = (x236 & 1);
    if (x237) {
      int32_t x238 = x1;
      int32_t x239 = x238 + x43;
      x0[x239] = x235;
      x1 += 1;
    } else {
    }
    //#decorated instruction for vectorization
    x39 += 1;
    int32_t x247 = x39;
    int32_t x248 = (x211 >> 3);
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x249 = (x248 & 1);
    if (x249) {
      int32_t x250 = x1;
      int32_t x251 = x250 + x43;
      x0[x251] = x247;
      x1 += 1;
    } else {
    }
    //#decorated instruction for vectorization
    x39 += 1;
  }
  int32_t x261 = x15 * x6;
  int32_t x262 = x261 / x5;
  x16 += x262;
  int32_t x264 = x16;
  for(int x266=x264; x266 < x3; x266++) {
    int32_t x267 = x266 + 3;
    float x268 = x0[x267];
    bool x269 = x268 >= x2;
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x269) {
      int32_t x270 = x1;
      int32_t x271 = x270 + 3;
      int32_t x272 = x271 + x3;
      x0[x272] = x266;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
  }
  int32_t x283 = x1;
  printf("%s\n","Number of tuples found: ");
  printf("%d\n",x283);
  bool x287 = x283 == 0;
  if (x287) {
  } else {
    printf("%s\n","Output array: ");
    for(int x290=0; x290 < x283; x290++) {
      int32_t x291 = x290 + x43;
      float x292 = x0[x291];
      printf("%f\n",x292);
    }
  }
  //#Scan Variants
}
/*****************************************
End of C Generated Code
*******************************************/
