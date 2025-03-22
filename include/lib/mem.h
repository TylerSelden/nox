#ifndef LIB_MEM_H
#define LIB_MEM_H

#include <multiboot.h>

#define PAGE_SIZE 4096

extern size_t mem_blocks_total;
extern size_t mem_blocks_free;

void kmalloc_init(multiboot_info_t *mbi);
void *kmalloc();
void kfree(uintptr_t addr);

void vmalloc_init();

#endif
