#ifndef LIB_MEM_C
#define LIB_MEM_C

#include <multiboot.h>

typedef struct {
  uint64_t start;
  uint64_t end;
} usable_range_t;

void mem_init(multiboot_info_t *mbi);

#endif
