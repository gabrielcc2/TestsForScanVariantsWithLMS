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
  float x5 = x4 / 4.0f;
  float x6 = x5 / 4.0f;
  float x3 = x0[0];
  int32_t x285 = x6 * 16;
  bool x286 = x285 < x4;
  //#Scan Variants
  // generated code for Scan Variants
  int32_t x1 = 0;
  int32_t x2 = 1;
  for(int x8=0; x8 < x6; x8++) {
    int32_t x9 = x8 * 16;
    int32_t x10 = x9 + 3;
    float x11 = x0[x10];
    bool x12 = x11 >= x3;
    int32_t x26 = 1 + x9;
    int32_t x27 = x26 + 3;
    float x28 = x0[x27];
    bool x29 = x28 >= x3;
    int32_t x43 = 2 + x9;
    int32_t x44 = x43 + 3;
    float x45 = x0[x44];
    bool x46 = x45 >= x3;
    int32_t x60 = 3 + x9;
    int32_t x61 = x60 + 3;
    float x62 = x0[x61];
    bool x63 = x62 >= x3;
    int32_t x77 = 4 + x9;
    int32_t x78 = x77 + 3;
    float x79 = x0[x78];
    bool x80 = x79 >= x3;
    int32_t x94 = 5 + x9;
    int32_t x95 = x94 + 3;
    float x96 = x0[x95];
    bool x97 = x96 >= x3;
    int32_t x111 = 6 + x9;
    int32_t x112 = x111 + 3;
    float x113 = x0[x112];
    bool x114 = x113 >= x3;
    int32_t x128 = 7 + x9;
    int32_t x129 = x128 + 3;
    float x130 = x0[x129];
    bool x131 = x130 >= x3;
    int32_t x145 = 8 + x9;
    int32_t x146 = x145 + 3;
    float x147 = x0[x146];
    bool x148 = x147 >= x3;
    int32_t x162 = 9 + x9;
    int32_t x163 = x162 + 3;
    float x164 = x0[x163];
    bool x165 = x164 >= x3;
    int32_t x179 = 10 + x9;
    int32_t x180 = x179 + 3;
    float x181 = x0[x180];
    bool x182 = x181 >= x3;
    int32_t x196 = 11 + x9;
    int32_t x197 = x196 + 3;
    float x198 = x0[x197];
    bool x199 = x198 >= x3;
    int32_t x213 = 12 + x9;
    int32_t x214 = x213 + 3;
    float x215 = x0[x214];
    bool x216 = x215 >= x3;
    int32_t x230 = 13 + x9;
    int32_t x231 = x230 + 3;
    float x232 = x0[x231];
    bool x233 = x232 >= x3;
    int32_t x247 = 14 + x9;
    int32_t x248 = x247 + 3;
    float x249 = x0[x248];
    bool x250 = x249 >= x3;
    int32_t x264 = 15 + x9;
    int32_t x265 = x264 + 3;
    float x266 = x0[x265];
    bool x267 = x266 >= x3;
    //#run iteration from unrolled loop
    // generated code for run iteration from unrolled loop
    //#decorated instruction
    // generated code for decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x12) {
      int32_t x13 = x1;
      int32_t x14 = x13 + 3;
      int32_t x15 = x14 + x4;
      x0[x15] = x9;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
    //#decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x29) {
      int32_t x30 = x1;
      int32_t x31 = x30 + 3;
      int32_t x32 = x31 + x4;
      x0[x32] = x26;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
    //#decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x46) {
      int32_t x47 = x1;
      int32_t x48 = x47 + 3;
      int32_t x49 = x48 + x4;
      x0[x49] = x43;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
    //#decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x63) {
      int32_t x64 = x1;
      int32_t x65 = x64 + 3;
      int32_t x66 = x65 + x4;
      x0[x66] = x60;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
    //#decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x80) {
      int32_t x81 = x1;
      int32_t x82 = x81 + 3;
      int32_t x83 = x82 + x4;
      x0[x83] = x77;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
    //#decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x97) {
      int32_t x98 = x1;
      int32_t x99 = x98 + 3;
      int32_t x100 = x99 + x4;
      x0[x100] = x94;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
    //#decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x114) {
      int32_t x115 = x1;
      int32_t x116 = x115 + 3;
      int32_t x117 = x116 + x4;
      x0[x117] = x111;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
    //#decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x131) {
      int32_t x132 = x1;
      int32_t x133 = x132 + 3;
      int32_t x134 = x133 + x4;
      x0[x134] = x128;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
    //#decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x148) {
      int32_t x149 = x1;
      int32_t x150 = x149 + 3;
      int32_t x151 = x150 + x4;
      x0[x151] = x145;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
    //#decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x165) {
      int32_t x166 = x1;
      int32_t x167 = x166 + 3;
      int32_t x168 = x167 + x4;
      x0[x168] = x162;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
    //#decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x182) {
      int32_t x183 = x1;
      int32_t x184 = x183 + 3;
      int32_t x185 = x184 + x4;
      x0[x185] = x179;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
    //#decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x199) {
      int32_t x200 = x1;
      int32_t x201 = x200 + 3;
      int32_t x202 = x201 + x4;
      x0[x202] = x196;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
    //#decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x216) {
      int32_t x217 = x1;
      int32_t x218 = x217 + 3;
      int32_t x219 = x218 + x4;
      x0[x219] = x213;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
    //#decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x233) {
      int32_t x234 = x1;
      int32_t x235 = x234 + 3;
      int32_t x236 = x235 + x4;
      x0[x236] = x230;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
    //#decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x250) {
      int32_t x251 = x1;
      int32_t x252 = x251 + 3;
      int32_t x253 = x252 + x4;
      x0[x253] = x247;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
    //#decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#decorated instruction
    // generated code for decorated instruction
    //#run instruction with branching
    // generated code for run instruction with branching
    if (x267) {
      int32_t x268 = x1;
      int32_t x269 = x268 + 3;
      int32_t x270 = x269 + x4;
      x0[x270] = x264;
      x1 += 1;
    } else {
    }
    //#run instruction with branching
    //#decorated instruction
    //#decorated instruction
    //#run iteration from unrolled loop
  }
  if (x286) {
    for(int x288=x285; x288 < x4; x288++) {
      int32_t x289 = x288 + 3;
      float x290 = x0[x289];
      bool x291 = x290 >= x3;
      //#decorated instruction
      // generated code for decorated instruction
      //#decorated instruction
      // generated code for decorated instruction
      //#run instruction with branching
      // generated code for run instruction with branching
      if (x291) {
        int32_t x292 = x1;
        int32_t x293 = x292 + 3;
        int32_t x294 = x293 + x4;
        x0[x294] = x288;
        x1 += 1;
      } else {
      }
      //#run instruction with branching
      //#decorated instruction
      //#decorated instruction
    }
  } else {
  }
  int32_t x309 = x1;
  printf("%s\n","Number of tuples found: ");
  printf("%d\n",x309);
  bool x313 = x309 == 0;
  if (x313) {
  } else {
    printf("%s\n","Output array: ");
    int32_t x315 = 3 + x4;
    for(int x317=0; x317 < x309; x317++) {
      int32_t x318 = x317 + x315;
      float x319 = x0[x318];
      printf("%f\n",x319);
    }
  }
  //#Scan Variants
}
/*****************************************
End of C Generated Code
*******************************************/
