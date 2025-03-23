#include <stdint.h>
#include <stddef.h>
#include <lib/mem.h>
#include <multiboot.h>
#include <drivers/vga.h>
#include <lib/panic.h>

#define PD_IDX(virt) ((virt) >> 22)
#define PT_IDX(virt) (((virt) >> 12) & 0x3ff)

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

  // vidmem passthrough
  dir[PD_IDX(VIDMEM_START)] = (dir_entry) {
    .p = 1,
    .rw = 1,
    .us = 0,
    .pwt = 0,
    .pcd = 1,
    .ps = 0,
    .addr = (uintptr_t) table >> 12
  };
  table[PT_IDX(VIDMEM_START)] = (table_entry) {
    .p = 1,
    .rw = 1,
    .us = 0,
    .pwt = 0,
    .pcd = 1,
    .pat = 0,
    .g = 1,
    .addr = VIDMEM_START >> 12
  };

  // kernel passthrough
  for (uint16_t i = 1; i <= kernel_pages; i++) {
    uintptr_t addr = (uintptr_t) &_kernel_start + (i * PAGE_SIZE);

    dir[PD_IDX(addr)] = (dir_entry) {
      .p = 1,
      .rw = 1,
      .us = 0,
      .pwt = 0,
      .pcd = 0,
      .ps = 0,
      .addr = (uintptr_t) table >> 12
    };
    table[PT_IDX(addr)] = (table_entry) {
      .p = 1,
      .rw = 1,
      .us = 0,
      .pwt = 0,
      .pcd = 0,
      .pat = 0,
      .g = 1,
      .addr = addr >> 12
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
}

void vmalloc_init() {
  // test function
  create_passthrough();
}
