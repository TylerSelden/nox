static inline void enable_cursor(uint8_t start, uint8_t end) {
  outb(0x3d4, 0x0a);
  outb(0x3d5, (inb(0x3d5) & 0xc0) | start);
  outb(0x3d4, 0x0b);
  outb(0x3d5, (inb(0x3d5) & 0xe0) | end);
}

static inline void disable_cursor(void) {
  outb(0x3d4, 0x0a);
  outb(0x3d5, 0x20);
}

// get cursor and move cursor

static inline void set_cursor(uint8_t x, uint8_t y) {
  uint16_t pos = y * VIDMEM_WIDTH + x;

  outb(0x3d4, 0x0f);
  outb(0x3d5, (uint8_t) (pos & 0xff));
  outb(0x3d4, 0x0e);
  outb(0x3d5, (uint8_t) (pos >> 8 & 0xff));
}

static inline uint16_t get_cursor() {
  uint16_t pos = 0;
  outb(0x3d4, 0x0f);
  pos |= inb(0x3d5);
  outb(0x3d4, 0x0e);
  pos |= ((uint16_t)inb(0x3d5)) << 8;
  return pos;
}

static uint8_t get_cursor_x() {
  return get_cursor() % VIDMEM_WIDTH;
}

static uint8_t get_cursor_y() {
  return get_cursor() / VIDMEM_WIDTH;
}

static inline void putc(char str, uint8_t x, uint8_t y) {
  vidmem[(y * VIDMEM_WIDTH) + x] = str | (color << 8);
}
static inline void putrc(char str, uint16_t *addr) {
  *addr = str | (color << 8);
}

// strings, ints, hex, formatted
