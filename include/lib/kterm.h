#ifndef LIB_KTERM_H
#define LIB_KTERM_H


#include <stdbool.h>
#include <stdint.h>

extern uint16_t kterm_buf_start;
uint16_t kterm_buf_end();

void kterm_start();
void kterm_main();

bool kterm_special_key();

#endif
