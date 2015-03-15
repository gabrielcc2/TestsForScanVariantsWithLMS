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
  float x7 = x0[2];
  float x4 = x0[1];
  float x5 = x4 / 4.0f;
  float x6 = x5 / 4.0f;
  int32_t x13 = x6 / x7;
  float x3 = x0[0];
  int32_t x11 = 2 * x7;
  int32_t x12 = 3 + x11;
  bool x94 = x7 > 0;
  int32_t x136 = x12 + 1;
  int32_t x145 = x12 + 2;
  int32_t x154 = x12 + 3;
  int32_t x163 = x12 + 4;
  int32_t x172 = x12 + 5;
  int32_t x181 = x12 + 6;
  int32_t x190 = x12 + 7;
  int32_t x199 = x12 + 8;
  int32_t x208 = x12 + 9;
  int32_t x217 = x12 + 10;
  int32_t x226 = x12 + 11;
  int32_t x235 = x12 + 12;
  int32_t x244 = x12 + 13;
  int32_t x253 = x12 + 14;
  int32_t x262 = x12 + 15;
  int32_t x277 = x13 * 16;
  int32_t x278 = x277 * x7;
  bool x279 = x278 < x4;
  //#Scan Variants
  // generated code for Scan Variants
  int32_t x1 = 0;
  int32_t x2 = 1;
  pthread_t threads[(int)x7];
  int *inputArray;
  inputArray=(int*)malloc(x7*sizeof(int));
  void* parallelPrefixSum(void* input){
    int x9=*(int*)input;
    int32_t x14 = x13 * x9;
    int32_t x15 = 16 * x14;
    int32_t x16 = x12 + x15;
    int32_t x87 = 3 + x9;
    //#parallel prefix sum
    // generated code for parallel prefix sum
    int32_t x10 = 0;
    for(int x18=0; x18 < x13; x18++) {
      int32_t x19 = x18 * 16;
      int32_t x20 = x16 + x19;
      float x21 = x0[x20];
      bool x22 = x21 >= x3;
      x10 += x22;
      int32_t x24 = x20 + 1;
      float x25 = x0[x24];
      bool x26 = x25 >= x3;
      x10 += x26;
      int32_t x28 = x24 + 1;
      float x29 = x0[x28];
      bool x30 = x29 >= x3;
      x10 += x30;
      int32_t x32 = x28 + 1;
      float x33 = x0[x32];
      bool x34 = x33 >= x3;
      x10 += x34;
      int32_t x36 = x32 + 1;
      float x37 = x0[x36];
      bool x38 = x37 >= x3;
      x10 += x38;
      int32_t x40 = x36 + 1;
      float x41 = x0[x40];
      bool x42 = x41 >= x3;
      x10 += x42;
      int32_t x44 = x40 + 1;
      float x45 = x0[x44];
      bool x46 = x45 >= x3;
      x10 += x46;
      int32_t x48 = x44 + 1;
      float x49 = x0[x48];
      bool x50 = x49 >= x3;
      x10 += x50;
      int32_t x52 = x48 + 1;
      float x53 = x0[x52];
      bool x54 = x53 >= x3;
      x10 += x54;
      int32_t x56 = x52 + 1;
      float x57 = x0[x56];
      bool x58 = x57 >= x3;
      x10 += x58;
      int32_t x60 = x56 + 1;
      float x61 = x0[x60];
      bool x62 = x61 >= x3;
      x10 += x62;
      int32_t x64 = x60 + 1;
      float x65 = x0[x64];
      bool x66 = x65 >= x3;
      x10 += x66;
      int32_t x68 = x64 + 1;
      float x69 = x0[x68];
      bool x70 = x69 >= x3;
      x10 += x70;
      int32_t x72 = x68 + 1;
      float x73 = x0[x72];
      bool x74 = x73 >= x3;
      x10 += x74;
      int32_t x76 = x72 + 1;
      float x77 = x0[x76];
      bool x78 = x77 >= x3;
      x10 += x78;
      int32_t x80 = x76 + 1;
      float x81 = x0[x80];
      bool x82 = x81 >= x3;
      x10 += x82;
    }
    int32_t x88 = x10;
    x0[x87] = x88;
    //#parallel prefix sum
  }
  for(int x9=0; x9 < x7; x9++) {
	inputArray[x9]=x9;
	pthread_create(&threads[x9], NULL, parallelPrefixSum, (void *)&inputArray[x9]);
  }
  for(int x9=0; x9 < x7; x9++) {
	pthread_join(threads[x9], NULL);
  }
  if (x94) {
    int32_t x95 = 3 + x7;
    x0[x95] = 0;
    for(int x98=1; x98 < x7; x98++) {
      int32_t x99 = x95 + x98;
      int32_t x100 = 3 + x98;
      int32_t x101 = x100 - 1;
      float x102 = x0[x101];
      int32_t x103 = x99 - 1;
      float x104 = x0[x103];
      float x105 = x102 + x104;
      x0[x99] = x105;
    }
    int32_t x109 = x95 - 1;
    float x110 = x0[x109];
    int32_t x111 = x109 + x7;
    float x112 = x0[x111];
    float x113 = x110 + x112;
    x1 = x113;
  } else {
  }
  void* parallelChunk(void* input){
    int x117=*(int*)input;
    int32_t x124 = 3 + x117;
    int32_t x125 = x124 + x7;
    float x126 = x0[x125];
    int32_t x127 = x126 + x12;
    int32_t x128 = x127 + x4;
    int32_t x120 = x13 * x117;
    //#parallel chunk
    // generated code for parallel chunk
    int32_t x118 = 0;
    for(int x119=0; x119 < x13; x119++) {
      int32_t x129 = x118;
      int32_t x130 = x128 + x129;
      int32_t x121 = x119 + x120;
      int32_t x122 = x121 * 4;
      int32_t x123 = x12 + x122;
      int32_t x131 = x123 - x12;
      x0[x130] = x131;
      float x133 = x0[x123];
      bool x134 = x133 >= x3;
      x118 += x134;
      int32_t x138 = x118;
      int32_t x139 = x128 + x138;
      int32_t x137 = x136 + x122;
      int32_t x140 = x137 - x12;
      x0[x139] = x140;
      float x142 = x0[x137];
      bool x143 = x142 >= x3;
      x118 += x143;
      int32_t x147 = x118;
      int32_t x148 = x128 + x147;
      int32_t x146 = x145 + x122;
      int32_t x149 = x146 - x12;
      x0[x148] = x149;
      float x151 = x0[x146];
      bool x152 = x151 >= x3;
      x118 += x152;
      int32_t x156 = x118;
      int32_t x157 = x128 + x156;
      int32_t x155 = x154 + x122;
      int32_t x158 = x155 - x12;
      x0[x157] = x158;
      float x160 = x0[x155];
      bool x161 = x160 >= x3;
      x118 += x161;
      int32_t x165 = x118;
      int32_t x166 = x128 + x165;
      int32_t x164 = x163 + x122;
      int32_t x167 = x164 - x12;
      x0[x166] = x167;
      float x169 = x0[x164];
      bool x170 = x169 >= x3;
      x118 += x170;
      int32_t x174 = x118;
      int32_t x175 = x128 + x174;
      int32_t x173 = x172 + x122;
      int32_t x176 = x173 - x12;
      x0[x175] = x176;
      float x178 = x0[x173];
      bool x179 = x178 >= x3;
      x118 += x179;
      int32_t x183 = x118;
      int32_t x184 = x128 + x183;
      int32_t x182 = x181 + x122;
      int32_t x185 = x182 - x12;
      x0[x184] = x185;
      float x187 = x0[x182];
      bool x188 = x187 >= x3;
      x118 += x188;
      int32_t x192 = x118;
      int32_t x193 = x128 + x192;
      int32_t x191 = x190 + x122;
      int32_t x194 = x191 - x12;
      x0[x193] = x194;
      float x196 = x0[x191];
      bool x197 = x196 >= x3;
      x118 += x197;
      int32_t x201 = x118;
      int32_t x202 = x128 + x201;
      int32_t x200 = x199 + x122;
      int32_t x203 = x200 - x12;
      x0[x202] = x203;
      float x205 = x0[x200];
      bool x206 = x205 >= x3;
      x118 += x206;
      int32_t x210 = x118;
      int32_t x211 = x128 + x210;
      int32_t x209 = x208 + x122;
      int32_t x212 = x209 - x12;
      x0[x211] = x212;
      float x214 = x0[x209];
      bool x215 = x214 >= x3;
      x118 += x215;
      int32_t x219 = x118;
      int32_t x220 = x128 + x219;
      int32_t x218 = x217 + x122;
      int32_t x221 = x218 - x12;
      x0[x220] = x221;
      float x223 = x0[x218];
      bool x224 = x223 >= x3;
      x118 += x224;
      int32_t x228 = x118;
      int32_t x229 = x128 + x228;
      int32_t x227 = x226 + x122;
      int32_t x230 = x227 - x12;
      x0[x229] = x230;
      float x232 = x0[x227];
      bool x233 = x232 >= x3;
      x118 += x233;
      int32_t x237 = x118;
      int32_t x238 = x128 + x237;
      int32_t x236 = x235 + x122;
      int32_t x239 = x236 - x12;
      x0[x238] = x239;
      float x241 = x0[x236];
      bool x242 = x241 >= x3;
      x118 += x242;
      int32_t x246 = x118;
      int32_t x247 = x128 + x246;
      int32_t x245 = x244 + x122;
      int32_t x248 = x245 - x12;
      x0[x247] = x248;
      float x250 = x0[x245];
      bool x251 = x250 >= x3;
      x118 += x251;
      int32_t x255 = x118;
      int32_t x256 = x128 + x255;
      int32_t x254 = x253 + x122;
      int32_t x257 = x254 - x12;
      x0[x256] = x257;
      float x259 = x0[x254];
      bool x260 = x259 >= x3;
      x118 += x260;
      int32_t x264 = x118;
      int32_t x265 = x128 + x264;
      int32_t x263 = x262 + x122;
      int32_t x266 = x263 - x12;
      x0[x265] = x266;
      float x268 = x0[x263];
      bool x269 = x268 >= x3;
      x118 += x269;
    }
    //#parallel chunk
  }
  for(int x117=0; x117 < x7; x117++) {
  	pthread_create(&threads[x117], NULL, parallelChunk, (void *)&inputArray[x117]);
  }
  for(int x117=0; x117 < x7; x117++) {
	pthread_join(threads[x117], NULL);
  }
  if (x279) {
    for(int x281=x278; x281 < x4; x281++) {
      int32_t x282 = x281 + x12;
      float x287 = x0[x282];
      bool x288 = x287 >= x3;
      //#decorated instruction
      // generated code for decorated instruction
      //#run instruction without branching
      // generated code for run instruction without branching
      int32_t x283 = x1;
      int32_t x284 = x283 + x12;
      int32_t x285 = x284 + x4;
      x0[x285] = x281;
      x1 += x288;
      //#run instruction without branching
      //#decorated instruction
    }
  } else {
  }
  int32_t x298 = x1;
  printf("%s\n","Number of tuples found: ");
  printf("%d\n",x298);
  bool x302 = x298 == 0;
  if (x302) {
  } else {
    printf("%s\n","Output array: ");
    int32_t x304 = x12 + x4;
    for(int x306=0; x306 < x298; x306++) {
      int32_t x307 = x306 + x304;
      float x308 = x0[x307];
      printf("%f\n",x308);
    }
  }
  //#Scan Variants
}
/*****************************************
End of C Generated Code
*******************************************/
