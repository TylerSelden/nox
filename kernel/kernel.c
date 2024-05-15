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
  pic_init();
  idt_init();

  vga_init();
  keyboard_init();

  vga_prints("Welcome to Nox!");

// no longer needed :)
/*  
  while (true) {
    // dev keybinds
    if (kbd_buf[1] == 'A') vga_puti(1 / 0, 10, 11, 11);
    if (kbd_buf[1] == 'B') vga_write_row(24, ' ');
    kbd_buf[1] = 0;
  }
*/

  return;
}
