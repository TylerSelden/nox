static void err(char *str) {
  uint8_t colorCpy = color;
  changeColor(vga_color_red, vga_color_black);
  print("Kernel-level error: ");
  printf(str);
  color = colorCpy;
}

static void dbg(char *str) {
  uint8_t colorCpy = color;
  changeColor(vga_color_green, vga_color_black);
  print("Debug: ");
  printf(str);
  color = colorCpy;
}
