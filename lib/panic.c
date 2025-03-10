#include <stdint.h>
#include <lib/panic.h>
#include <drivers/vga.h>

void panic(char *str) {
  color = 0x4f;
  vga_clear();

  vga_set_cursor((VIDMEM_WIDTH / 2) - 8, 1);
  vga_prints("! KERNEL PANIC !");

  vga_set_cursor(1, 3);
  vga_prints("A kernel panic has occurred, likely initiated by code.");

  vga_set_cursor(1, 7);
  vga_prints("Reason given:");

  vga_set_cursor(1, 9);
  vga_prints(str);

  vga_set_cursor(1, 18);
  vga_prints("Hopefully this was intentional, or at the very least an easy fix.");

  vga_set_cursor(1, VIDMEM_HEIGHT - 4);
  vga_prints("The OS will halt now.");

  vga_set_cursor((VIDMEM_WIDTH / 2) - 3, VIDMEM_HEIGHT - 1);
  vga_prints("Nox OS");

  vga_disable_cursor();

  __asm__ volatile ("cli; hlt");
}

void panic_exc(uint8_t num, uint8_t err) {
  color = 0x4f;
  vga_clear();

  vga_set_cursor((VIDMEM_WIDTH / 2) - 8, 1);
  vga_prints("! KERNEL PANIC !");

  vga_set_cursor(1, 3);
  vga_prints("A kernel panic has occurred, likely from an exception.");

  vga_set_cursor(1, 7);
  vga_prints("Exception number: 0x");
  vga_printi(num, 16);
  vga_prints(" (");
  vga_printi(num, 10);
  vga_prints(")");

  vga_set_cursor(1, 9);
  vga_prints("Exception error code: 0x");
  vga_printi(err, 16);
  vga_prints(" (");
  vga_printi(err, 10);
  vga_prints(")");

  vga_set_cursor(1, 12);
  vga_prints("Hopefully this was intentional, or at the very least an easy fix.");

  vga_set_cursor(1, VIDMEM_HEIGHT - 4);
  vga_prints("The OS will halt now.");

  vga_set_cursor((VIDMEM_WIDTH / 2) - 3, VIDMEM_HEIGHT - 1);
  vga_prints("Nox OS");

  vga_disable_cursor();

  __asm__ volatile ("cli; hlt");
}
