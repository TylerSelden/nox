#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_CMD_PORT 0x64
#define KEYBOARD_ENABLE 0xae


char kbdus[84];
char kbdus_shift[84];
char kbdus_caps[84];

uint8_t keyboard_shift;
bool keyboard_capsLock;

//void (*keyHandlers[256])();
static bool keyboard_handleSpecialChar(uint8_t scanCode);



uint8_t lastScanCode;
uint8_t scanCodeBuf;
char keyBuf;
static void enableKeyboard();
static void handleKeyboardInput();
