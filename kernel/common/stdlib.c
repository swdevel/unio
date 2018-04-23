#include "stdlib.h"

int abs(int n)
{
	int mask = (n >> (sizeof(int) * CHAR_BIT - 1));
	return (n + mask) ^ mask;
}