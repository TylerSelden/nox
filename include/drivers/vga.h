#ifndef DRIVERS_VGA_H
#define DRIVERS_VGA_H


#define VIDMEM_START 0xb8000
#define VIDMEM_WIDTH 80
#define VIDMEM_HEIGHT 25
#define VIDMEM_SIZE (VIDMEM_WIDTH * VIDMEM_HEIGHT)
#define VIDMEM_END (VIDMEM_START + VIDMEM_SIZE)

extern uint16_t *vidmem;
extern uint8_t color;

void enable_cursor(uint8_t start, uint8_t end);
void disable_cursor(void);

void putc(char str, uint8_t x, uint8_t y);
void putc_raw(char str, uint16_t *addr);


#endif
