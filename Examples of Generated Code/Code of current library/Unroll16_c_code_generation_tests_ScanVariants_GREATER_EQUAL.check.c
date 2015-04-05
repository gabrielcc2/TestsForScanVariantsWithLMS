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
  float x5 = x4 / 4.0f;
  float x2 = x0[0];
  int32_t x252 = x5 * 16;
  //#Scan Variants
  // generated code for Scan Variants
  int32_t x1 = 0;
  for(int x7=0; x7 < x5; x7++) {
    int32_t x8 = x7 * 16;
    int32_t x9 = x8 + 3;
    float x10 = x0[x9];
    bool x11 = x10 >= x2;
    int32_t x23 = 1 + x8;
    int32_t x24 = x23 + 3;
    float x25 = x0[x24];
    bool x26 = x25 >= x2;
    int32_t x38 = 2 + x8;
    int32_t x39 = x38 + 3;
    float x40 = x0[x39];
    bool x41 = x40 >= x2;
    int32_t x53 = 3 + x8;
    int32_t x54 = x53 + 3;
    float x55 = x0[x54];
    bool x56 = x55 >= x2;
    int32_t x68 = 4 + x8;
    int32_t x69 = x68 + 3;
    float x70 = x0[x69];
    bool x71 = x70 >= x2;
    int32_t x83 = 5 + x8;
    int32_t x84 = x83 + 3;
    float x85 = x0[x84];
    bool x86 = x85 >= x2;
    int32_t x98 = 6 + x8;
    int32_t x99 = x98 + 3;
    float x100 = x0[x99];
    bool x101 = x100 >= x2;
    int32_t x113 = 7 + x8;
    int32_t x114 = x113 + 3;
    float x115 = x0[x114];
    bool x116 = x115 >= x2;
    int32_t x128 = 8 + x8;
    int32_t x129 = x128 + 3;
    float x130 = x0[x129];
    bool x131 = x130 >= x2;
    int32_t x143 = 9 + x8;
    int32_t x144 = x143 + 3;
    float x145 = x0[x144];
    bool x146 = x145 >= x2;
    int32_t x158 = 10 + x8;
    int32_t x159 = x158 + 3;
    float x160 = x0[x159];
    bool x161 = x160 >= x2;
    int32_t x173 = 11 + x8;
    int32_t x174 = x173 + 3;
    float x175 = x0[x174];
    bool x176 = x175 >= x2;
    int32_t x188 = 12 + x8;
    int32_t x189 = x188 + 3;
    float x190 = x0[x189];
    bool x191 = x190 >= x2;
    int32_t x203 = 13 + x8;
    int32_t x204 = x203 + 3;
    float x205 = x0[x204];
    bool x206 = x205 >= x2;
    int32_t x218 = 14 + x8;
    int32_t x219 = x218 + 3;
    float x220 = x0[x219];
    bool x221 = x220 >= x2;
    int32_t x233 = 15 + x8;
    int32_t x234 = x233 + 3;
    float x235 = x0[x234];
    bool x236 = x235 >= x2;
    //#run iteration from unrolled loop
    // generated code for run iteration from unrolled loop
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x11) {
      int32_t x12 = x1;
      int32_t x13 = x12 + 3;
      int32_t x14 = x13 + x3;
      x0[x14] = x8;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x26) {
      int32_t x27 = x1;
      int32_t x28 = x27 + 3;
      int32_t x29 = x28 + x3;
      x0[x29] = x23;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x41) {
      int32_t x42 = x1;
      int32_t x43 = x42 + 3;
      int32_t x44 = x43 + x3;
      x0[x44] = x38;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x56) {
      int32_t x57 = x1;
      int32_t x58 = x57 + 3;
      int32_t x59 = x58 + x3;
      x0[x59] = x53;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x71) {
      int32_t x72 = x1;
      int32_t x73 = x72 + 3;
      int32_t x74 = x73 + x3;
      x0[x74] = x68;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x86) {
      int32_t x87 = x1;
      int32_t x88 = x87 + 3;
      int32_t x89 = x88 + x3;
      x0[x89] = x83;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x101) {
      int32_t x102 = x1;
      int32_t x103 = x102 + 3;
      int32_t x104 = x103 + x3;
      x0[x104] = x98;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x116) {
      int32_t x117 = x1;
      int32_t x118 = x117 + 3;
      int32_t x119 = x118 + x3;
      x0[x119] = x113;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x131) {
      int32_t x132 = x1;
      int32_t x133 = x132 + 3;
      int32_t x134 = x133 + x3;
      x0[x134] = x128;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x146) {
      int32_t x147 = x1;
      int32_t x148 = x147 + 3;
      int32_t x149 = x148 + x3;
      x0[x149] = x143;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x161) {
      int32_t x162 = x1;
      int32_t x163 = x162 + 3;
      int32_t x164 = x163 + x3;
      x0[x164] = x158;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x176) {
      int32_t x177 = x1;
      int32_t x178 = x177 + 3;
      int32_t x179 = x178 + x3;
      x0[x179] = x173;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x191) {
      int32_t x192 = x1;
      int32_t x193 = x192 + 3;
      int32_t x194 = x193 + x3;
      x0[x194] = x188;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x206) {
      int32_t x207 = x1;
      int32_t x208 = x207 + 3;
      int32_t x209 = x208 + x3;
      x0[x209] = x203;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x221) {
      int32_t x222 = x1;
      int32_t x223 = x222 + 3;
      int32_t x224 = x223 + x3;
      x0[x224] = x218;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x236) {
      int32_t x237 = x1;
      int32_t x238 = x237 + 3;
      int32_t x239 = x238 + x3;
      x0[x239] = x233;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
    //#run iteration from unrolled loop
  }
  for(int x254=x252; x254 < x3; x254++) {
    int32_t x255 = x254 + 3;
    float x256 = x0[x255];
    bool x257 = x256 >= x2;
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x257) {
      int32_t x258 = x1;
      int32_t x259 = x258 + 3;
      int32_t x260 = x259 + x3;
      x0[x260] = x254;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
  }
  int32_t x271 = x1;
  printf("%s\n","Number of tuples found: ");
  printf("%d\n",x271);
  bool x275 = x271 == 0;
  if (x275) {
  } else {
    printf("%s\n","Output array: ");
    int32_t x277 = 3 + x3;
    for(int x279=0; x279 < x271; x279++) {
      int32_t x280 = x279 + x277;
      float x281 = x0[x280];
      printf("%f\n",x281);
    }
  }
  //#Scan Variants
}
/*****************************************
End of C Generated Code
*******************************************/
