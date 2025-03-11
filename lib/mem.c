#include <stdint.h>
#include <lib/mem.h>
#include <drivers/vga.h>
#include <multiboot.h>
#include <lib/panic.h>

typedef struct {
  uint64_t start;
  uint64_t end;
} usable_range_t;

void mem_init(multiboot_info_t *mbi) {
  if (!(mbi->flags & 0x41)) panic("Essential memory information is not available.");

  memory_map_t *mmap = (memory_map_t*) mbi->mmap_addr;
  uint32_t mmap_size = mbi->mmap_length;

  usable_range_t ranges[32];
  int range_count = 0;

  uint64_t kernel_start = (uint64_t)(uint32_t)&_kernel_start;
  uint64_t kernel_end = (uint64_t)(uint32_t)&_kernel_end;

  // process all entries, merging where necessary
  for (; mmap_size > 0;
  mmap_size -= mmap->size + sizeof(uint32_t),
  mmap = (memory_map_t*)((uint32_t) mmap + mmap->size + sizeof(uint32_t))) {

    // skip reserved reigons
    if (mmap->type != 1) continue;
    uint64_t start = ((uint64_t)mmap->base_addr_high << 32) | mmap->base_addr_low;
    uint64_t end = start + (((uint64_t)mmap->length_high << 32) | mmap->length_low);
    if (end <= start) continue;

    // split for kernel if necessary
    if (start < kernel_end && end > kernel_start) {
      if (start < kernel_start) ranges[range_count++] = (usable_range_t){start, kernel_start};
      if (end > kernel_end) ranges[range_count++] = (usable_range_t){kernel_end, end};
    } else {
      ranges[range_count++] = (usable_range_t){start, end};
    }
  }

  printf("Usable Memory Ranges:");
  for (int i = 0; i < range_count; i++) {
    printf("Start: 0x%x, End: 0x%x", (uint32_t)ranges[i].start, (uint32_t)ranges[i].end);
  }
}

