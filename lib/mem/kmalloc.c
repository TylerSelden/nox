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
size_t mem_blocks_free = 0;
size_t bm_len = 0;
uint8_t *bitmap = NULL;


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
    uint64_t end = mmap->addr + mmap->len;

    // prevent interference with null pointers
    if (start == 0) start += PAGE_SIZE;

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
  if ((size_t) index >= (size_t) bitmap + mem_blocks_total) panic("Kernel memory allocator tried to access a bitmap value that is out-of-bounds.");
  return (uint8_t*) bitmap + (index / 8);
}
void set_bit(uintptr_t index, bool val) {
  uint8_t *byte = get_byte(index);
  uint8_t bit = index % 8;
  if (val) *byte = *byte | (1U << bit);
  else *byte = *byte & ~(1U << bit);
}
bool get_bit(uintptr_t index) {
  uint8_t *byte = get_byte(index);
  uint8_t bit = index % 8;

  return (*byte & (1U << bit)) != 0;
}
uintptr_t get_addr(uintptr_t index) {
  if ((size_t) index >= (size_t) bitmap + mem_blocks_total) panic("Kernel memory allocator tried to access a bitmap value that is out-of-bounds.");

  uint8_t base = 0;
  while (index >= ranges[base].block_count) {
    index -= ranges[base].block_count;
    base++;
  }

  return ranges[base].start + (index * PAGE_SIZE);
}
uintptr_t get_index(uintptr_t addr) {
  uint8_t base = 0;
  while (base < range_count) {
    if (addr >= ranges[base].start && addr < ranges[base].end) {
      uintptr_t index = (addr - ranges[base].start) / PAGE_SIZE;
      for (uint8_t i = 0; i < base; i++) index += ranges[i].block_count;

      return index;
    }
    base++;
  }

  panic("Kernel memory allocator tried to access an unwritable page.");
  return 0; // keeps the compiler happy
}


void create_bitmap() {
  // calculate sizes
  for (uint32_t i = 0; i < range_count; i++) {
    size_t count = ranges[i].size / PAGE_SIZE;
    mem_blocks_total += count;
    ranges[i].block_count = count;
  }
  mem_blocks_total = (mem_blocks_total / 8) * 8;
  mem_blocks_free = mem_blocks_total;
  bm_len = mem_blocks_total / 8;

  // find a safe place to put the index
  for (uint8_t i = 0; i < range_count; i++) {
    if (ranges[i].size >= bm_len) {
      bitmap = (uint8_t*) ranges[i].start;
      break;
    }
  }
  if (bitmap == NULL) panic("Usable memory segments are too small for the memory allocation bitmap.");

  // zero out bitmap and reserve pages containing it
  for (uint8_t *i = bitmap; i < (uint8_t*) (bitmap + bm_len); i++) *i = 0;
  size_t bm_pages = (bm_len + PAGE_SIZE - 1) / PAGE_SIZE;
  for (uintptr_t i = 0; i < bm_pages; i++) set_bit(i, 1);
  mem_blocks_free -= bm_pages;
}



void mem_init(multiboot_info_t *mbi) {
  if (!(mbi->flags & 0x41)) panic("Essential memory information is not available.");

  extract_entries(mbi);
  sort_entries(entries, entry_count);
  extract_usable();
  create_bitmap();
}

void *kmalloc() {
  for (uint8_t *i = bitmap; i < (uint8_t*) (bitmap + bm_len); i++) {
    if (*i == 0xff) continue;

    // figure out which bit is a 0
    uintptr_t base = ((uintptr_t) i - (uintptr_t) bitmap) * 8;
    for (uintptr_t j = base; j < base + 8; j++) {
      if (get_bit(j) == 0) {
        set_bit(j, 1);
        mem_blocks_free--;
        return (void*) get_addr(j);
      }
    }
  }
  return 0;
}

void kfree(uintptr_t addr) {
  uintptr_t index = get_index(addr);
  if (!get_bit(index)) return;

  set_bit(index, 0);
  mem_blocks_free++;
}
