// Default libs

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
 
// included libs
#include "../drivers/io/io.c"
#include "../drivers/vga/vga.c"
#include "../drivers/keyboard/keyboard.c"

void main() {
  // disable cursor?
  vga_init();
  
  vga_printf("Welcome to Nox!");
  vga_printf("\nThis is the beginnings of an amateur-made OS. At the moment, it can't do much.");
  vga_printf("\nHopefully soon it can support some kind of filesystem, and commands, but in the meantime, enjoy.\n");
  vga_print("> ");
  enableKeyboard();

  while (true) {
    handleKeyboardInput();
  }
}
