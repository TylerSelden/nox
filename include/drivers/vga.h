#ifndef DRIVERS_VGA_H
#define DRIVERS_VGA_H


#define VIDMEM_START 0xb8000
#define VIDMEM_WIDTH 80
#define VIDMEM_HEIGHT 25
#define VIDMEM_SIZE (VIDMEM_WIDTH * VIDMEM_HEIGHT)
#define VIDMEM_END (VIDMEM_START + VIDMEM_SIZE)

extern uint16_t *vidmem;
extern uint8_t color;

void vga_enable_cursor(uint8_t start, uint8_t end);
void vga_disable_cursor(void);

void vga_set_cursor(uint8_t x, uint8_t y);
void vga_set_cursor_x(uint8_t x);
void vga_set_cursor_y(uint8_t y);

uint16_t vga_get_cursor();
uint16_t vga_get_cursor_addr();
uint8_t vga_get_cursor_x();
uint8_t vga_get_cursor_y();

void vga_add_cursor_x(int8_t x);
void vga_add_cursor_y(int8_t y);

void vga_write_row(uint8_t row, char str);
void vga_clear();
void vga_scroll();
void vga_newl();

void vga_putc(char str, uint8_t x, uint8_t y);
void vga_putcr(char str, uint16_t addr);
void vga_printc(char str);

void vga_puts(char *str, uint8_t x, uint8_t y);
void vga_putsr(char *str, uint16_t addr);
void vga_prints(char *str);

void vga_puti(uint32_t num, uint8_t base, uint8_t x, uint8_t y);
void vga_putir(uint32_t num, uint8_t base, uint16_t addr);
void vga_printi(uint32_t num, uint8_t base);

void vga_init();


#endif
