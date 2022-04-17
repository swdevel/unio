#include "tty.h"

u16int* frame_buffer = (u16int*) TTY_FRAME_BUFFER_ADDR;

TTY_STATE state = {
    .cursor.x = 0,
    .cursor.y = 0,
    .color = TTY_COLOR_BLACK << 4 | TTY_COLOR_LIGHTGREEN
};

inline static u16int tty_get_frame_buffer_offset()
{
    return state.cursor.x + state.cursor.y * TTY_CONSOLE_WIDTH;
}

static void tty_scroll_screen()
{
    const u16int last = TTY_CONSOLE_WIDTH * (TTY_CONSOLE_HEIGHT - 1);

    for (u16int i = 0; i < last; i++)
        frame_buffer[i] = frame_buffer[i + TTY_CONSOLE_WIDTH];

    const u8int blank = 0x20; // Код символа "пробел"

    for (u16int i = last; i < TTY_CONSOLE_WIDTH * TTY_CONSOLE_HEIGHT; i++)
        frame_buffer[i] = blank | (state.color << 8);

    state.cursor.y = TTY_CONSOLE_HEIGHT - 1;
}

static void tty_move_cursor_to(const u16int x, const u16int y)
{
    if (x >= TTY_CONSOLE_WIDTH || y >= TTY_CONSOLE_HEIGHT)
        return;

    state.cursor.x = x;
    state.cursor.y = y;

    const u16int offset = tty_get_frame_buffer_offset();

    // Установка младшего байта позиции курсора
    outb(0x3D4, 0x0F); 
    outb(0x3D5, offset & 0xFF);

    // Установка старшего байта позиции курсора
    outb(0x3D4, 0x0E); 
    outb(0x3D5, (offset >> 8) & 0xFF);
}

static void tty_reset_state()
{
    state.cursor.x = 0;
    state.cursor.y = 0;

    state.color = TTY_COLOR_BLACK << 4 | TTY_COLOR_LIGHTGREEN;
}

void tty_init()
{
    tty_reset_state();
    tty_clear_screen();
}

void tty_clear_screen()
{
    const u8int blank = 0x20; // Код символа "пробел"

    for (u16int i = 0; i < TTY_CONSOLE_WIDTH * TTY_CONSOLE_HEIGHT; i++)
        frame_buffer[i] = blank | (state.color << 8);

    tty_move_cursor_to(0, 0);
}

void tty_put_string(const char* string)
{
    if (!string)
        return;

    char* ptr = string;

    while (*ptr != '\0')
        tty_put_char(*ptr++);
}

void tty_put_char(const char c)
{
    switch (c)
    {
    case '\n':
        // Перенос строки
        state.cursor.x = 0;
        state.cursor.y++;
        break;

    case '\r':
        // Возврат каретки на начало строки
        state.cursor.x = 0;
        break;

    case 0x08:
        // Возврат каретки на одну позицию
        if (state.cursor.x > 0)
            state.cursor.x--;
        break;

    case '\t':
        // Табуляция
        state.cursor.x = (state.cursor.x + 8) & ~7;
        break;

    default:
        frame_buffer[tty_get_frame_buffer_offset()] = c | (state.color << 8);
        state.cursor.x++;
        break;
    }

    if (state.cursor.x >= TTY_CONSOLE_WIDTH)
    {
        state.cursor.x = 0;
        state.cursor.y++;
    }

    if (state.cursor.y >= TTY_CONSOLE_HEIGHT)
        tty_scroll_screen();

    tty_move_cursor_to(state.cursor.x, state.cursor.y);
}