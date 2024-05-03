/* This will force us to create a kernel entry function instead of jumping to kernel.c:0x00 */
//void dummy_test_entrypoint() {
//}

void main() {
  char* vidmem = (char*) 0xb8000;
  *vidmem = 'S';
  
  // string
  char *str = "Hello, World!";
  char *str_ptr = str;
  while (*str_ptr != 0) {
    *vidmem++ = *str_ptr++;
    *vidmem++ = 0x07;
  }
}
