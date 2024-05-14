#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Header files
#include <drivers/io.h>
#include <drivers/vga.h>



void main(void) {
  vga_enable_cursor(14, 15);
  vga_clear();
  vga_set_cursor(0, 0);

  // output X to vidmem
  vga_prints("Welcome to Nox!");


//  vga_prints("Testing! This is a superlong string asldkfjsldkfjldksjflkjsdlskdfjsldkfjlkjlkjl1");

  return;
}
