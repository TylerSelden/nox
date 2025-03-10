#include <stdint.h>
#include <lib/panic.h>
#include <drivers/vga.h>

void panic(char *str) {
  color = 0x4f;
  vga_clear();

  vga_set_cursor((VIDMEM_WIDTH / 2) - 8, 1);
  printf("! KERNEL PANIC !");

  vga_set_cursor(1, 3);
  printf("A kernel panic has occurred, likely initiated by code.");

  vga_set_cursor(1, 7);
  printf("Reason given:");

  vga_set_cursor(1, 9);
  printf("%s", str);

  vga_set_cursor(1, 18);
  printf("Hopefully this was intentional, or at the very least an easy fix.");

  vga_set_cursor(1, VIDMEM_HEIGHT - 4);
  printf("The OS will halt now.");

  vga_set_cursor((VIDMEM_WIDTH / 2) - 3, VIDMEM_HEIGHT - 1);
  printf("Nox OS");

  vga_disable_cursor();

  __asm__ volatile ("cli; hlt");
}

void panic_exc(uint8_t num, uint8_t err) {
  color = 0x4f;
  vga_clear();

  vga_set_cursor((VIDMEM_WIDTH / 2) - 8, 1);
  printf("! KERNEL PANIC !");

  vga_set_cursor(1, 3);
  printf("A kernel panic has occurred, likely from an exception.");

  vga_set_cursor(1, 7);
  printf("Exception number: 0x%x (%d)", num, num);

  vga_set_cursor(1, 9);
  printf("Exception error code: 0x%x (%d)", err, err);

  vga_set_cursor(1, 12);
  printf("Hopefully this was intentional, or at the very least an easy fix.");

  vga_set_cursor(1, VIDMEM_HEIGHT - 4);
  printf("The OS will halt now.");

  vga_set_cursor((VIDMEM_WIDTH / 2) - 3, VIDMEM_HEIGHT - 1);
  printf("Nox OS");

  vga_disable_cursor();

  __asm__ volatile ("cli; hlt");
}

