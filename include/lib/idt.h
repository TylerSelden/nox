#ifndef LIB_IDT_H
#define LIB_IDT_H


#include <stdint.h>

#define IDT_SIZE 256

typedef struct {
  uint16_t offset_low;  // lower 16 bits of handler func addr
  uint16_t selector;    // kernel segment selector
  uint8_t  reserved;    // this musts be zero
  uint8_t  attributes;  // type and attributes
  uint16_t offset_high; // higher 16 bits of handler func addr
} __attribute__((packed)) idt_entry_t;

typedef struct {
  uint16_t limit;
  uint32_t base;
} __attribute__((packed)) idtr_t;

extern idt_entry_t idt[IDT_SIZE];
extern idtr_t idtr;

void set_idt_entry(uint8_t index, uint32_t handler_addr);
void idt_init();

void exception_handler(uint8_t test);


void irq_handler(uint8_t irq);


#endif
