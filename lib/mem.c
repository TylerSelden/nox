#include <stdint.h>
#include <lib/mem.h>
#include <drivers/vga.h>
#include <multiboot.h>
#include <lib/panic.h>

#include <stdint.h>
#include <lib/mem.h>
#include <drivers/vga.h>
#include <multiboot.h>
#include <lib/panic.h>

void mem_init(multiboot_info_t *mbi) {
  if (!(mbi->flags & 0x41)) {
    panic("Essential memory information is not available.");
  }

  uint32_t mmap_size = mbi->mmap_length;
  memory_map_t *mmap = (memory_map_t *)mbi->mmap_addr;

  printf("Test %d", 10);

  vga_prints("Memory Map:\n");
  vga_prints("Mem_lower: ");
  vga_printi(mbi->mem_lower, 16);
  vga_prints("\nMem_upper: ");
  vga_printi(mbi->mem_upper, 16);
  vga_printc('\n');

  // Loop through the memory map entries
  while (mmap_size > 0) {
    vga_prints("Base Address: 0x");
    vga_printi(mmap->base_addr_high, 16);
    vga_printi(mmap->base_addr_low, 16);

    vga_prints("; Length: 0x");
    vga_printi(mmap->length_high, 16);
    vga_printi(mmap->length_low, 16);

    vga_prints("; Type: ");
    vga_printi(mmap->type, 10);
    vga_printc('\n');

    mmap_size -= mmap->size + sizeof(uint32_t);
    mmap = (memory_map_t *)((uint32_t)mmap + mmap->size + sizeof(uint32_t));
  }
}

