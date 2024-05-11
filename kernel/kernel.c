#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void main(void) {
  // output X to vidmem
  uint16_t *vidmem = 0xb8000;
  uint8_t color = 0x0f;
  vidmem[0] = 'N' | (color << 8);
  vidmem[1] = 'o' | (color << 8);
  vidmem[2] = 'x' | (color << 8);
  
  return;
}