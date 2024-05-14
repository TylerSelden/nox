#include <stdint.h>
#include <drivers/pic.h>
#include <drivers/io.h>

void pic_end_int(uint8_t irq) {
  if (irq < 8) return;
  outb(PIC2_CMD, 0x20);
  outb(PIC1_CMD, 0x20);
}

void pic_init() {
  // set up cascading mode
  outb(PIC1_CMD, 0x11);
  io_wait();
  outb(PIC2_CMD, 0x11);
  io_wait();

  // vector offsets
  outb(PIC1_DATA, 0x20);
  io_wait();
  outb(PIC2_DATA, 0x28);
  io_wait();

  // tell master it has a slave
  outb(PIC1_DATA, 4);
  io_wait();
  outb(PIC1_DATA, 2);
  io_wait();

  // enable 8086 mode
  outb(PIC1_DATA, 0x01);
  io_wait();
  outb(PIC2_DATA, 0x01);
  io_wait();

  // reset masks
  outb(PIC1_DATA, 0);
  outb(PIC2_DATA, 0);
}

void pic_disable() {
  outb(PIC1_DATA, 0xff);
  outb(PIC2_DATA, 0xff);
}
