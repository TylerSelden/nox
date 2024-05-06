static void vga_enableCursor(uint8_t cursor_start, uint8_t cursor_end) {
  outb(0x3d4, 0x0a);
	outb(0x3d5, (inb(0x3d5) & 0xc0) | cursor_start);
 
	outb(0x3d4, 0x0b);
	outb(0x3d5, (inb(0x3d5) & 0xc0) | cursor_end);
}

static void vga_disableCursor() {
  outb(0x3d4, 0x0a);
	outb(0x3d5, 0x20);
}



static uint16_t vga_getCursorPos() {
  uint16_t pos = 0;
  outb(0x3d4, 0x0f);
  pos |= inb(0x3d5);
  outb(0x3d4, 0x0e);
  pos |= ((uint16_t)inb(0x3d5)) << 8;
  return pos;
}

static uint8_t vga_getCursorX() {
  return vga_getCursorPos() % VGA_WIDTH;
}

static uint8_t vga_getCursorY() {
  return vga_getCursorPos() / VGA_WIDTH;
}





static void vga_moveCursorTo(uint8_t x, uint8_t y) {
  uint16_t pos = y * VGA_WIDTH + x;
 
	outb(0x3d4, 0x0f);
	outb(0x3d5, (uint8_t) (pos & 0xff));
	outb(0x3d4, 0x0e);
	outb(0x3d5, (uint8_t) ((pos >> 8) & 0xff));
}

static void vga_moveCursorX(uint8_t x) {
  vga_moveCursorTo(x, vga_getCursorY());
}

static void vga_moveCursorY(uint8_t y) {
  vga_moveCursorTo(vga_getCursorX(), y);
}

static void vga_addCursorX(uint8_t x) {
  vga_moveCursorX(vga_getCursorX() + x);
}

static void vga_addCursorY(uint8_t y) {
  vga_moveCursorY(vga_getCursorY() + y);
}

