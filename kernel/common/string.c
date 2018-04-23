#include "string.h"

void * memcpy(void *dest, const void *src, size_t len)
{
	char *d = dest;
	const char *s = src;

	while (len--)
		*d++ = *s++;
	return dest;
}

void * memset(void *dest, int val, size_t len)
{
	uchar *ptr = dest;

	while (len--)
		*ptr++ = val;
	return dest;
}

int memcmp(const void *str1, const void *str2, size_t count)
{
	const uchar *s1 = str1;
	const uchar *s2 = str2;

	while (count--)
	{
		if (*s1++ != *s2++)
			return s1[-1] < s2[-1] ? -1 : 1;
	}
	return 0;
}

char * strcpy(char *dest, const char *src)
{
	char *ptr = dest;

	while (*dest++ = *src++);
	return ptr;
}

char * strcat(char *dest, const char *src)
{
	char * ptr = dest;

	while (*dest)
		*dest++;
	while (*dest++ = *src++);
	return ptr;
}

size_t strlen(const char *str)
{
	char * ptr = (char*)str;

	while (*ptr)
		*ptr++;
	return ptr - str;
}