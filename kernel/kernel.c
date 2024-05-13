#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Header files
#include "../drivers/vga/vga.h"
#include "../drivers/io/io.h"

// C files
#include "../drivers/io/io.c"
#include "../drivers/vga/vga.c"


void main(void) {
  enable_cursor(14, 15);

  // output X to vidmem
  putrc('N', VIDMEM_START);
  putrc('O', VIDMEM_START + (2 * VIDMEM_WIDTH) + 2);
  putc('X', 2, 2);

  set_cursor(2, 2);

  return;
}
