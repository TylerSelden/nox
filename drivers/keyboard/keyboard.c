#include "./chars.c"

uint8_t lastScanCode = 0;
char keyBuf = 0;

static void enableKeyboard() {
  outb(KEYBOARD_CMD_PORT, 0xae);
}


static void handleKeyboardInput() {
  uint8_t scanCode = inb(KEYBOARD_DATA_PORT);
  if (scanCode == lastScanCode) return;
  lastScanCode = scanCode;
  
  // handle shift and caps lock
  if (scanCode == 0x2a || scanCode == 0x36) {
    return keyboard_shift++;
  } else if (scanCode == 0xaa || scanCode == 0xb6) {
    return keyboard_shift--;
  } else if (scanCode == 0x3a) {
    return keyboard_capsLock = !keyboard_capsLock;
  }

  // get char
  char str = kbdus[scanCode];
  if (keyboard_capsLock) str = kbdus_caps[scanCode];
  if (keyboard_shift > 0) str = kbdus_shift[scanCode];
 

  // send to key buffer,
  //if (keyboard_handleSpecialChar(scanCode) || str == 0) return;
  if (scanCode >= 0x80) return;

  scanCodeBuf = scanCode;
  keyBuf = str;
}
