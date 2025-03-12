#ifndef LIB_MEM_H
#define LIB_MEM_H

#include <multiboot.h>

extern uintptr_t mem_amt;

void mem_init(multiboot_info_t *mbi);

#endif
