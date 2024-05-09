#define VGA_VIDMEM_START 0xb8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MAX_VIDMEM_LEN VGA_WIDTH * VGA_HEIGHT * 2
#define VGA_VIDMEM_END VGA_VIDMEM_START + VGA_MAX_VIDMEM_LEN

enum vga_color {
	vga_color_black = 0,
	vga_color_blue = 1,
  vga_color_green = 2,
	vga_color_cyan = 3,
	vga_color_red = 4,
	vga_color_magenta = 5,
	vga_color_brown = 6,
	vga_color_light_grey = 7,
	vga_color_dark_grey = 8,
	vga_color_light_blue = 9,
	vga_color_light_green = 10,
	vga_color_light_cyan = 11,
	vga_color_light_red = 12,
	vga_color_light_magenta = 13,
	vga_color_light_brown = 14,
	vga_color_white = 15,
};

uint8_t color = vga_color_white | vga_color_black << 4;


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


uint8_t vga_cursorXMax = 0;
static void vga_cursorLeft();
static void vga_cursorUp();
static void vga_cursorRight();
static void vga_cursorDown();

static char *vga_cursorLeftf();
static char *vga_cursorUpf();
static char *vga_cursorRightf();
static char *vga_cursorDownf();


static char *vga_xyToVidmem(uint8_t x, uint8_t y);
static char *vga_cursorToVidmem();

static void changeColor(enum vga_color fg, enum vga_color bg);

static void newl();
static void inewl();

static void scroll();
static void overflow();

static void print(char *str);
static void printc(char str);
static void insertc(char str);
static void printf(char *str);

static void vga_clearRow(uint8_t row);
static void vga_clearScreen();

static void printh(uint64_t hex);
static void printhf(uint64_t hex);

static void vgaInit();

static char *vga_bufEndAfter(char *ptr);
static char *vga_bufEndBefore(char *ptr);

static bool vga_specialChar(char str);
