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
  int32_t x41 = 3 + x3;
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
      float x26 = x0[x21];
      bool x27 = x26 >= x2;
      //#decorated instruction
      // generated code for decorated instruction
      //#run instruction without branching
      // generated code for run instruction without branching
      int32_t x22 = x1;
      int32_t x23 = x22 + 3;
      int32_t x24 = x23 + x3;
      x0[x24] = x20;
      x1 += x27;
      //#run instruction without branching
      //#decorated instruction
    }
  }
  int32_t x37 = x12;
  __m128 x38 = _mm_set1_ps(x2);
  __m128 x39 = _mm_set1_ps(0);
  __m128 x40 = _mm_set1_ps(0);
  int32_t x13 = x3 - x12;
  int32_t x14 = x13 * x5;
  int32_t x15 = x14 / x6;
  int32_t x42 = x15 / 4;
  for(int x44=0; x44 < x42; x44++) {
    int32_t x45 = 4 * x44;
    float* x47 = (float*) &(x8[x45]);
    __m128 x48 = _mm_load_ps(x47);
    __m128 x49 = _mm_cmpge_ps(x48,x38);
    int32_t x50 = _mm_movemask_ps((__m128)x49);
    int32_t x51 = x37;
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x52 = x1;
    int32_t x53 = x52 + x41;
    x0[x53] = x51;
    int32_t x55 = (x50 & 1);
    x1 += x55;
    //#decorated instruction for vectorization
    x37 += 1;
    int32_t x60 = x37;
    int32_t x61 = (x50 >> 1);
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x62 = x1;
    int32_t x63 = x62 + x41;
    x0[x63] = x60;
    int32_t x65 = (x61 & 1);
    x1 += x65;
    //#decorated instruction for vectorization
    x37 += 1;
    int32_t x70 = x37;
    int32_t x71 = (x50 >> 2);
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x72 = x1;
    int32_t x73 = x72 + x41;
    x0[x73] = x70;
    int32_t x75 = (x71 & 1);
    x1 += x75;
    //#decorated instruction for vectorization
    x37 += 1;
    int32_t x80 = x37;
    int32_t x81 = (x50 >> 3);
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x82 = x1;
    int32_t x83 = x82 + x41;
    x0[x83] = x80;
    int32_t x85 = (x81 & 1);
    x1 += x85;
    //#decorated instruction for vectorization
    x37 += 1;
    int32_t x90 = x45 + 1;
    float* x92 = (float*) &(x8[x90]);
    __m128 x93 = _mm_load_ps(x92);
    __m128 x94 = _mm_cmpge_ps(x93,x38);
    int32_t x95 = _mm_movemask_ps((__m128)x94);
    int32_t x96 = x37;
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x97 = x1;
    int32_t x98 = x97 + x41;
    x0[x98] = x96;
    int32_t x100 = (x95 & 1);
    x1 += x100;
    //#decorated instruction for vectorization
    x37 += 1;
    int32_t x105 = x37;
    int32_t x106 = (x95 >> 1);
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x107 = x1;
    int32_t x108 = x107 + x41;
    x0[x108] = x105;
    int32_t x110 = (x106 & 1);
    x1 += x110;
    //#decorated instruction for vectorization
    x37 += 1;
    int32_t x115 = x37;
    int32_t x116 = (x95 >> 2);
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x117 = x1;
    int32_t x118 = x117 + x41;
    x0[x118] = x115;
    int32_t x120 = (x116 & 1);
    x1 += x120;
    //#decorated instruction for vectorization
    x37 += 1;
    int32_t x125 = x37;
    int32_t x126 = (x95 >> 3);
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x127 = x1;
    int32_t x128 = x127 + x41;
    x0[x128] = x125;
    int32_t x130 = (x126 & 1);
    x1 += x130;
    //#decorated instruction for vectorization
    x37 += 1;
    int32_t x135 = x45 + 2;
    float* x137 = (float*) &(x8[x135]);
    __m128 x138 = _mm_load_ps(x137);
    __m128 x139 = _mm_cmpge_ps(x138,x38);
    int32_t x140 = _mm_movemask_ps((__m128)x139);
    int32_t x141 = x37;
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x142 = x1;
    int32_t x143 = x142 + x41;
    x0[x143] = x141;
    int32_t x145 = (x140 & 1);
    x1 += x145;
    //#decorated instruction for vectorization
    x37 += 1;
    int32_t x150 = x37;
    int32_t x151 = (x140 >> 1);
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x152 = x1;
    int32_t x153 = x152 + x41;
    x0[x153] = x150;
    int32_t x155 = (x151 & 1);
    x1 += x155;
    //#decorated instruction for vectorization
    x37 += 1;
    int32_t x160 = x37;
    int32_t x161 = (x140 >> 2);
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x162 = x1;
    int32_t x163 = x162 + x41;
    x0[x163] = x160;
    int32_t x165 = (x161 & 1);
    x1 += x165;
    //#decorated instruction for vectorization
    x37 += 1;
    int32_t x170 = x37;
    int32_t x171 = (x140 >> 3);
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x172 = x1;
    int32_t x173 = x172 + x41;
    x0[x173] = x170;
    int32_t x175 = (x171 & 1);
    x1 += x175;
    //#decorated instruction for vectorization
    x37 += 1;
    int32_t x180 = x45 + 3;
    float* x182 = (float*) &(x8[x180]);
    __m128 x183 = _mm_load_ps(x182);
    __m128 x184 = _mm_cmpge_ps(x183,x38);
    int32_t x185 = _mm_movemask_ps((__m128)x184);
    int32_t x186 = x37;
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x187 = x1;
    int32_t x188 = x187 + x41;
    x0[x188] = x186;
    int32_t x190 = (x185 & 1);
    x1 += x190;
    //#decorated instruction for vectorization
    x37 += 1;
    int32_t x195 = x37;
    int32_t x196 = (x185 >> 1);
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x197 = x1;
    int32_t x198 = x197 + x41;
    x0[x198] = x195;
    int32_t x200 = (x196 & 1);
    x1 += x200;
    //#decorated instruction for vectorization
    x37 += 1;
    int32_t x205 = x37;
    int32_t x206 = (x185 >> 2);
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x207 = x1;
    int32_t x208 = x207 + x41;
    x0[x208] = x205;
    int32_t x210 = (x206 & 1);
    x1 += x210;
    //#decorated instruction for vectorization
    x37 += 1;
    int32_t x215 = x37;
    int32_t x216 = (x185 >> 3);
    //#decorated instruction for vectorization
    // generated code for decorated instruction for vectorization
    int32_t x217 = x1;
    int32_t x218 = x217 + x41;
    x0[x218] = x215;
    int32_t x220 = (x216 & 1);
    x1 += x220;
    //#decorated instruction for vectorization
    x37 += 1;
  }
  int32_t x227 = x15 * x6;
  int32_t x228 = x227 / x5;
  x16 += x228;
  int32_t x230 = x16;
  for(int x232=x230; x232 < x3; x232++) {
    int32_t x233 = x232 + 3;
    float x238 = x0[x233];
    bool x239 = x238 >= x2;
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction without branching
    // generated code for run instruction without branching
    int32_t x234 = x1;
    int32_t x235 = x234 + 3;
    int32_t x236 = x235 + x3;
    x0[x236] = x232;
    x1 += x239;
    //#run instruction without branching
    //#decorated instruction
  }
  int32_t x247 = x1;
  printf("%s\n","Number of tuples found: ");
  printf("%d\n",x247);
  bool x251 = x247 == 0;
  if (x251) {
  } else {
    printf("%s\n","Output array: ");
    for(int x254=0; x254 < x247; x254++) {
      int32_t x255 = x254 + x41;
      float x256 = x0[x255];
      printf("%f\n",x256);
    }
  }
  //#Scan Variants
}
/*****************************************
End of C Generated Code
*******************************************/
