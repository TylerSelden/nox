#include <stdbool.h>
#include <stdint.h>
#include <lib/panic.h>
#include <lib/idt.h>
#include <drivers/pic.h>
#include <drivers/vga.h>
#include <drivers/keyboard.h>

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

  for (uint8_t i = 0; i < 48; i++) {
    idt_set_entry(i, isr_stub_table[i]);
  }

  __asm__ volatile ("lidt %0" : : "m"(idtr)); // load idt
  __asm__ volatile ("sti"); // enable interrupts
}


void exception_handler(idt_registers_t *state) {
  panic_exc(state->int_no, state->err_code);
}



void irq_handler(idt_registers_t *state) {
  if (state->int_no == 33) keyboard_input();

  pic_end_int(state->int_no);
}
