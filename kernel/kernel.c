#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Header files
#include <drivers/io.h>
#include <drivers/vga.h>



void main(void) {
  vga_init();

  vga_prints("Welcome to Nox!");



  return;
}
