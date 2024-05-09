#include "./cursor.c"

static char *vga_xyToVidmem(uint8_t x, uint8_t y) {
  char *vidmem = (char*) VGA_VIDMEM_START;
  vidmem += x * 2;
  vidmem += y * 2 * VGA_WIDTH;
  return vidmem;
}

static char *vga_cursorToVidmem() {
  char *vidmem = (char*) VGA_VIDMEM_START;
  vidmem += vga_getCursorX() * 2;
  vidmem += vga_getCursorY() * 2 * VGA_WIDTH;
  return vidmem;
}


static void changeColor(enum vga_color fg, enum vga_color bg) {
  color = fg | bg << 4;
}

static void newl() {
  vga_moveCursorX(0);
  if (vga_addCursorY(1) >= VGA_HEIGHT) scroll();
}

static void scroll() {
  // clear out first row
  vga_clearRow(0);
  // for each row (1 through WIDTH-1), shift it up 1
  for (uint8_t y = 1; y < VGA_HEIGHT; y++) {
    for (uint8_t x = 0; x < VGA_WIDTH; x++) {
      *vga_xyToVidmem(x, y - 1) = *vga_xyToVidmem(x, y);
    }
  }
  // clear last row
  vga_clearRow(VGA_HEIGHT - 1);
  vga_addCursorY(-1);
}

static void overflow() {
  if (vga_getCursorX() >= VGA_WIDTH) newl();
  if (vga_getCursorY() >= VGA_HEIGHT) scroll();
}

static void printc(char str) {
  if (vga_specialChar(str)) return;
  char *vidmem = vga_cursorToVidmem();
  if (vga_addCursorX(1) >= VGA_WIDTH) overflow();
  *vidmem++ = str;
  *vidmem   = color;
}

static void insertc(char str) {
  if (vga_specialChar(str)) return;
  char *vidmem = vga_cursorToVidmem();
  if (vga_addCursorX(1) >= VGA_WIDTH) overflow();
  // shift everything to the RIGHT
  // get last position and work backwards :/
  char *cpy = vga_cursorToVidmem();
  while (*cpy != 0) cpy += 2;
  while (cpy > vidmem) {
    *cpy = *(cpy - 2);
    *(cpy + 1) = *(cpy - 1);
    cpy -= 2;
  }
  vidmem = vga_cursorToVidmem() - 2;
  *vidmem++ = str;
  *vidmem   = color;
}


static void print(char *str) {
  while (*str) {
    printc(*str);
    str++;
  }
}

static void printf(char *str) {
  print(str);
  newl();
}

static void vga_clearRow(uint8_t row) {
  char *row_start = (char*) VGA_VIDMEM_START + (VGA_WIDTH * row * 2);
  char *row_end = (char*) row_start + (VGA_WIDTH * 2) - 1;
  for (char *i = row_start; i < row_end; i += 2) {
    *i = 0x00;
    *(i + 1) = 0x0f;
  }
}

static void vga_clearScreen() {
  for (uint8_t row = 0; row < VGA_HEIGHT; row++) {
    vga_clearRow(row);
  }
  return;
  vga_moveCursorTo(0, 0);
}

static void printh(uint64_t hex) {
  char hexMap[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
//  printc(hexMap[(hex & 0xF0) >> 4]);
//  printc(hexMap[(hex & 0x0F) >> 0]);
  bool started = false;
  uint8_t shift;

  for (shift = 60; shift > 0; shift -= 4) {
    char digit = hexMap[(hex >> shift) & 0xf];
    if (!started && digit == '0' && shift > 4) {
      continue;
    }
    started = true;
    printc(digit);
  }
  // finally, last character
  printc(hexMap[hex & 0xf]);
}

static void printhf(uint64_t hex) {
  print("0x");
  printh(hex);
  newl();
}

static void vgaInit() {
  // enable cursor, set to 0, 0
  vga_enableCursor(14, 15);
  vga_moveCursorTo(0, 0);

  vga_clearScreen();
}


static bool vga_specialChar(char str) {
  if (str == '\n') {
    newl();
  } else if (str == '\t') {
    printc(' ');
    printc(' ');
  } else if (str == '\r') {
    vga_moveCursorX(0);
  } else if (str == '\b') {
    // backspace
    vga_cursorLeftf();
    char *vidmem = vga_cursorToVidmem();
    
    // shift everything over
    while (*vidmem != 0) {
      *vidmem = *(vidmem + 2);
      *(vidmem + 1) = *(vidmem + 3);
      vidmem += 2;
    }
  } else {
    return false;
  }
  return true;
}
