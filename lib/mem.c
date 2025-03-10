#include <stdint.h>
#include <lib/mem.h>
#include <drivers/vga.h>
#include <multiboot.h>

void process_memory_map(multiboot_info_t *mbd) {
    // Access the memory map address and length directly from mbd
    unsigned int mmap_addr = mbd->mmap_addr;
    unsigned int mmap_length = mbd->mmap_length;

    unsigned int i = 0;
    memory_map_entry_t *entry;

    if (mmap_addr == 0 || mmap_length == 0) {
        vga_prints("Invalid memory map address or length.\n");
        return;
    }

    // Iterate through each memory map entry
    while (i < mmap_length) {
        entry = (memory_map_entry_t *)(mmap_addr + i);

        // Check if the memory type is usable (type 1 means usable)
        if (entry->type == 1) {
            // Print usable memory regions (type 1 means usable)
            vga_prints("Usable Memory: ");
            vga_printi(entry->base_addr, 16); // Print base address in hex
            vga_prints(" - ");
            vga_printi(entry->base_addr + entry->length, 16); // Print end address in hex
            vga_prints(" Size: ");
            vga_printi(entry->length, 16); // Print size in hex
            vga_prints("\n");
        }

        // Move to the next entry in the memory map
        i += entry->size + sizeof(entry->size);
    }
}

