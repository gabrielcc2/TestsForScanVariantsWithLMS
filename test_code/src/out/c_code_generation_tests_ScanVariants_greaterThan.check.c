#include <fcntl.h>
#include <errno.h>
#include <err.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
void Scan(float*);
int main(int argc, char *argv[])
{
  if (argc < 3) {
    printf("Missing arguments. Usage: filename numberOfTuples compareValue\n");
    return 0;
  }
  FILE *ptr_file;
  char buf[1000];
  int numTuples=atoi(argv[2]);
  float compareValue=atof(argv[3]);
  ptr_file =fopen(argv[1],"r");
  if (!ptr_file){
    return 0;
  }
  int numReadTuples=0;
  float *array;
  array=(float*)malloc(((2*numTuples)+2)*sizeof(float));
  array[0]=compareValue;
  array[1]=(float)numTuples;
  while (fgets(buf,1000, ptr_file)!=NULL && numReadTuples<numTuples){
    array[numReadTuples+2]=atof(buf);
    numReadTuples++;
  }
  fclose(ptr_file);
  if (numReadTuples<numTuples){
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
  float x4 = x0[0];
  int32_t x144 = x3 + 2;
  bool x145 = 18 < x144;
  //#Scan Variants- timer goes here
  // generated code for Scan Variants- timer goes here
  int32_t x1 = 2;
  int32_t x2 = 1;
  for(int x12=0; x12 < 1; x12++) {
    int32_t x13 = x12 * 16;
    int32_t x14 = x13 + 2;
    float x17 = x0[x14];
    bool x19 = x17 > x4;
    int32_t x21 = x13 + 1;
    int32_t x22 = x21 + 2;
    float x25 = x0[x22];
    bool x27 = x25 > x4;
    int32_t x29 = x14 + 2;
    float x32 = x0[x29];
    bool x34 = x32 > x4;
    int32_t x36 = x13 + 3;
    int32_t x37 = x36 + 2;
    float x40 = x0[x37];
    bool x42 = x40 > x4;
    int32_t x44 = x13 + 4;
    int32_t x45 = x44 + 2;
    float x48 = x0[x45];
    bool x50 = x48 > x4;
    int32_t x52 = x13 + 5;
    int32_t x53 = x52 + 2;
    float x56 = x0[x53];
    bool x58 = x56 > x4;
    int32_t x60 = x13 + 6;
    int32_t x61 = x60 + 2;
    float x64 = x0[x61];
    bool x66 = x64 > x4;
    int32_t x68 = x13 + 7;
    int32_t x69 = x68 + 2;
    float x72 = x0[x69];
    bool x74 = x72 > x4;
    int32_t x76 = x13 + 8;
    int32_t x77 = x76 + 2;
    float x80 = x0[x77];
    bool x82 = x80 > x4;
    int32_t x84 = x13 + 9;
    int32_t x85 = x84 + 2;
    float x88 = x0[x85];
    bool x90 = x88 > x4;
    int32_t x92 = x13 + 10;
    int32_t x93 = x92 + 2;
    float x96 = x0[x93];
    bool x98 = x96 > x4;
    int32_t x100 = x13 + 11;
    int32_t x101 = x100 + 2;
    float x104 = x0[x101];
    bool x106 = x104 > x4;
    int32_t x108 = x13 + 12;
    int32_t x109 = x108 + 2;
    float x112 = x0[x109];
    bool x114 = x112 > x4;
    int32_t x116 = x13 + 13;
    int32_t x117 = x116 + 2;
    float x120 = x0[x117];
    bool x122 = x120 > x4;
    int32_t x124 = x13 + 14;
    int32_t x125 = x124 + 2;
    float x128 = x0[x125];
    bool x130 = x128 > x4;
    int32_t x132 = x13 + 15;
    int32_t x133 = x132 + 2;
    float x136 = x0[x133];
    bool x138 = x136 > x4;
    //#run instruction
    // generated code for run instruction
    int32_t x15 = x1;
    int32_t x16 = x15 + x3;
    x0[x16] = x17;
    x1 += x19;
    int32_t x23 = x1;
    int32_t x24 = x23 + x3;
    x0[x24] = x25;
    x1 += x27;
    int32_t x30 = x1;
    int32_t x31 = x30 + x3;
    x0[x31] = x32;
    x1 += x34;
    int32_t x38 = x1;
    int32_t x39 = x38 + x3;
    x0[x39] = x40;
    x1 += x42;
    int32_t x46 = x1;
    int32_t x47 = x46 + x3;
    x0[x47] = x48;
    x1 += x50;
    int32_t x54 = x1;
    int32_t x55 = x54 + x3;
    x0[x55] = x56;
    x1 += x58;
    int32_t x62 = x1;
    int32_t x63 = x62 + x3;
    x0[x63] = x64;
    x1 += x66;
    int32_t x70 = x1;
    int32_t x71 = x70 + x3;
    x0[x71] = x72;
    x1 += x74;
    int32_t x78 = x1;
    int32_t x79 = x78 + x3;
    x0[x79] = x80;
    x1 += x82;
    int32_t x86 = x1;
    int32_t x87 = x86 + x3;
    x0[x87] = x88;
    x1 += x90;
    int32_t x94 = x1;
    int32_t x95 = x94 + x3;
    x0[x95] = x96;
    x1 += x98;
    int32_t x102 = x1;
    int32_t x103 = x102 + x3;
    x0[x103] = x104;
    x1 += x106;
    int32_t x110 = x1;
    int32_t x111 = x110 + x3;
    x0[x111] = x112;
    x1 += x114;
    int32_t x118 = x1;
    int32_t x119 = x118 + x3;
    x0[x119] = x120;
    x1 += x122;
    int32_t x126 = x1;
    int32_t x127 = x126 + x3;
    x0[x127] = x128;
    x1 += x130;
    int32_t x134 = x1;
    int32_t x135 = x134 + x3;
    x0[x135] = x136;
    x1 += x138;
    //#run instruction
  }
  if (x145) {
    for(int x147=18; x147 < x144; x147++) {
      float x150 = x0[x147];
      bool x152 = x150 > x4;
      //#run residue instructions after unroll
      // generated code for run residue instructions after unroll
      int32_t x148 = x1;
      int32_t x149 = x148 + x3;
      x0[x149] = x150;
      x1 += x152;
      //#run residue instructions after unroll
    }
  } else {
  }
  printf("%s\n","Output array: ");
  int32_t x161 = x1;
  int32_t x162 = x161 - 2;
  for(int x164=0; x164 < x162; x164++) {
    int32_t x165 = x144 + x164;
    float x166 = x0[x165];
    printf("%f\n",x166);
  }
  //#Scan Variants- timer goes here
}
/*****************************************
End of C Generated Code
*******************************************/
