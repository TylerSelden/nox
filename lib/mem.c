#include <stdint.h>
#include <lib/mem.h>
#include <drivers/vga.h>
#include <multiboot.h>
#include <lib/panic.h>

void mem_init(multiboot_info_t *mbi) {
  if (!(mbi->flags & 0x1)) {
    panic("No memory map is available.");
  }
}

