#include <stdbool.h>
#include <stdint.h>
#include <drivers/vga.h>
#include <drivers/io.h>

uint16_t *vidmem = (uint16_t*) VIDMEM_START;
uint8_t color = 0x0f;


void vga_enable_cursor(uint8_t start, uint8_t end) {
  outb(0x3d4, 0x0a);
  outb(0x3d5, (inb(0x3d5) & 0xc0) | start);
  outb(0x3d4, 0x0b);
  outb(0x3d5, (inb(0x3d5) & 0xe0) | end);
}

void vga_disable_cursor(void) {
  outb(0x3d4, 0x0a);
  outb(0x3d5, 0x20);
}

void vga_set_cursor(int8_t x, int8_t y) {
  uint16_t pos = y * VIDMEM_WIDTH + x;

  outb(0x3d4, 0x0f);
  outb(0x3d5, (uint8_t) (pos & 0xff));
  outb(0x3d4, 0x0e);
  outb(0x3d5, (uint8_t) (pos >> 8 & 0xff));
}
void vga_set_cursor_x(uint8_t x) {
  vga_set_cursor(x, vga_get_cursor_y());
}
void vga_set_cursor_y(uint8_t y) {
  vga_set_cursor(vga_get_cursor_x(), y);
}

uint16_t vga_get_cursor() {
  uint16_t pos = 0;
  outb(0x3d4, 0x0f);
  pos |= inb(0x3d5);
  outb(0x3d4, 0x0e);
  pos |= ((uint16_t)inb(0x3d5)) << 8;
  return pos;
}
uint16_t vga_get_cursor_addr() {
  uint16_t addr = 0;
  addr += vga_get_cursor_x();
  addr += vga_get_cursor_y() * VIDMEM_WIDTH;
  return addr;
}
uint8_t vga_get_cursor_x() {
  return vga_get_cursor() % VIDMEM_WIDTH;
}
uint8_t vga_get_cursor_y() {
  return vga_get_cursor() / VIDMEM_WIDTH;
}
void vga_add_cursor_x(int8_t x) {
  vga_set_cursor(vga_get_cursor_x() + x, vga_get_cursor_y());
}
void vga_add_cursor_y(int8_t y) {
  vga_set_cursor(vga_get_cursor_x(), vga_get_cursor_y() + y);
}

// helper funcs

void vga_write_row(uint8_t row, char str) {
  for (uint8_t x = 0; x < VIDMEM_WIDTH; x++) {
    vga_putc(str, x, row);
  }
}

void vga_clear() {
  for (uint8_t row = 0; row < VIDMEM_HEIGHT; row++) {
    vga_write_row(row, 0);
  }
}

void vga_scroll() {
  for (uint8_t y = 1; y < VIDMEM_HEIGHT; y++) {
    for (uint8_t x = 0; x < VIDMEM_WIDTH; x++) {
      vidmem[((y - 1) * VIDMEM_WIDTH) + x] = vidmem[(y * VIDMEM_WIDTH) + x];
    }
  }
  // make last row empty
  vga_write_row(VIDMEM_HEIGHT - 1, ' ');
  vga_set_cursor_y(VIDMEM_HEIGHT - 1);
}

void vga_newl() {
  vga_set_cursor_x(0);
  vga_add_cursor_y(1);
  if (vga_get_cursor_y() >= VIDMEM_HEIGHT) vga_scroll();
}

static void vga_handle_overflow() {
  if (vga_get_cursor_y() >= VIDMEM_HEIGHT) vga_scroll();
}


static bool vga_special_char(char str, uint16_t addr, bool print) {
  if (str == '\n') {
    if (print) {
      vga_newl();
    }
  } else if (str == '\t') {
    vidmem[addr] = ' ' | (color << 8);
    vidmem[addr + 1] = ' ' | (color << 8);
    if (print) {
      vga_add_cursor_x(2);
    }
  } else {
    return false;
  }
  return true;
}



void vga_putc(char str, uint8_t x, uint8_t y) {
  uint16_t addr = (y * VIDMEM_WIDTH) + x;
  if (vga_special_char(str, addr, false)) return;
  vidmem[addr] = str | (color << 8);
}
void vga_putcr(char str, uint16_t addr) {
  if (vga_special_char(str, addr, false)) return;
  vidmem[addr] = str | (color << 8);
}
void vga_printc(char str) {
  if (vga_special_char(str, vga_get_cursor_addr(), true)) return;
  vga_putcr(str, vga_get_cursor_addr());
  vga_add_cursor_x(1);
  vga_handle_overflow();
}

void vga_puts(char *str, uint8_t x, uint8_t y) {
  while (*str != 0) {
    vga_putc(*str, x, y);
    str++;
    x++; // this is fine if it exceeds VIDMEM_WIDTH, as the math still works in putc()
  }
}
void vga_putsr(char *str, uint16_t addr) {
  while (*str != 0) {
    vga_putcr(*str, addr);
    str++;
    addr++;
  }
}
void vga_prints(char *str) {
  while (*str != 0) {
    vga_printc(*str);
    str++;
  }
}


void vga_puti(uint32_t num, uint8_t base, uint8_t x, uint8_t y) {
  if (base > 16) return;
  char *map = "0123456789abcdef";
  char buffer[33];
  int i = 32;
  buffer[i] = 0;

  if (num == 0) {
    i--;
    buffer[i] = map[0];
  } else {
    while (num && i) {
      buffer[--i] = map[num % base];
      num /= base;
    }
  }
  vga_puts(&buffer[i], x, y);
}
void vga_putir(uint32_t num, uint8_t base, uint16_t addr) {
  if (base > 16) return;
  char *map = "0123456789abcdef";
  char buffer[33];
  int i = 32;
  buffer[i] = 0;

  if (num == 0) {
    i--;
    buffer[i] = map[0];
  } else {
    while (num && i) {
      buffer[--i] = map[num % base];
      num /= base;
    }
  }
  vga_putsr(&buffer[i], addr);
}
void vga_printi(uint32_t num, uint8_t base) {
  if (base > 16) return;
  char *map = "0123456789abcdef";
  char buffer[33];
  int i = 32;
  buffer[i] = 0;
  
  if (num == 0) {
    vga_printc(map[0]);
    return;
  }

  while (num && i) {
    buffer[--i] = map[num % base];
    num /= base;
  }

  vga_prints(&buffer[i]);
}

void vga_init() {
  vga_enable_cursor(14, 15);
  vga_clear();
  vga_set_cursor(0, 0);
}
