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

static uint8_t vga_addCursorX(uint8_t x) {
  x = vga_getCursorX() + x;
  vga_moveCursorX(x);
  return x;
}

static uint8_t vga_addCursorY(uint8_t y) {
  y = vga_getCursorY() + y;
  vga_moveCursorY(y);
  return y;
}

// these functions are protected versions of the above
static void vga_cursorLeft() {
  if (vga_getCursorX() <= 0) {
    if (vga_getCursorY() <= 0) {
      vga_moveCursorTo(0, 0);
    } else {
      vga_moveCursorTo(VGA_WIDTH - 1, vga_getCursorY() - 1);
    }
  } else {
    vga_addCursorX(-1);
  }
}

static void vga_cursorUp() {
  if (vga_getCursorY() <= 0) {
    vga_moveCursorTo(0, 0);
  } else {
    vga_addCursorY(-1);
  }
}


static void vga_cursorRight() {
  vga_addCursorX(1);
  overflow();
}

static void vga_cursorDown() {
  vga_addCursorY(1);
  overflow();
}

// formatted / pretty versions

static char *vga_cursorLeftf() {
  // move cursor left until non-null character, or end of vidmem
  vga_cursorLeft();
  char *vidmem = vga_cursorToVidmem();
  while (vga_getCursorX() > 0 && *(vidmem - 2) == 0x00) {
    vga_cursorLeft();
    vidmem -= 2;
  }
  return vidmem;
}

static char *vga_cursorUpf() {
  vga_cursorUp();
  char *vidmem = vga_cursorToVidmem();
  while (vga_getCursorX() > 0 && *(vidmem - 2) == 0x00) {
    vga_cursorLeft();
    vidmem -= 2;
  }
  return vidmem;
}

static char *vga_cursorRightf() {
  char *vidmem = vga_cursorToVidmem();
  if (*vidmem != 0x00) {
    vga_cursorRight();
  } else {
    vga_cursorDownf();
    vga_moveCursorX(0);
  }
  return vga_cursorToVidmem();
}

static char *vga_cursorDownf() {
  vga_cursorDown();
  char *vidmem = vga_cursorToVidmem();
  while (vga_getCursorX() > 0 && *(vidmem - 2) == 0x00) {
    vga_cursorLeft();
    vidmem -= 2;
  }
  return vidmem;
}


