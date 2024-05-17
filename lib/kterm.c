#include <stdbool.h>
#include <stdint.h>

#include <lib/kterm.h>
#include <drivers/keyboard.h>
#include <drivers/vga.h>

uint16_t kterm_buf_start = 0;

uint16_t kterm_buf_end() {
  uint16_t addr = kterm_buf_start;
  while ((char) vidmem[addr++] != 0 && addr < VIDMEM_SIZE);
  return addr - 1;
}


void kterm_start() {
  keyboard_input_handler = &kterm_main;
  vga_prints("KTerm v0.00.1\n\nWelcome to KTerm, Nox's built-in kernel shell. If you need help, type `help`.\n\n> ");

  kterm_buf_start = vga_get_cursor_addr();
}

void kterm_main() {
  if (kterm_special_key()) return;

  if (kbd_buf[1] == 'A') {
    vga_putsr("| KTerm buffer end!", kterm_buf_end());
  } else {
    vga_printc(kbd_buf[1]);
  }
}



bool kterm_special_key() {
  if (kbd_buf[0] == 0x4b) {
    if (vga_get_cursor_addr() - 1 < kterm_buf_start) return true;
    vga_add_cursor_x(-1);
  } else if (kbd_buf[0] == 0x4d) {
    if ((char) vidmem[vga_get_cursor_addr()] == 0) return true;
    vga_add_cursor_x(1);
  } else if (kbd_buf[1] == '\n') {
    // run command
  } else {
    return (kbd_buf[1] == 0);
  }
  return true;
}
