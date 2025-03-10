#ifndef LIB_PANIC_H
#define LIB_PANIC_H


#include <stdint.h>

void panic(char *str);
void panic_exc(uint8_t num, uint8_t err);


#endif
