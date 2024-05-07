// Default libs

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// ============================== Headers ==============================
// libs
#include "../libc/err/err.h"

// drivers
#include "../drivers/io/io.h"
#include "../drivers/vga/vga.h"
#include "../drivers/keyboard/keyboard.h"

// ==============================   Code  ==============================
// libs
#include "../libc/err/err.c"

// drivers
#include "../drivers/io/io.c"
#include "../drivers/vga/vga.c"
#include "../drivers/keyboard/keyboard.c"


void main() {
  // disable cursor?
  vgaInit();
  enableKeyboard();

  printf("Welcome to Nox!");
  printf("\nThis is the beginnings of an amateur-made OS. At the moment, it can't do much.");
  printf("\nHopefully soon it can support some kind of filesystem, and commands, but in the meantime, enjoy.\n");
  print("> ");


  while (true) {
    handleKeyboardInput();
  }
}

