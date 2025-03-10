#include <stdbool.h>
#include <stdint.h>

#include <lib/kterm.h>
#include <lib/strings.h>
#include <drivers/vga.h>
#include <drivers/keyboard.h>
#include <drivers/io.h>

#include <multiboot.h>

char kterm_buf[VIDMEM_SIZE];
uint16_t kterm_buf_start = 0;
uint16_t kterm_buf_end() {
  uint16_t pos = kterm_buf_start;
  while ((vidmem[pos] & 0xff) != 0 && pos < VIDMEM_SIZE) pos++;
  return pos;
}

void kterm_buf_reset() {
  kterm_buf_start = vga_get_cursor_addr();
  for (size_t i = 0; i < VIDMEM_SIZE; i++) {
    kterm_buf[i] = 0;
  }
}

void kterm_init() {
  vga_set_cursor(0, 0);
  vga_clear();
  vga_prints("KTerm version 0.11.2\nStarting...\n\nWelcome to KTerm, Nox's built-in kernel-level shell! Type \"help\" if you don't know what you're doing.\n\n> ");
  kterm_buf_reset();
  keyboard_input_handler = kterm_main;
}

bool kterm_special_char() {
  if (kbd_buf[0] == 0x4b) {
    // left
    if (vga_get_cursor_addr() > kterm_buf_start) vga_add_cursor_x(-1);
  } else if (kbd_buf[0] == 0x4d) {
    // right
    if (vga_get_cursor_addr() < kterm_buf_end()) vga_add_cursor_x(1);
  } else if (kbd_buf[0] == 0x0e) {
    if (vga_get_cursor_addr() > kterm_buf_start) {
      vga_putcr(0, vga_get_cursor_addr() - 1);
      kterm_buf[vga_get_cursor_addr() - kterm_buf_start - 1] = 0;
      //// shift everything over??

      vga_add_cursor_x(-1);
    }
  } else if (kbd_buf[1] == '\n') {
    // run cmd
    vga_printc('\n');
    kterm_run_cmd();
    vga_prints("> ");
    kterm_buf_reset();
  } else {
    return kbd_buf[1] == 0;
  }
  return true;
}

void kterm_main() {
  if (kterm_special_char()) return;

  kterm_buf[vga_get_cursor_addr() - kterm_buf_start] = kbd_buf[1];
  vga_printc(kbd_buf[1]);
}

void kterm_run_cmd() {
  if (strcmp(kterm_buf, "test")) {
    vga_prints("SHUT UP COMPILER PLEASE AND TNAK YOU\n");
  } else if (strcmp(kterm_buf, "clear")) {
    vga_clear();
  } else if (strcmp(kterm_buf, "help")) {
    vga_prints("This command is in development :(\n");
  } else if (strncmp(kterm_buf, "echo", 4)) {
    // if there's actually something to print
    if (*(kterm_buf + 4) == ' ') vga_prints(kterm_buf + 5);
    vga_printc('\n');
  } else if (strcmp(kterm_buf, "exit")) {
    outb(0x64, 0xfe);
  } else if (strlen(kterm_buf) == 0) {
  } else {
    vga_prints("Command not found: \"");
    vga_prints(kterm_buf);
    vga_prints("\"\n");
  }
}
