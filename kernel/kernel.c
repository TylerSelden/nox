// Default libs

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
 
// included libs
#include "./libc/io.c"
#include "./libc/vga.c"

void main() {
  // disable cursor?
  vga_init();

  vga_printf("Coordinates of cursor:");
  vga_print("X: ");
  vga_printHexf(vga_getCursorX());
  vga_print("Y: ");
  vga_printHexf(vga_getCursorY());
}
