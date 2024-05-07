#define VGA_VIDMEM_START 0xb8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MAX_VIDMEM_LEN VGA_WIDTH * VGA_HEIGHT * 2

static void vga_enableCursor(uint8_t cursor_start, uint8_t cursor_end);
static void vga_disableCursor();

static uint16_t vga_getCursorPos();
static uint8_t vga_getCursorX();
static uint8_t vga_getCursorY();

static void vga_moveCursorTo(uint8_t x, uint8_t y);
static void vga_moveCursorX(uint8_t x);
static void vga_moveCursorY(uint8_t y);
static uint8_t vga_addCursorX(uint8_t x);
static uint8_t vga_addCursorY(uint8_t y);


static char *vga_xyToVidmem(uint8_t x, uint8_t y);
static char *vga_cursorToVidmem();

static void newl();
static void scroll();
static void overflow();

static void print(char *str);
static void printc(char str);
static void printf(char *str);

static void vga_clearRow(uint8_t row);
static void vga_clearScreen();

static void printh(uint8_t hex);
static void printhf(uint8_t hex);

static void vga_init();

static bool vga_specialChar(char str);


#include "./cursor.c"
#include "./vga.c"
