#include "./chars.c"

uint8_t lastScanCode = 0;

static void enableKeyboard() {
  outb(0x64, 0xae);
}


static void handleKeyboardInput() {
  uint8_t scanCode = inb(0x60);
  if (scanCode == 0 || scanCode == lastScanCode) return;
  lastScanCode = scanCode;
  // get char
  char str = kbdus[scanCode];
  if (keyboard_capsLock) str = kbdus_caps[scanCode];
  if (keyboard_shift > 0) str = kbdus_shift[scanCode];
 

  if (keyboard_handleSpecialChar(scanCode) || str == 0) return;
  if (scanCode >= 0x80) return; // relase


  vga_printChar(str);
}
