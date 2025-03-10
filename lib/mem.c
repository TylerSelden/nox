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

  printf("\nMemory map:\n mem_lower: 0x%x\n mem_upper: 0x%x\n", mbi->mem_lower, mbi->mem_upper);
  printf("Kernel memory:\n lower: %x\n upper: %x\n", &kernel_start, &kernel_end);

  // Loop through the memory map entries
  while (mmap_size > 0) {
    printf("Addr: 0x%x%x; Length: 0x%x%x; Type: %x",
               mmap->base_addr_high, mmap->base_addr_low,
               mmap->length_high, mmap->length_low,
               mmap->type);

    mmap_size -= mmap->size + sizeof(uint32_t);
    mmap = (memory_map_t *)((uint32_t)mmap + mmap->size + sizeof(uint32_t));
  }
}

