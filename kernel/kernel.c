#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

// Header files
#include <lib/panic.h>
#include <lib/idt.h>
#include <lib/strings.h>
#include <lib/kterm.h>
#include <drivers/io.h>
#include <drivers/pic.h>

#include <drivers/vga.h>
#include <drivers/keyboard.h>

#include <lib/mem.h>
#include <multiboot.h>

void main(multiboot_info_t *mbi) {
  mem_init(mbi);

  pic_init();
  idt_init();

  vga_init();
  keyboard_init();

  kterm_init();

  return;
}
