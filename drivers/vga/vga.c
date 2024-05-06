#define VGA_VIDMEM_START 0xb8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MAX_VIDMEM_LEN VGA_WIDTH * VGA_HEIGHT * 2


#include "./vga.h"
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


static void vga_newl() {
  vga_moveCursorX(0);
  vga_addCursorY(1);
  if (vga_getCursorY() >= VGA_HEIGHT) vga_moveCursorY(0);
}

static void vga_printChar(char str) {
  if (vga_specialChar(str)) return;
  char *vidmem = vga_cursorToVidmem();
  vga_addCursorX(1);
  if (vga_getCursorX() >= VGA_WIDTH) vga_newl();
  *vidmem++ = str;
  *vidmem   = 0x0f;
}



static void vga_print(char *str) {
  char *vidmem = (char*) vga_cursorToVidmem();
  while (*str) {
    vga_printChar(*str);
    *str++;
  }
}

static void vga_printf(char *str) {
  vga_print(str);
  vga_newl();
}

static void vga_clearScreen() {
  for (char *i = (char*) VGA_VIDMEM_START; i < (char*) VGA_VIDMEM_START + VGA_MAX_VIDMEM_LEN; i += 2) {
    *i = 0x0000;
  }
  for (uint8_t i = 0; i < (VGA_MAX_VIDMEM_LEN - VGA_VIDMEM_START) / 2; i++) {
    vga_printChar(' ');
  }
  vga_moveCursorTo(0, 0);
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
  vga_moveCursorTo(0, 0);

  vga_clearScreen();
}


static bool vga_specialChar(char str) {
  if (str == '\n') {
    vga_newl();
  } else if (str == '\t') {
    vga_printChar(' ');
    vga_printChar(' ');
  } else if (str == '\r') {
    vga_moveCursorX(0);
  } else if (str == '\b') {
    // backspace
    // move cursor backwards
    if (vga_getCursorX() == 0) {
      if (vga_getCursorY() == 0) {
        vga_moveCursorTo(0, 0);
      } else {
        vga_moveCursorTo(VGA_WIDTH - 1, vga_getCursorY() - 1);
      }
    } else {
      vga_addCursorX(-1);
    }

    char *vidmem = vga_cursorToVidmem();
    *vidmem++ = ' ';
    *vidmem   = 0x0f;

  } else {
    return false;
  }
  return true;
}
