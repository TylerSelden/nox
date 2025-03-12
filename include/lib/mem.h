#ifndef LIB_MEM_C
#define LIB_MEM_C

#include <multiboot.h>

#define MAX_MMAP_ENTRIES 64

void sort_mmap_entries(multiboot_memory_map_t *entries[], uint32_t count);
void mem_init(multiboot_info_t *mbi);

#endif
