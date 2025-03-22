#include <stdint.h>
#include <stddef.h>
#include <lib/mem.h>
#include <multiboot.h>
#include <drivers/vga.h>
#include <lib/panic.h>

typedef struct {
  unsigned int p    : 1;  //     0: Present
  unsigned int rw   : 1;  //     1: Read/Write
  unsigned int us   : 1;  //     2: User/Supervisor
  unsigned int pwt  : 1;  //     3: Write-Through
  unsigned int pcd  : 1;  //     4: Cache Disable
  unsigned int a    : 1;  //     5: Accessed
  unsigned int avl0 : 1;  //     6: AVAILABLE
  unsigned int ps   : 1;  //     7: Page Size (0)
  unsigned int avl1 : 4;  //  8-11: AVAILABLE
  unsigned int addr : 20; // 12-31: Address
} __attribute__ ((packed)) dir_entry;

typedef struct {
  unsigned int p    : 1;  //     0: Present
  unsigned int rw   : 1;  //     1: Read/Write
  unsigned int us   : 1;  //     2: User/Supervisor
  unsigned int pwt  : 1;  //     3: Write-Through
  unsigned int pcd  : 1;  //     4: Cache Disable
  unsigned int a    : 1;  //     5: Accessed
  unsigned int d    : 1;  //     6: Dirty
  unsigned int pat  : 1;  //     7: Page Attribute Table
  unsigned int g    : 1;  //     8: Global
  unsigned int avl0 : 3;  //  9-11: AVAILABLE
  unsigned int addr : 20; // 12-31: Address
} __attribute__ ((packed)) table_entry;

dir_entry *dir;
table_entry *table;

void create_passthrough() {
  dir = kmalloc();
  table = kmalloc();
  if (!dir || !table) panic("Not enough memory to inizialize paging.");

  size_t kernel_size = &_kernel_end - &_kernel_start;
  uint16_t kernel_pages = (kernel_size + PAGE_SIZE - 1) / PAGE_SIZE;

  for (uint16_t i = 0; i < 1024; i++) {
    dir[i].p = 0;
    table[i].p = 0;
  }

  // vidmem passthrough
  dir[0] = (dir_entry) {
    .p = 1,
    .rw = 1,
    .us = 0,
    .pwt = 0,
    .pcd = 1,
    .ps = 0,
    .addr = ((uintptr_t) table >> 12) & 0xfffff
  };
  table[0] = (table_entry) {
    .p = 1,
    .rw = 1,
    .us = 0,
    .pwt = 0,
    .pcd = 1,
    .pat = 0,
    .g = 1,
    .addr = 0xb8000
  };

  // kernel passthrough
  for (uint16_t i = 1; i <= kernel_pages; i++) {
    dir[i] = (dir_entry) {
      .p = 1,
      .rw = 1,
      .us = 0,
      .pwt = 0,
      .pcd = 0,
      .ps = 0,
      .addr = ((uintptr_t) table >> 12) & 0xfffff
    };
    table[i] = (table_entry) {
      .p = 1,
      .rw = 1,
      .us = 0,
      .pwt = 0,
      .pcd = 0,
      .pat = 0,
      .g = 1,
      .addr = ((_kernel_start + (i * PAGE_SIZE)) >> 12) & 0xfffff
    };
  }

  // enable paging
  asm volatile (
    "mov %0, %%cr3\n\t"         // load page directory address into CR3
    "mov %%cr0, %%eax\n\t"      // copy CR0 into EAX
    "or $0x80000000, %%eax\n\t" // set the PG bit (bit 31) in EAX
    "mov %%eax, %%cr0\n\t"      // write modified value back to CR0
    :
    : "r" (dir)                 // pass in dir pointer
    : "eax"                     // clobbers EAX register
  );

  printf("Test value: %d", dir[0].addr);
}

void vmalloc_init() {
  // test function
  create_passthrough();
}
