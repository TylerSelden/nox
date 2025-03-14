#ifndef LIB_MEM_H
#define LIB_MEM_H

#include <multiboot.h>

#define PAGE_SIZE 4096

extern uintptr_t mem_blocks_total;

void mem_init(multiboot_info_t *mbi);

#endif
