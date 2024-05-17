#ifndef DRIVERS_KEYBOARD_H
#define DRIVERS_KEYBOARD_H


#include <stdint.h>
#include <stdbool.h>

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_CMD_PORT 0x64

extern char kbd[84];
extern char kbd_shift[84];
extern char kbd_caps[84];

extern uint8_t keyboard_shift;
extern bool keyboard_caps; 

extern uint8_t kbd_buf[2];

void keyboard_init();
void keyboard_input();

extern void (*keyboard_input_handler)();
void dummy_keyboard_input_handler();

#endif
