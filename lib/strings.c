#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <lib/strings.h>

size_t strlen(char *str) {
  size_t len = 0;
  while (str[len]) len++;
  return len;
}


bool strncmp(char *stra, char *strb, size_t len) {
  while ((*stra == *strb) && *strb && len > 0) {
    if (*stra != *strb) return false;

    stra++;
    strb++;
    len--;
  }
  return len == 0 || *stra == *strb;
}

bool strcmp(char *stra, char *strb) {
  return strncmp(stra, strb, strlen(strb)) && strncmp(strb, stra, strlen(stra));
}
