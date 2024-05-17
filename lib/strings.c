#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <lib/strings.h>

size_t strlen(char *str) {
  size_t len = 0;
  while (str[len]) len++;
  return len;
}

bool strcmp(char *stra, char *strb) {
  while ((*stra == *strb) && *strb) {
    stra++;
    strb++;
  }
  return (((int) (unsigned char) *stra) - ((int) (unsigned char) *strb) == 0);
}
