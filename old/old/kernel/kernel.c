#define VIDMEM_ADDR 0xb8000
#define MAX_ROWS 25
#define MAX_COLS 80
#define MAX_VIDMEM_LEN MAX_ROWS * MAX_COLS * 2

#include "../libc/printf.h"


void main() {
  char* vidmem = (char*) 0xb8000;
  *vidmem = 'S';
  
  // string
  char str[] = "this is a super long string, i want to see how long it can get";
  print(str);
}
