#ifndef LIB_KTERM_H
#define LIB_KTERM_H


#include <stdbool.h>
#include <stdint.h>
#include <drivers/vga.h>

extern char kterm_buf[VIDMEM_SIZE];
extern uint16_t kterm_buf_start;
uint16_t kterm_buf_end();
void kterm_buf_reset();

bool kterm_special_char();

void kterm_init();
void kterm_main();

void kterm_run_cmd();


#endif
