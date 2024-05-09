static void tty_setBufAtCursor();

static bool tty_handleSpecialChar(uint8_t scanCode);

static void tty_handleInput();

static void ttyInit();

char *ttyBuf;
