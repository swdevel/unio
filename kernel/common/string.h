#ifndef _STRING_H_

#include "common.h"

void * memcpy(void *dest, const void *src, size_t len);
void * memset(void *dest, int val, size_t len);
int memcmp(const void *str1, const void *str2, size_t count);
char * strcpy(char *dest, const char *src);
char * strcat(char *dest, const char *src);
size_t strlen(const char *str);

#endif /* _MEMORY_H_ */
