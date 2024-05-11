#define VIDMEM_START 0xb8000
#define VIDMEM_WIDTH 80
#define VIDMEM_HEIGHT 25
#define VIDMEM_SIZE (VIDMEM_WIDTH * VIDMEM_HEIGHT)
#define VIDMEM_END (VIDMEM_START + VIDMEM_SIZE)

uint16_t *vidmem = (uint16_t*) VIDMEM_START;
uint8_t color = 0x0f;

static inline void enable_cursor(uint8_t start, uint8_t end);
static inline void disable_cursor(void);

static inline void putc(char str, uint8_t x, uint8_t y);
static inline void putc_raw(char str, uint16_t *addr);
