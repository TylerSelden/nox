#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Header files
#include <drivers/io.h>
#include <drivers/vga.h>
#include <drivers/keyboard.h>

void main(void) {
  vga_init();
  keyboard_init();

  vga_prints("Welcome to Nox!");

  while (true) {
    keyboard_input();
  }

  return;
}
