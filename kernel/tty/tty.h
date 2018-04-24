#ifndef _TTY_H_
#define _TTY_H_
    
#include "../common/common.h"

/* Адрес кадрового буфера в текстовом режиме */
#define TTY_FRAME_BUFFER_ADDR    0x000b8000

/* Количество символов в строке */
#define TTY_CONSOLE_WIDTH   80
/* Количество строк на экране */
#define TTY_CONSOLE_HEIGHT  25

#define TTY_COLOR_BLACK         0x00
#define TTY_COLOR_BLUE          0x01
#define TTY_COLOR_GREEN         0x02
#define TTY_COLOR_CYAN          0x03
#define TTY_COLOR_RED           0x04
#define TTY_COLOR_MAGENTA       0x05
#define TTY_COLOR_BROWN         0x06
#define TTY_COLOR_LIGHTGREY     0x07    
#define TTY_COLOR_DARKGREY      0x08
#define TTY_COLOR_LIGHTBLUE     0x09
#define TTY_COLOR_LIGHTGREEN    0x0A
#define TTY_COLOR_LIGHTCYAN     0x0B
#define TTY_COLOR_PINK          0x0C
#define TTY_COLOR_LIGHTMAGENTA  0x0D
#define TTY_COLOR_YELLOW        0x0E
#define TTY_COLOR_WHITE         0x0F

/* Структура для описания текущих параметров консоли */
typedef struct
{
    Point cursor;    // Текущее положение курсора
    /*
        Принцип формирования атрибута цвета символа:

        [1 byte]
        xxxx .... - Цвет фона (может принимать значения от 0x00 до 0x07)
        .... xxxx - Цвет текста (может принимать значения от 0x00 до 0x0F)
    */
    u8int color;    // Атрибут (цвет фона и буквы) следующего символа, который будет напечатан в консоли
} TTY_PARAMS;

void tty_cursor_move_to(u16int x, u16int y);
void tty_clear_screen();
void tty_put_char(char c);
void tty_put_string(char *str);

#endif /* _TTY_H_ */