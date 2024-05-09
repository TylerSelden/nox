// Default libs

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// ============================== Headers ==============================
// libs
#include "../libc/err/err.h"
#include "../libc/tty/tty.h"

// drivers
#include "../drivers/io/io.h"
#include "../drivers/vga/vga.h"
#include "../drivers/keyboard/keyboard.h"

// ==============================   Code  ==============================
// libs
#include "../libc/err/err.c"
#include "../libc/tty/tty.c"

// drivers
#include "../drivers/io/io.c"
#include "../drivers/vga/vga.c"
#include "../drivers/keyboard/keyboard.c"



void main() {
  // disable cursor?
  vgaInit();
  enableKeyboard();
  ttyInit();


  while (true) {
    handleKeyboardInput();
    tty_handleInput();
  }
}

