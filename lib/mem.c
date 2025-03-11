#include <stdint.h>
#include <lib/mem.h>
#include <drivers/vga.h>
#include <multiboot.h>
#include <lib/panic.h>

void mem_init(multiboot_info_t *mbi) {
  if (!(mbi->flags & 0x41)) panic("Essential memory information is not available.");


  memory_map_t *mmap = (memory_map_t*) mbi->mmap_addr;
  for (uint32_t mmap_size = mbi->mmap_length;
      mmap_size > 0;
      mmap_size -= mmap->size + sizeof(uint32_t),
      mmap = (memory_map_t*)((uint32_t) mmap + mmap->size + sizeof(uint32_t))) {

    // for each mmap entry
    
    printf("Start: 0x%x%x, end: 0x%x%x, size: 0x%x%x, type: %d",
       mmap->base_addr_high, mmap->base_addr_low,
       mmap->base_addr_high + mmap->length_high, mmap->base_addr_low + mmap->length_low,
       mmap->length_high, mmap->length_low,
       mmap->type);


  }

  printf("Kernel:\n start: %x, end: %x", &kernel_start, &kernel_end);
}
