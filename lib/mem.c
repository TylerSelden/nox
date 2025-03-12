#include <stdint.h>
#include <lib/mem.h>
#include <drivers/vga.h>
#include <multiboot.h>
#include <lib/panic.h>

void sort_mmap_entries(multiboot_memory_map_t *entries[], uint32_t count) {
  for (uint32_t i = 0; i < count - 1; i++) {
    for (uint32_t j = 0; j < count - i - 1; j++) {
      if (entries[j]->addr > entries[j + 1]->addr) {
        multiboot_memory_map_t *tmp = entries[j];
        entries[j] = entries[j + 1];
        entries[j + 1] = tmp;
      }
    }
  }
}

void mem_init(multiboot_info_t *mbi) {
  if (!(mbi->flags & 0x41)) panic("Essential memory information is not available.");

  multiboot_memory_map_t *mmap_ptr = (multiboot_memory_map_t*) mbi->mmap_addr;
  uint32_t mmap_size = mbi->mmap_length;
  multiboot_memory_map_t *mmap_entries[MAX_MMAP_ENTRIES];
  uint32_t entry_count = 0;

  while (mmap_size > 0 && entry_count < MAX_MMAP_ENTRIES) {
    mmap_entries[entry_count++] = mmap_ptr;
    uint32_t entry_size = mmap_ptr->size + sizeof(uint32_t);
    mmap_size -= entry_size;
    mmap_ptr = (multiboot_memory_map_t*)((uint32_t)mmap_ptr + entry_size);
  }

  sort_mmap_entries(mmap_entries, entry_count);

  // sorting is done, process the entries
  usable_range_t ranges[32];
  uint32_t range_count = 0;

  uint64_t kernel_start = (uint64_t)(uint32_t)&_kernel_start;
  uint64_t kernel_end  = (uint64_t)(uint32_t)&_kernel_end;

  for (uint32_t i = 0; i < entry_count; i++) {
    multiboot_memory_map_t *mmap = mmap_entries[i];

    // skip reserved regions.
    if (mmap->type != 1) continue;
    uint64_t start = mmap->addr;
    uint64_t end   = mmap->addr + mmap->len;
    if (end <= start) continue;

    // check for kernel overlaps
    if (start < kernel_end && end > kernel_start) {
      if (start < kernel_start) ranges[range_count++] = (usable_range_t){start, kernel_start};
      if (end > kernel_end) ranges[range_count++] = (usable_range_t){kernel_end, end};
    } else {
      ranges[range_count++] = (usable_range_t){start, end};
    }
  }

  uintptr_t size = 0;
  printf("Usable Memory Ranges:");
  for (uint32_t i = 0; i < range_count; i++) {
    size += ranges[i].end - ranges[i].start;
    printf("Start: 0x%x, End: 0x%x, Size: %dMB", (uint32_t)ranges[i].start, (uint32_t)ranges[i].end, (uint32_t)(ranges[i].end - ranges[i].start) / 1000000);
  }
  printf("Total writable size: %dMB", size / 1000000);
}

