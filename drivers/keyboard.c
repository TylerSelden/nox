#include <drivers/keyboard.h>
#include <drivers/io.h>

#include <drivers/vga.h>

char kbd[84] = {
   0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0,
   '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']',
   '\n', 0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
   0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
   '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '7', 0, '9',
   '-', 0, '5', 0, '+', '1', 0, '3', '0', '.'
};
char kbd_shift[84] = {
   0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 0,
   '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}',
   '\n', 0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"', '~',
   0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
   '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '7', 0, '9',
   '-', 0, '5', 0, '+', '1', 0, '3', '0', '.'
};
char kbd_caps[84] = {
   0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0,
   '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '[', ']',
   '\n', 0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'', '`',
   0, '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', ',', '.', '/', 0,
   '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '7', 0, '9',
   '-', 0, '5', 0, '+', '1', 0, '3', '0', '.'
};

uint8_t keyboard_shift = 0;
bool keyboard_caps = false;
uint8_t kbd_buf[2];

void keyboard_init() {
  outb(KEYBOARD_CMD_PORT, 0xae);
  keyboard_input_handler = &dummy_keyboard_input_handler;
}

void keyboard_input() { 
  uint8_t scancode = inb(KEYBOARD_DATA_PORT);
  kbd_buf[0] = scancode;


  // shift and caps lock
  if (scancode == 0x2a || scancode == 0x36) {
    keyboard_shift++;
  } else if (scancode == 0xaa || scancode == 0xb6) {
    keyboard_shift--;
  } else if (scancode == 0x3a) {
    keyboard_caps = !keyboard_caps;
  }

 if (scancode >= 0x80) return; //released key

  // get char
  char str = kbd[scancode];
  if (keyboard_shift) {
    str = kbd_shift[scancode];
  } else if (keyboard_caps) {
    str = kbd_caps[scancode];
  }

  kbd_buf[1] = str;

  (*keyboard_input_handler)();
}

void (*keyboard_input_handler)();
void dummy_keyboard_input_handler() {}
