static void tty_setBufAtCursor() {
  ttyBuf = (char*) vga_cursorToVidmem();
}

static bool tty_handleSpecialChar(uint8_t scanCode) {
  if (scanCode == 0x4b) {
    if (vga_cursorToVidmem() > ttyBuf) vga_cursorLeftf();
  } else if (scanCode == 0x48) {
    // last command?
  } else if (scanCode == 0x4d) {
    // make sure cursor isn't going to new line
    if (*vga_cursorToVidmem() != 0) vga_cursorRightf();
  } else if (scanCode == 0x50) {
    // next command?
  } else if (keyBuf == '\b') {
    if (vga_cursorToVidmem() > ttyBuf) insertc('\b');
  } else {
    return false;
  }
  return true;
}

static void tty_handleInput() {
  if (!scanCodeBuf) return;

  if (!tty_handleSpecialChar(scanCodeBuf)) insertc(keyBuf);
  scanCodeBuf = 0;
  keyBuf = 0;
}

static void ttyInit() {
  printf("Welcome to Nox!");
  printf("\nThis is the beginnings of an amateur OS. At the moment it can't do much.");
  printf("\nRight now, terminal support is being worked on. In the meantime, enjoy.\n");
  print("> ");

  tty_setBufAtCursor();
}
