#include "common.h"
#include "stdio.h"
#include "../tty/tty.h"

void panic(char *message, char *file, u32int line)
{
    char buf[1024] = { 0 };

    asm("cli");

    sprintf(buf, "\n\rKernel panic: %s (file: %s, line %d)", message, file, line);
    tty_put_string(buf);

    while (TRUE);
}

void assert(char *file, u32int line, char *exp)
{
    char buf[1024] = { 0 };

    asm("cli");

    sprintf(buf, "\n\rAssert: %s (file: %s, line %d)", exp, file, line);
    tty_put_string(buf);

    while (TRUE);
}