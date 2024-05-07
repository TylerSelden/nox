uint8_t lastScanCode = 0;

static void enableKeyboard() {
  outb(KEYBOARD_CMD_PORT, 0xae);
}


static void handleKeyboardInput() {
  uint8_t scanCode = inb(KEYBOARD_DATA_PORT);
  if (scanCode == 0 || scanCode == lastScanCode) return;
  lastScanCode = scanCode;
  // get char
  char str = kbdus[scanCode];
  if (keyboard_capsLock) str = kbdus_caps[scanCode];
  if (keyboard_shift > 0) str = kbdus_shift[scanCode];
 

  if (keyboard_handleSpecialChar(scanCode) || str == 0) return;
  if (scanCode >= 0x80) return;


  printc(str);
}
