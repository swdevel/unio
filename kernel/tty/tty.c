#include "tty.h"

TTY_PARAMS tty_params = {{0, 0}, TTY_COLOR_BLACK << 4 | TTY_COLOR_LIGHTGREEN};
s8int *fb_addr = (s8int*)TTY_FRAME_BUFFER_ADDR;

/*
	Функция скролла экрана
*/
static void tty_scroll()
{
	int size = TTY_CONSOLE_WIDTH * (TTY_CONSOLE_HEIGHT - 1);

	for (int i = 0; i < size; i++)
	{
		*(fb_addr + i * 2) = *(fb_addr + ((i + TTY_CONSOLE_WIDTH) * 2));
		*(fb_addr + i * 2 + 1) = *(fb_addr + ((i + TTY_CONSOLE_WIDTH) * 2) + 1);
	}

	for (int i = size; i < TTY_CONSOLE_WIDTH * TTY_CONSOLE_HEIGHT; i++)
	{
		*(fb_addr + i * 2) = 0x20; // Код символа "пробел"
		*(fb_addr + i * 2 + 1) = tty_params.color;
	}

	tty_cursor_move_to(tty_params.cursor.x, tty_params.cursor.y);
}

/*
	Функция установки курсора по заданным координатам
*/
void tty_cursor_move_to(u16int x, u16int y)
{
	u16int position;

	if (x > TTY_CONSOLE_WIDTH || y > TTY_CONSOLE_HEIGHT)
		return;

	tty_params.cursor.x = x;
	tty_params.cursor.y = y;

	position = tty_params.cursor.y * TTY_CONSOLE_HEIGHT + tty_params.cursor.x;
	// Установка младшего байта позиции курсора
	outb(0x3D4, 0x0F); 
	outb(0x3D5, position & 0xFF);
	// Установка старшего байта позиции курсора
	outb(0x3D4, 0x0E); 
	outb(0x3D5, (position >> 8) & 0xFF);
}

/*
	Функция очистки экрана
*/
void tty_clear_screen()
{
	for (int i = 0; i < TTY_CONSOLE_WIDTH * TTY_CONSOLE_HEIGHT; i++)
	{
		*(fb_addr + i * 2) = 0x20; // Код символа "пробел"
		*(fb_addr + i * 2 + 1) = tty_params.color;
	}
	tty_cursor_move_to(0, 0);
}

/*
	Функция вывода символа на экран в текущих координатах
*/
void tty_put_char(char c)
{
	u16int offset;

	switch (c)
	{
	case '\n':
		// Перенос строки
		tty_params.cursor.x = 0;
		tty_params.cursor.y++;
		break;
	case '\r':
		// Возврат каретки на начало строки
		tty_params.cursor.x = 0;
		break;
	case 0x08:
		// Возврат каретки на одну позицию
		if (tty_params.cursor.x > 0)
			tty_params.cursor.x--;
		break;
	case '\t':
		// Табуляция
		tty_params.cursor.x = (tty_params.cursor.x + 8) & ~7;
		break;
	default:
		// Вывод всех остальных символов
		offset = tty_params.cursor.x + tty_params.cursor.y * TTY_CONSOLE_WIDTH;
		*(fb_addr + offset * 2) = c;
		*(fb_addr + offset * 2 + 1) = tty_params.color;
		tty_params.cursor.x++;
		break;
	}

	if (tty_params.cursor.x >= TTY_CONSOLE_WIDTH)
	{
		tty_params.cursor.x = 0;
		tty_params.cursor.y++;
	}

	// Скролл экрана
	if (tty_params.cursor.y >= TTY_CONSOLE_HEIGHT)
		tty_scroll();
}

/*
	Функция вывода строки на экран в текущих координатах
*/
void tty_put_string(char *str)
{
	char * ptr = str;

	if (ptr)
		while (*ptr != '\0')
		{
			tty_put_char(*ptr);
			*ptr++;
		}
}