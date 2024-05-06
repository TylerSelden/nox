#define VGA_VIDMEM_BASE_ADDR 0xb8000
#define VGA_MAX_ROWS 80
#define VGA_MAX_COLS 25
#define VGA_MAX_VIDMEM_LEN VGA_MAX_ROWS * VGA_MAX_COLS * 2

int vga_cursorX = 0;
int vga_cursorY = 0;


// cursor stuff
static void vga_enableCursor(uint8_t cursor_start, uint8_t cursor_end) {
  outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);
 
	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

static void vga_disableCursor() {
  outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

static void vga_moveCursorTo(uint8_t x, uint8_t y) {
  uint16_t pos = y * VGA_MAX_ROWS + x;
 
	outb(0x3D4, 0x0F);
	outb(0x3D5, (uint8_t) (pos & 0xFF));
	outb(0x3D4, 0x0E);
	outb(0x3D5, (uint8_t) ((pos >> 8) & 0xFF));
}

static uint16_t vga_getCursorPos() {
  uint16_t pos = 0;
  outb(0x3D4, 0x0F);
  pos |= inb(0x3D5);
  outb(0x3D4, 0x0E);
  pos |= ((uint16_t)inb(0x3D5)) << 8;
  return pos;
}

static uint8_t vga_getCursorX() {
  return vga_getCursorPos() % VGA_MAX_ROWS;
}

static uint8_t vga_getCursorY() {
  return vga_getCursorPos() / VGA_MAX_ROWS;
}





static char *vga_xyToVidmem(int x, int y) {
  char *vidmem = (char*) VGA_VIDMEM_BASE_ADDR;
  vidmem += x * 2;
  vidmem += y * 2 * VGA_MAX_ROWS;
  return vidmem;
}

static void vga_newl() {
  vga_cursorX = 0;
  vga_cursorY++;
  if (vga_cursorY >= VGA_MAX_COLS) vga_cursorY = 0;
}

static void vga_print(char *str) {
  char *vidmem = vga_xyToVidmem(vga_cursorX, vga_cursorY);
  while (*str) {
    vga_cursorX++;
    if (vga_cursorX >= VGA_MAX_ROWS) vga_newl();
    *vidmem++ = *str++;
    *vidmem++ = 0x0f;
  }
}

static void vga_printChar(char str) {
  char *vidmem = vga_xyToVidmem(vga_cursorX, vga_cursorY);
  vga_cursorX++;
  if (vga_cursorX >= VGA_MAX_ROWS) vga_newl();
  *vidmem++ = str;
  *vidmem   = 0x0f;
}

static void vga_printf(char *str) {
  vga_print(str);
  vga_newl();
}

static void vga_clearScreen() {
  vga_cursorX = 0;
  vga_cursorY = 0;
  for (char *i = (char*) VGA_VIDMEM_BASE_ADDR; i < (char*) VGA_VIDMEM_BASE_ADDR + VGA_MAX_VIDMEM_LEN; i += 2) {
    *i = 0x0000;
  }
}

static void vga_printHex(uint8_t hex) {
  char hexMap[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
  vga_printChar(hexMap[(hex & 0xF0) >> 4]);
  vga_printChar(hexMap[(hex & 0x0F) >> 0]);
}

static void vga_printHexf(uint8_t hex) {
  vga_print("0x");
  vga_printHex(hex);
  vga_newl();
}

static void vga_init() {
  // enable cursor, set to 0, 0
  vga_enableCursor(14, 15);
  vga_moveCursorTo(0x10, 0x02);

  vga_clearScreen();
}
