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
  float x3 = x0[1];
  float x2 = x0[0];
  int32_t x42 = 16 * x5;
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
      float x29 = x0[x24];
      bool x30 = x29 >= x2;
      //#decorated instruction
      // generated code for decorated instruction
      //#decorated instruction
      // generated code for decorated instruction
      //#run instruction without branching
      // generated code for run instruction without branching
      int32_t x25 = x1;
      int32_t x26 = x25 + x7;
      int32_t x27 = x26 + x3;
      x0[x27] = x23;
      x1 += x30;
      //#run instruction without branching
      //#decorated instruction
      //#decorated instruction
    }
  }
  int32_t x43 = x15 + x42;
  bool x44 = x43 < x3;
  if (x44) {
    int32_t x16 = x3 - x15;
    int32_t x17 = x16 * x8;
    int32_t x18 = x17 / x9;
    int32_t x51 = 4 * x5;
    int32_t x52 = x18 / x51;
  pthread_t threads[(int)x5];
  int *inputArray;
  inputArray=(int*)malloc(x5*sizeof(int));
  void* parallelPrefixSum(void* input){
    int x46=*(int*)input;
      int32_t x56 = 4 * x46;
      int32_t x57 = x56 / x5;
      int32_t x128 = 3 + x46;
      //#parallel prefix sum
      // generated code for parallel prefix sum
      int32_t x47 = 0;
      __m128 x48 = _mm_set1_ps(x2);
      __m128 x49 = _mm_set1_ps(0);
      __m128 x50 = _mm_set1_ps(0);
      for(int x54=0; x54 < x52; x54++) {
        int32_t x55 = 4 * x54;
        int32_t x58 = x55 + x57;
        float* x60 = (float*) &(x11[x58]);
        __m128 x61 = _mm_load_ps(x60);
        __m128 x62 = _mm_cmpge_ps(x61,x48);
        int32_t x63 = _mm_movemask_ps((__m128)x62);
        int32_t x64 = (x63 & 1);
        x47 += x64;
        int32_t x66 = (x63 >> 1);
        int32_t x67 = (x66 & 1);
        x47 += x67;
        int32_t x69 = (x63 >> 2);
        int32_t x70 = (x69 & 1);
        x47 += x70;
        int32_t x72 = (x63 >> 3);
        int32_t x73 = (x72 & 1);
        x47 += x73;
        int32_t x75 = x58 + 1;
        float* x77 = (float*) &(x11[x75]);
        __m128 x78 = _mm_load_ps(x77);
        __m128 x79 = _mm_cmpge_ps(x78,x48);
        int32_t x80 = _mm_movemask_ps((__m128)x79);
        int32_t x81 = (x80 & 1);
        x47 += x81;
        int32_t x83 = (x80 >> 1);
        int32_t x84 = (x83 & 1);
        x47 += x84;
        int32_t x86 = (x80 >> 2);
        int32_t x87 = (x86 & 1);
        x47 += x87;
        int32_t x89 = (x80 >> 3);
        int32_t x90 = (x89 & 1);
        x47 += x90;
        int32_t x92 = x58 + 2;
        float* x94 = (float*) &(x11[x92]);
        __m128 x95 = _mm_load_ps(x94);
        __m128 x96 = _mm_cmpge_ps(x95,x48);
        int32_t x97 = _mm_movemask_ps((__m128)x96);
        int32_t x98 = (x97 & 1);
        x47 += x98;
        int32_t x100 = (x97 >> 1);
        int32_t x101 = (x100 & 1);
        x47 += x101;
        int32_t x103 = (x97 >> 2);
        int32_t x104 = (x103 & 1);
        x47 += x104;
        int32_t x106 = (x97 >> 3);
        int32_t x107 = (x106 & 1);
        x47 += x107;
        int32_t x109 = x58 + 3;
        float* x111 = (float*) &(x11[x109]);
        __m128 x112 = _mm_load_ps(x111);
        __m128 x113 = _mm_cmpge_ps(x112,x48);
        int32_t x114 = _mm_movemask_ps((__m128)x113);
        int32_t x115 = (x114 & 1);
        x47 += x115;
        int32_t x117 = (x114 >> 1);
        int32_t x118 = (x117 & 1);
        x47 += x118;
        int32_t x120 = (x114 >> 2);
        int32_t x121 = (x120 & 1);
        x47 += x121;
        int32_t x123 = (x114 >> 3);
        int32_t x124 = (x123 & 1);
        x47 += x124;
      }
      int32_t x129 = x47;
      x0[x128] = x129;
      //#parallel prefix sum
  }
    for(int x46=0; x46 < x5; x46++) {
	inputArray[x46]=x46;
	pthread_create(&threads[x46], NULL, parallelPrefixSum, (void *)&inputArray[x46]);
  }
    for(int x46=0; x46 < x5; x46++) {
	pthread_join(threads[x46], NULL);
  }
    int32_t x136 = x1;
    int32_t x135 = 3 + x5;
    x0[x135] = x136;
    for(int x139=1; x139 < x5; x139++) {
      int32_t x140 = x135 + x139;
      int32_t x141 = 3 + x139;
      int32_t x142 = x141 - 1;
      float x143 = x0[x142];
      int32_t x144 = x140 - 1;
      float x145 = x0[x144];
      float x146 = x143 + x145;
      x0[x140] = x146;
    }
    int32_t x150 = x135 - 1;
    float x151 = x0[x150];
    int32_t x152 = x150 + x5;
    float x153 = x0[x152];
    float x154 = x151 + x153;
    x1 = x154;
    int32_t x165 = x7 + x3;
  void* parallelWriting(void* input){
    int x156=*(int*)input;
      int32_t x157 = x156 * x5;
      int32_t x158 = x157 * x18;
      int32_t x159 = x158 / x51;
      int32_t x160 = x15 + x159;
      int32_t x169 = 4 * x156;
      int32_t x170 = x169 / x5;
      int32_t x177 = 3 + x156;
      int32_t x178 = x177 + x5;
      float x179 = x0[x178];
      //#parallel writing
      // generated code for parallel writing
      int32_t x161 = 0;
      __m128 x162 = _mm_set1_ps(x2);
      __m128 x163 = _mm_set1_ps(0);
      __m128 x164 = _mm_set1_ps(0);
      int32_t x166 = x160;
      for(int x167=0; x167 < x52; x167++) {
        int32_t x168 = 4 * x167;
        int32_t x171 = x168 + x170;
        float* x173 = (float*) &(x11[x171]);
        __m128 x174 = _mm_load_ps(x173);
        __m128 x175 = _mm_cmpge_ps(x174,x162);
        int32_t x176 = _mm_movemask_ps((__m128)x175);
        int32_t x180 = x161;
        int32_t x183 = x166;
        int32_t x181 = x179 + x180;
        int32_t x182 = x181 + x165;
        x0[x182] = x183;
        x166 += 1;
        int32_t x186 = (x176 & 1);
        x161 += x186;
        int32_t x188 = x161;
        int32_t x191 = x166;
        int32_t x189 = x179 + x188;
        int32_t x190 = x189 + x165;
        x0[x190] = x191;
        x166 += 1;
        int32_t x194 = (x176 >> 1);
        int32_t x195 = (x194 & 1);
        x161 += x195;
        int32_t x197 = x161;
        int32_t x200 = x166;
        int32_t x198 = x179 + x197;
        int32_t x199 = x198 + x165;
        x0[x199] = x200;
        x166 += 1;
        int32_t x203 = (x176 >> 2);
        int32_t x204 = (x203 & 1);
        x161 += x204;
        int32_t x206 = x161;
        int32_t x209 = x166;
        int32_t x207 = x179 + x206;
        int32_t x208 = x207 + x165;
        x0[x208] = x209;
        x166 += 1;
        int32_t x212 = (x176 >> 3);
        int32_t x213 = (x212 & 1);
        x161 += x213;
        int32_t x215 = x171 + 1;
        float* x217 = (float*) &(x11[x215]);
        __m128 x218 = _mm_load_ps(x217);
        __m128 x219 = _mm_cmpge_ps(x218,x162);
        int32_t x220 = _mm_movemask_ps((__m128)x219);
        int32_t x221 = x161;
        int32_t x224 = x166;
        int32_t x222 = x179 + x221;
        int32_t x223 = x222 + x165;
        x0[x223] = x224;
        x166 += 1;
        int32_t x227 = (x220 & 1);
        x161 += x227;
        int32_t x229 = x161;
        int32_t x232 = x166;
        int32_t x230 = x179 + x229;
        int32_t x231 = x230 + x165;
        x0[x231] = x232;
        x166 += 1;
        int32_t x235 = (x220 >> 1);
        int32_t x236 = (x235 & 1);
        x161 += x236;
        int32_t x238 = x161;
        int32_t x241 = x166;
        int32_t x239 = x179 + x238;
        int32_t x240 = x239 + x165;
        x0[x240] = x241;
        x166 += 1;
        int32_t x244 = (x220 >> 2);
        int32_t x245 = (x244 & 1);
        x161 += x245;
        int32_t x247 = x161;
        int32_t x250 = x166;
        int32_t x248 = x179 + x247;
        int32_t x249 = x248 + x165;
        x0[x249] = x250;
        x166 += 1;
        int32_t x253 = (x220 >> 3);
        int32_t x254 = (x253 & 1);
        x161 += x254;
        int32_t x256 = x171 + 2;
        float* x258 = (float*) &(x11[x256]);
        __m128 x259 = _mm_load_ps(x258);
        __m128 x260 = _mm_cmpge_ps(x259,x162);
        int32_t x261 = _mm_movemask_ps((__m128)x260);
        int32_t x262 = x161;
        int32_t x265 = x166;
        int32_t x263 = x179 + x262;
        int32_t x264 = x263 + x165;
        x0[x264] = x265;
        x166 += 1;
        int32_t x268 = (x261 & 1);
        x161 += x268;
        int32_t x270 = x161;
        int32_t x273 = x166;
        int32_t x271 = x179 + x270;
        int32_t x272 = x271 + x165;
        x0[x272] = x273;
        x166 += 1;
        int32_t x276 = (x261 >> 1);
        int32_t x277 = (x276 & 1);
        x161 += x277;
        int32_t x279 = x161;
        int32_t x282 = x166;
        int32_t x280 = x179 + x279;
        int32_t x281 = x280 + x165;
        x0[x281] = x282;
        x166 += 1;
        int32_t x285 = (x261 >> 2);
        int32_t x286 = (x285 & 1);
        x161 += x286;
        int32_t x288 = x161;
        int32_t x291 = x166;
        int32_t x289 = x179 + x288;
        int32_t x290 = x289 + x165;
        x0[x290] = x291;
        x166 += 1;
        int32_t x294 = (x261 >> 3);
        int32_t x295 = (x294 & 1);
        x161 += x295;
        int32_t x297 = x171 + 3;
        float* x299 = (float*) &(x11[x297]);
        __m128 x300 = _mm_load_ps(x299);
        __m128 x301 = _mm_cmpge_ps(x300,x162);
        int32_t x302 = _mm_movemask_ps((__m128)x301);
        int32_t x303 = x161;
        int32_t x306 = x166;
        int32_t x304 = x179 + x303;
        int32_t x305 = x304 + x165;
        x0[x305] = x306;
        x166 += 1;
        int32_t x309 = (x302 & 1);
        x161 += x309;
        int32_t x311 = x161;
        int32_t x314 = x166;
        int32_t x312 = x179 + x311;
        int32_t x313 = x312 + x165;
        x0[x313] = x314;
        x166 += 1;
        int32_t x317 = (x302 >> 1);
        int32_t x318 = (x317 & 1);
        x161 += x318;
        int32_t x320 = x161;
        int32_t x323 = x166;
        int32_t x321 = x179 + x320;
        int32_t x322 = x321 + x165;
        x0[x322] = x323;
        x166 += 1;
        int32_t x326 = (x302 >> 2);
        int32_t x327 = (x326 & 1);
        x161 += x327;
        int32_t x329 = x161;
        int32_t x332 = x166;
        int32_t x330 = x179 + x329;
        int32_t x331 = x330 + x165;
        x0[x331] = x332;
        x166 += 1;
        int32_t x335 = (x302 >> 3);
        int32_t x336 = (x335 & 1);
        x161 += x336;
      }
      //#parallel writing
  }
    for(int x156=0; x156 < x5; x156++) {
  	pthread_create(&threads[x156], NULL, parallelWriting, (void *)&inputArray[x156]);
  }
    for(int x156=0; x156 < x5; x156++) {
	pthread_join(threads[x156], NULL);
  }
    double x344 = x18 / x51;
    bool x346 = x344 == x52;
    if (x346) {
      int32_t x347 = x18 * x9;
      int32_t x348 = x347 / x8;
      x19 += x348;
    } else {
      int32_t x351 = x18 * 4;
      int32_t x352 = x351 * x5;
      x19 += x352;
    }
  } else {
  }
  int32_t x358 = x19;
  for(int x360=x358; x360 < x3; x360++) {
    int32_t x361 = x360 + x7;
    float x366 = x0[x361];
    bool x367 = x366 >= x2;
    //#decorated instruction
    // generated code for decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction without branching
    // generated code for run instruction without branching
    int32_t x362 = x1;
    int32_t x363 = x362 + x7;
    int32_t x364 = x363 + x3;
    x0[x364] = x360;
    x1 += x367;
    //#run instruction without branching
    //#decorated instruction
    //#decorated instruction
  }
  int32_t x377 = x1;
  printf("%s\n","Number of tuples found: ");
  printf("%d\n",x377);
  bool x381 = x377 == 0;
  if (x381) {
  } else {
    printf("%s\n","Output array: ");
    int32_t x165 = x7 + x3;
    for(int x384=0; x384 < x377; x384++) {
      int32_t x385 = x384 + x165;
      float x386 = x0[x385];
      printf("%f\n",x386);
    }
  }
  //#Scan Variants
}
/*****************************************
End of C Generated Code
*******************************************/
