#include "entry.h"

void _start()
{
	// Инициализация системы обработки прерываний
	irq_init();

	while (1);
}