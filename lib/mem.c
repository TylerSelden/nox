#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <lib/mem.h>
#include <drivers/vga.h>
#include <multiboot.h>
#include <lib/panic.h>

// this can't be >255 (nor should it be)
#define MAX_ENTRIES 64

typedef struct {
  uintptr_t start;
  uintptr_t end;
  size_t size;
  size_t block_count;
} mem_range_t;

multiboot_memory_map_t *entries[MAX_ENTRIES];
uint8_t entry_count = 0;
mem_range_t ranges[MAX_ENTRIES];
uint8_t range_count = 0;
size_t mem_blocks_total = 0;
size_t bm_len = 0;
uint8_t *bitmap = (uint8_t*) -1;


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
      if (start < kernel_start) ranges[range_count++] = (mem_range_t){start, kernel_start, kernel_start - start, 0};
      if (end > kernel_end) ranges[range_count++] = (mem_range_t){kernel_end, end, end - kernel_end, 0};
    } else {
      ranges[range_count++] = (mem_range_t){start, end, end - start, 0};
    }
  }
}

uint8_t *get_byte(uintptr_t index) {
  if ((size_t) index >= bm_len) {
    panic("Kernel memory allocator accessed a byte out-of-bounds.");
  }

  index /= 8;

  uint8_t *byte = (uint8_t*) (bitmap + index);
  return byte;
}

void set_bit(uintptr_t index, bool val) {
  uint8_t *byte = get_byte(index);
  uint8_t bit = index % 8;
  if (val) {
    *byte = *byte | (1U << bit);
  } else {
    *byte = *byte & ~(1U << bit);
  }
}
bool get_bit(uintptr_t index) {
  uint8_t *byte = get_byte(index);
  uint8_t bit = index % 8;

  return (*byte & (1U << bit)) != 0;
}



void create_bitmap() {
  // calculate sizes
  for (uint32_t i = 0; i < range_count; i++) {
    size_t count = ranges[i].size / PAGE_SIZE;
    mem_blocks_total += count;
    ranges[i].block_count = count;
  }
  bm_len = mem_blocks_total / 8;

  // find a safe place to put the index
  for (uint8_t i = 0; i < range_count; i++) {
    if (ranges[i].size >= bm_len) {
      bitmap = (uint8_t*) ranges[i].start;
      break;
    }
  }
  if (bitmap == (uint8_t*) -1) panic("Usable memory segments are too small for the memory allocation bitmap.");

  // zero out bitmap
  for (uint8_t *i = bitmap; i < (uint8_t*) (bitmap + bm_len); i++) *i = 0;

  // reserve the section that contains the bitmap (NOT ALWAYS the first block)
  size_t bm_pages = (bm_len + PAGE_SIZE - 1) / PAGE_SIZE;
  for (uintptr_t i = 0; i < bm_pages; i++) set_bit(i, 1);
}

void mem_init(multiboot_info_t *mbi) {
  if (!(mbi->flags & 0x41)) panic("Essential memory information is not available.");

  extract_entries(mbi);
  sort_entries(entries, entry_count);
  extract_usable();
  create_bitmap();

  // test stuff
  printf("%dMB allocated, with %d bit (%d page) bitmap.",
         (mem_blocks_total * PAGE_SIZE) / 1048576,
         bm_len,
         (bm_len + PAGE_SIZE - 1) / PAGE_SIZE);
  for (int i = 0; i < 8; i++) {
    printf("Test bit %d: %d", i, get_bit(i));
  }
}

