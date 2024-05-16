#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Header files
#include <lib/panic.h>
#include <lib/idt.h>
#include <drivers/io.h>
#include <drivers/pic.h>

#include <drivers/vga.h>
#include <drivers/keyboard.h>

void main(void) {
  pic_init();
  idt_init();

  vga_init();
  keyboard_init();

  vga_prints("Welcome to Nox! Type some stuff: ");

  return;
}
