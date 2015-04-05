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
  float x5 = x0[2];
  int32_t x6 = 2 * x5;
  int32_t x7 = 3 + x6;
  float x2 = x0[0];
  float x3 = x0[1];
  int32_t x44 = 16 * x5;
  //#Scan Variants
  // generated code for Scan Variants
  int32_t x1 = 0;
  int32_t x8 = sizeof(float);
  int32_t x9 = sizeof(__m128);
  __m128* x11 = (__m128*) &(x0[x7]);
  int32_t x12 = (intptr_t)(x11);
  int32_t x13 = x12 % x9;
  int32_t x14 = x9 - x13;
  int32_t x15 = x14 / x8;
  int32_t x19 = x15;
  char* tmp_array= (char*) x11;
  tmp_array+=x14;
  x11=(__m128*)(tmp_array);
  bool x21 = x14 == 0;
  if (x21) {
  } else {
    for(int x23=0; x23 < x15; x23++) {
      int32_t x24 = x23 + x7;
      float x25 = x0[x24];
      bool x26 = x25 >= x2;
      //#decorated instruction
      // generated code for decorated instruction
      //#decorated instruction
      // generated code for decorated instruction
      //#run instruction with branching
      // generated code for run instruction with branching
      if (x26) {
        int32_t x27 = x1;
        int32_t x28 = x27 + x7;
        int32_t x29 = x28 + x3;
        x0[x29] = x23;
        x1 += 1;
      } else {
      }
      //#run instruction with branching
      //#decorated instruction
      //#decorated instruction
    }
  }
  int32_t x45 = x15 + x44;
  bool x46 = x45 < x3;
  if (x46) {
    int32_t x16 = x3 - x15;
    int32_t x17 = x16 * x8;
    int32_t x18 = x17 / x9;
    int32_t x53 = 4 * x5;
    int32_t x54 = x18 / x53;
  pthread_t threads[(int)x5];
  int *inputArray;
  inputArray=(int*)malloc(x5*sizeof(int));
  void* parallelPrefixSum(void* input){
    int x48=*(int*)input;
      int32_t x58 = 4 * x48;
      int32_t x59 = x58 / x5;
      int32_t x162 = 3 + x48;
      //#parallel prefix sum
      // generated code for parallel prefix sum
      int32_t x49 = 0;
      __m128 x50 = _mm_set1_ps(x2);
      __m128 x51 = _mm_set1_ps(0);
      __m128 x52 = _mm_set1_ps(0);
      for(int x56=0; x56 < x54; x56++) {
        int32_t x57 = 4 * x56;
        int32_t x60 = x57 + x59;
        float* x62 = (float*) &(x11[x60]);
        __m128 x63 = _mm_load_ps(x62);
        __m128 x64 = _mm_cmpge_ps(x63,x50);
        int32_t x65 = _mm_movemask_ps((__m128)x64);
        int32_t x66 = (x65 & 1);
        if (x66) {
          x49 += 1;
        } else {
        }
        int32_t x70 = (x65 >> 1);
        int32_t x71 = (x70 & 1);
        if (x71) {
          x49 += 1;
        } else {
        }
        int32_t x75 = (x65 >> 2);
        int32_t x76 = (x75 & 1);
        if (x76) {
          x49 += 1;
        } else {
        }
        int32_t x80 = (x65 >> 3);
        int32_t x81 = (x80 & 1);
        if (x81) {
          x49 += 1;
        } else {
        }
        int32_t x85 = x60 + 1;
        float* x87 = (float*) &(x11[x85]);
        __m128 x88 = _mm_load_ps(x87);
        __m128 x89 = _mm_cmpge_ps(x88,x50);
        int32_t x90 = _mm_movemask_ps((__m128)x89);
        int32_t x91 = (x90 & 1);
        if (x91) {
          x49 += 1;
        } else {
        }
        int32_t x95 = (x90 >> 1);
        int32_t x96 = (x95 & 1);
        if (x96) {
          x49 += 1;
        } else {
        }
        int32_t x100 = (x90 >> 2);
        int32_t x101 = (x100 & 1);
        if (x101) {
          x49 += 1;
        } else {
        }
        int32_t x105 = (x90 >> 3);
        int32_t x106 = (x105 & 1);
        if (x106) {
          x49 += 1;
        } else {
        }
        int32_t x110 = x60 + 2;
        float* x112 = (float*) &(x11[x110]);
        __m128 x113 = _mm_load_ps(x112);
        __m128 x114 = _mm_cmpge_ps(x113,x50);
        int32_t x115 = _mm_movemask_ps((__m128)x114);
        int32_t x116 = (x115 & 1);
        if (x116) {
          x49 += 1;
        } else {
        }
        int32_t x120 = (x115 >> 1);
        int32_t x121 = (x120 & 1);
        if (x121) {
          x49 += 1;
        } else {
        }
        int32_t x125 = (x115 >> 2);
        int32_t x126 = (x125 & 1);
        if (x126) {
          x49 += 1;
        } else {
        }
        int32_t x130 = (x115 >> 3);
        int32_t x131 = (x130 & 1);
        if (x131) {
          x49 += 1;
        } else {
        }
        int32_t x135 = x60 + 3;
        float* x137 = (float*) &(x11[x135]);
        __m128 x138 = _mm_load_ps(x137);
        __m128 x139 = _mm_cmpge_ps(x138,x50);
        int32_t x140 = _mm_movemask_ps((__m128)x139);
        int32_t x141 = (x140 & 1);
        if (x141) {
          x49 += 1;
        } else {
        }
        int32_t x145 = (x140 >> 1);
        int32_t x146 = (x145 & 1);
        if (x146) {
          x49 += 1;
        } else {
        }
        int32_t x150 = (x140 >> 2);
        int32_t x151 = (x150 & 1);
        if (x151) {
          x49 += 1;
        } else {
        }
        int32_t x155 = (x140 >> 3);
        int32_t x156 = (x155 & 1);
        if (x156) {
          x49 += 1;
        } else {
        }
      }
      int32_t x163 = x49;
      x0[x162] = x163;
      //#parallel prefix sum
  }
    for(int x48=0; x48 < x5; x48++) {
	inputArray[x48]=x48;
	pthread_create(&threads[x48], NULL, parallelPrefixSum, (void *)&inputArray[x48]);
  }
    for(int x48=0; x48 < x5; x48++) {
	pthread_join(threads[x48], NULL);
  }
    int32_t x170 = x1;
    int32_t x169 = 3 + x5;
    x0[x169] = x170;
    for(int x173=1; x173 < x5; x173++) {
      int32_t x174 = x169 + x173;
      int32_t x175 = 3 + x173;
      int32_t x176 = x175 - 1;
      float x177 = x0[x176];
      int32_t x178 = x174 - 1;
      float x179 = x0[x178];
      float x180 = x177 + x179;
      x0[x174] = x180;
    }
    int32_t x184 = x169 - 1;
    float x185 = x0[x184];
    int32_t x186 = x184 + x5;
    float x187 = x0[x186];
    float x188 = x185 + x187;
    x1 = x188;
    int32_t x199 = x7 + x3;
  void* parallelWriting(void* input){
    int x190=*(int*)input;
      int32_t x191 = x190 * x5;
      int32_t x192 = x191 * x18;
      int32_t x193 = x192 / x53;
      int32_t x194 = x15 + x193;
      int32_t x203 = 4 * x190;
      int32_t x204 = x203 / x5;
      int32_t x212 = 3 + x190;
      int32_t x213 = x212 + x5;
      float x214 = x0[x213];
      //#parallel writing
      // generated code for parallel writing
      int32_t x195 = 0;
      __m128 x196 = _mm_set1_ps(x2);
      __m128 x197 = _mm_set1_ps(0);
      __m128 x198 = _mm_set1_ps(0);
      int32_t x200 = x194;
      for(int x201=0; x201 < x54; x201++) {
        int32_t x202 = 4 * x201;
        int32_t x205 = x202 + x204;
        float* x207 = (float*) &(x11[x205]);
        __m128 x208 = _mm_load_ps(x207);
        __m128 x209 = _mm_cmpge_ps(x208,x196);
        int32_t x210 = _mm_movemask_ps((__m128)x209);
        int32_t x211 = (x210 & 1);
        if (x211) {
          int32_t x215 = x195;
          int32_t x218 = x200;
          int32_t x216 = x214 + x215;
          int32_t x217 = x216 + x199;
          x0[x217] = x218;
          x195 += 1;
        } else {
        }
        x200 += 1;
        int32_t x224 = (x210 >> 1);
        int32_t x225 = (x224 & 1);
        if (x225) {
          int32_t x226 = x195;
          int32_t x229 = x200;
          int32_t x227 = x214 + x226;
          int32_t x228 = x227 + x199;
          x0[x228] = x229;
          x195 += 1;
        } else {
        }
        x200 += 1;
        int32_t x235 = (x210 >> 2);
        int32_t x236 = (x235 & 1);
        if (x236) {
          int32_t x237 = x195;
          int32_t x240 = x200;
          int32_t x238 = x214 + x237;
          int32_t x239 = x238 + x199;
          x0[x239] = x240;
          x195 += 1;
        } else {
        }
        x200 += 1;
        int32_t x246 = (x210 >> 3);
        int32_t x247 = (x246 & 1);
        if (x247) {
          int32_t x248 = x195;
          int32_t x251 = x200;
          int32_t x249 = x214 + x248;
          int32_t x250 = x249 + x199;
          x0[x250] = x251;
          x195 += 1;
        } else {
        }
        x200 += 1;
        int32_t x257 = x205 + 1;
        float* x259 = (float*) &(x11[x257]);
        __m128 x260 = _mm_load_ps(x259);
        __m128 x261 = _mm_cmpge_ps(x260,x196);
        int32_t x262 = _mm_movemask_ps((__m128)x261);
        int32_t x263 = (x262 & 1);
        if (x263) {
          int32_t x264 = x195;
          int32_t x267 = x200;
          int32_t x265 = x214 + x264;
          int32_t x266 = x265 + x199;
          x0[x266] = x267;
          x195 += 1;
        } else {
        }
        x200 += 1;
        int32_t x273 = (x262 >> 1);
        int32_t x274 = (x273 & 1);
        if (x274) {
          int32_t x275 = x195;
          int32_t x278 = x200;
          int32_t x276 = x214 + x275;
          int32_t x277 = x276 + x199;
          x0[x277] = x278;
          x195 += 1;
        } else {
        }
        x200 += 1;
        int32_t x284 = (x262 >> 2);
        int32_t x285 = (x284 & 1);
        if (x285) {
          int32_t x286 = x195;
          int32_t x289 = x200;
          int32_t x287 = x214 + x286;
          int32_t x288 = x287 + x199;
          x0[x288] = x289;
          x195 += 1;
        } else {
        }
        x200 += 1;
        int32_t x295 = (x262 >> 3);
        int32_t x296 = (x295 & 1);
        if (x296) {
          int32_t x297 = x195;
          int32_t x300 = x200;
          int32_t x298 = x214 + x297;
          int32_t x299 = x298 + x199;
          x0[x299] = x300;
          x195 += 1;
        } else {
        }
        x200 += 1;
        int32_t x306 = x205 + 2;
        float* x308 = (float*) &(x11[x306]);
        __m128 x309 = _mm_load_ps(x308);
        __m128 x310 = _mm_cmpge_ps(x309,x196);
        int32_t x311 = _mm_movemask_ps((__m128)x310);
        int32_t x312 = (x311 & 1);
        if (x312) {
          int32_t x313 = x195;
          int32_t x316 = x200;
          int32_t x314 = x214 + x313;
          int32_t x315 = x314 + x199;
          x0[x315] = x316;
          x195 += 1;
        } else {
        }
        x200 += 1;
        int32_t x322 = (x311 >> 1);
        int32_t x323 = (x322 & 1);
        if (x323) {
          int32_t x324 = x195;
          int32_t x327 = x200;
          int32_t x325 = x214 + x324;
          int32_t x326 = x325 + x199;
          x0[x326] = x327;
          x195 += 1;
        } else {
        }
        x200 += 1;
        int32_t x333 = (x311 >> 2);
        int32_t x334 = (x333 & 1);
        if (x334) {
          int32_t x335 = x195;
          int32_t x338 = x200;
          int32_t x336 = x214 + x335;
          int32_t x337 = x336 + x199;
          x0[x337] = x338;
          x195 += 1;
        } else {
        }
        x200 += 1;
        int32_t x344 = (x311 >> 3);
        int32_t x345 = (x344 & 1);
        if (x345) {
          int32_t x346 = x195;
          int32_t x349 = x200;
          int32_t x347 = x214 + x346;
          int32_t x348 = x347 + x199;
          x0[x348] = x349;
          x195 += 1;
        } else {
        }
        x200 += 1;
        int32_t x355 = x205 + 3;
        float* x357 = (float*) &(x11[x355]);
        __m128 x358 = _mm_load_ps(x357);
        __m128 x359 = _mm_cmpge_ps(x358,x196);
        int32_t x360 = _mm_movemask_ps((__m128)x359);
        int32_t x361 = (x360 & 1);
        if (x361) {
          int32_t x362 = x195;
          int32_t x365 = x200;
          int32_t x363 = x214 + x362;
          int32_t x364 = x363 + x199;
          x0[x364] = x365;
          x195 += 1;
        } else {
        }
        x200 += 1;
        int32_t x371 = (x360 >> 1);
        int32_t x372 = (x371 & 1);
        if (x372) {
          int32_t x373 = x195;
          int32_t x376 = x200;
          int32_t x374 = x214 + x373;
          int32_t x375 = x374 + x199;
          x0[x375] = x376;
          x195 += 1;
        } else {
        }
        x200 += 1;
        int32_t x382 = (x360 >> 2);
        int32_t x383 = (x382 & 1);
        if (x383) {
          int32_t x384 = x195;
          int32_t x387 = x200;
          int32_t x385 = x214 + x384;
          int32_t x386 = x385 + x199;
          x0[x386] = x387;
          x195 += 1;
        } else {
        }
        x200 += 1;
        int32_t x393 = (x360 >> 3);
        int32_t x394 = (x393 & 1);
        if (x394) {
          int32_t x395 = x195;
          int32_t x398 = x200;
          int32_t x396 = x214 + x395;
          int32_t x397 = x396 + x199;
          x0[x397] = x398;
          x195 += 1;
        } else {
        }
        x200 += 1;
      }
      //#parallel writing
  }
    for(int x190=0; x190 < x5; x190++) {
  	pthread_create(&threads[x190], NULL, parallelWriting, (void *)&inputArray[x190]);
  }
    for(int x190=0; x190 < x5; x190++) {
	pthread_join(threads[x190], NULL);
  }
    double x410 = x18 / x53;
    bool x412 = x410 == x54;
    if (x412) {
      int32_t x413 = x18 * x9;
      int32_t x414 = x413 / x8;
      x19 += x414;
    } else {
      int32_t x417 = x18 * 4;
      int32_t x418 = x417 * x5;
      x19 += x418;
    }
  } else {
  }
  int32_t x424 = x19;
  for(int x426=x424; x426 < x3; x426++) {
    int32_t x427 = x426 + x7;
    float x428 = x0[x427];
    bool x429 = x428 >= x2;
    //#decorated instruction
    // generated code for decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x429) {
      int32_t x430 = x1;
      int32_t x431 = x430 + x7;
      int32_t x432 = x431 + x3;
      x0[x432] = x426;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
    //#decorated instruction
  }
  int32_t x445 = x1;
  printf("%s\n","Number of tuples found: ");
  printf("%d\n",x445);
  bool x449 = x445 == 0;
  if (x449) {
  } else {
    printf("%s\n","Output array: ");
    int32_t x199 = x7 + x3;
    for(int x452=0; x452 < x445; x452++) {
      int32_t x453 = x452 + x199;
      float x454 = x0[x453];
      printf("%f\n",x454);
    }
  }
  //#Scan Variants
}
/*****************************************
End of C Generated Code
*******************************************/
