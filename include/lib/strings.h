#ifndef LIB_STRINGS_H
#define LIB_STRINGS_H


#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

size_t strlen(char *str);

bool strncmp(char *stra, char *strb, size_t len);
bool strcmp(char *stra, char *strb);

#endif
