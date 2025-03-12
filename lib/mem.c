#include <stdint.h>
#include <lib/mem.h>
#include <drivers/vga.h>
#include <multiboot.h>
#include <lib/panic.h>

// this can't be >255 (nor should it be)
#define MAX_ENTRIES 64

typedef struct {
  uint64_t start;
  uint64_t end;
} mem_range_t;

multiboot_memory_map_t *entries[MAX_ENTRIES];
uint8_t entry_count = 0;
mem_range_t ranges[32];
uint8_t range_count = 0;


static void sort_entries(multiboot_memory_map_t *entries[], uint32_t count) {
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

static void extract_entries(multiboot_info_t *mbi) {
  multiboot_memory_map_t *mmap = (multiboot_memory_map_t*) mbi->mmap_addr;
  uint32_t mmap_size = mbi->mmap_length;

  while (mmap_size > 0 && entry_count < MAX_ENTRIES) {
    entries[entry_count++] = mmap;
    uint32_t size = mmap->size + sizeof(uint32_t);
    mmap_size -= size;
    mmap = (multiboot_memory_map_t*)((uint32_t) mmap + size);
  }
}

static void extract_usable() {
  uint64_t kernel_start = (uint64_t)(uint32_t)&_kernel_start;
  uint64_t kernel_end  = (uint64_t)(uint32_t)&_kernel_end;

  for (uint32_t i = 0; i < entry_count; i++) {
    multiboot_memory_map_t *mmap = entries[i];

    // skip reserved regions.
    if (mmap->type != 1) continue;
    uint64_t start = mmap->addr;
    uint64_t end   = mmap->addr + mmap->len;
    if (end <= start) continue;

    // check for kernel overlaps
    if (start < kernel_end && end > kernel_start) {
      if (start < kernel_start) ranges[range_count++] = (mem_range_t){start, kernel_start};
      if (end > kernel_end) ranges[range_count++] = (mem_range_t){kernel_end, end};
    } else {
      ranges[range_count++] = (mem_range_t){start, end};
    }
  }
}

void mem_init(multiboot_info_t *mbi) {
  if (!(mbi->flags & 0x41)) panic("Essential memory information is not available.");

  extract_entries(mbi);
  sort_entries(entries, entry_count);
  extract_usable(); 


  uintptr_t size = 0;
  printf("Ussable Memory Ranges:");
  for (uint32_t i = 0; i < range_count; i++) {
    size += ranges[i].end - ranges[i].start;
    printf("Start: 0x%x, End: 0x%x, Size: %dMB", (uint32_t)ranges[i].start, (uint32_t)ranges[i].end, (uint32_t)(ranges[i].end - ranges[i].start) / 1000000);
  }
  printf("Total writable size: %dMB", size / 1000000);
}

