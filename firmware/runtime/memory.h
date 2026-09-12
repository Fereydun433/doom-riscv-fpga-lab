#ifndef LAB_MEMORY_H
#define LAB_MEMORY_H

#include <stddef.h>

void *memcpy(void *restrict destination, const void *restrict source, size_t count);
void *memset(void *destination, int value, size_t count);
int memcmp(const void *left, const void *right, size_t count);
int test_memory_functions(void);

#endif
