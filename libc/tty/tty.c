static void tty_setBufAtCursor() {
  ttyBuf = (char*) vga_cursorToVidmem();
}

static void tty_handleInput() {
  if (!keyBuf) return;
  printc(keyBuf);
  keyBuf = 0;
}

static void ttyInit() {
  enableKeyboard();
  tty_setBufAtCursor();
}
