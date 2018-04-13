#include "entry.h"

void _start()
{
	// Инициализация системы обработки прерываний
	irq_init();
	// Очистка экрана
	tty_clear_screen();
	while (1);
}