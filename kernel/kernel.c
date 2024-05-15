#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Header files
#include <lib/idt.h>
#include <drivers/io.h>
#include <drivers/pic.h>

#include <drivers/vga.h>
#include <drivers/keyboard.h>

void main(void) {
//  idt_init();
//  pic_init();

  vga_init();
  // even with all of this commented out, I get exception 13 thrown at me.
//  keyboard_init();

  vga_prints("Welcome to Nox!");

//  while (true) {
//    keyboard_input();
//  }


  return;
}
