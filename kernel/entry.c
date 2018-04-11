#include "entry.h"

void _start()
{
	// »нициализаци€ системы обработки прерываний
	irq_init();

	while (1);
}