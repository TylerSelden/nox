#ifndef LIB_MEM_C
#define LIB_MEM_C

#include <multiboot.h>

typedef struct {
    unsigned int size;        // Size of the entry
    unsigned int base_addr;   // Base address of the region
    unsigned int length;      // Length of the region
    unsigned int type;        // Type of memory (usable, reserved, etc.)
} __attribute__((packed)) memory_map_entry_t;

// Function to process and print the memory map
void process_memory_map(multiboot_info_t *mbd);

#endif
