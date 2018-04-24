#include "stdio.h"

static char * put_char(char *buf, char c)
{
	*buf++ = c;
	return buf;
}

static char * put_string(char *buf, char *str)
{
	while (*str != '\0')
	{
		buf = put_char(buf, *str);
		*str++;
	}
	return buf;
}

static char * put_dec_dword(char *buf, int dword)
{
	char table[] = "0123456789";
	char tmp[64];
	int negative = FALSE;
	int len = 0;

	if (dword < 0)
	{
		negative = TRUE;
		dword = abs(dword);
	}

	do
	{
		tmp[len] = table[dword % 10];
		tmp[++len] = '\0';
		dword /= 10;
	} while (dword);

	if (negative)
	{
		tmp[len] = '-';
		tmp[++len] = '\0';
	}

	for (int i = len - 1; i >= 0; i--)
		buf = put_char(buf, tmp[i]);

	return buf;
}

static char * put_hex_byte(char *buf, uchar byte)
{
	char table[] = "0123456789abcdef";
	uchar half;

	half = byte >> 4;
	buf = put_char(buf, table[half]);

	half = byte & 0x0F;
	buf = put_char(buf, table[half]);

	return buf;
}

static char * put_hex_dword(char *buf, u32int dword)
{
	buf = put_hex_byte(buf, (dword & 0xFF000000) >> 24);
	buf = put_hex_byte(buf, (dword & 0x00FF0000) >> 16);
	buf = put_hex_byte(buf, (dword & 0x0000FF00) >> 8);
	buf = put_hex_byte(buf, (dword & 0x000000FF));

	return buf;
}

static int vsprintf(char *buf, const char *format, va_list args)
{
	char *ptr = buf;

	for (; *format != '\0'; format++)
	{
		if (*format == '%')
		{
			format++;

			switch (*format)
			{
			case 'd':
			case 'i':
				buf = put_dec_dword(buf, va_arg(args, int));
				break;

			case 'x':
				buf = put_hex_dword(buf, va_arg(args, u32int));
				break;

			case 's':
				buf = put_string(buf, va_arg(args, char *));
				break;

			case 'c':
				// C wants to promote char type to int.
				buf = put_char(buf, va_arg(args, int));
				break;

			default:
				break;
			}
		}
		else
			buf = put_char(buf, *format);
	}

	return buf - ptr;
}

int sprintf(char *str, const char *format, ...)
{
	va_list args;
	int ret;

	va_start(args, format);
	ret = vsprintf(str, format, args);
	va_end(args);

	return ret;
}