int cursorX = 0;
int cursorY = 0;

char *xyToVidmem(int x, int y) {
  char *vidmem = (char*) VIDMEM_ADDR;
  vidmem += x * 2;
  vidmem += y * 2 * MAX_COLS;
  return vidmem;
}

void newl() {
  cursorX = 0;
  cursorY++;
}

void print(char *str) {
  char *vidmem = xyToVidmem(cursorX, cursorY);
  while (*str != 0) {
    cursorX++;
    if (cursorX >= MAX_ROWS) newl();
    if (cursorY >= MAX_COLS) cursorY = 0;
    *vidmem++ = *str++;
    *vidmem++ = 0x0f;
  }
}
