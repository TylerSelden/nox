#include <stdbool.h>
#include <stdint.h>
#include <lib/idt.h>
#include <drivers/vga.h>

__attribute__((aligned(0x10))) idt_entry_t idt[IDT_SIZE];
idtr_t idtr;

void idt_set_entry(uint8_t index, uint32_t handler_addr) {
  idt[index].offset_low  = handler_addr & 0xffff;
  idt[index].selector    = 0x08; // kernel code segment
  idt[index].reserved    = 0;
  idt[index].attributes  = 0x8e; // interrupt gate
  idt[index].offset_high = (handler_addr >> 16) & 0xffff;
}

extern int isr_stub_table[]; // int to stop gcc errors, is default anyway

void idt_init() {
  idtr.base = (uint32_t) (uint8_t*) &idt[0];
  idtr.limit = (uint16_t) 64 * IDT_SIZE - 1;

  for (uint8_t i = 0; i < 32; i++) {
    idt_set_entry(i, isr_stub_table[i]);
  }

  __asm__ volatile ("lidt %0" : : "m"(idtr)); // load idt
  __asm__ volatile ("sti"); // enable interrupts
}


void exception_handler(uint8_t test) {
  color = 0x4f;
  vga_set_cursor(0, 0);
  vga_prints("Exception occurred, halting! Exception number: "); 
  vga_printi(test, 10);

  __asm__ volatile ("hlt");
}

void irq_test(uint8_t irq) {
  color = 0x5f;
  vga_set_cursor(0, 1);
  vga_prints("IRQ: ");
  vga_printi(irq, 10);

  pic_end_int(irq);
}
