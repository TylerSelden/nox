static void err(char *str) {
  uint8_t colorCpy = color;
  changeColor(vga_color_red, vga_color_black);
  print("Kernel-level error: ");
  printf(str);
  color = colorCpy;
}
