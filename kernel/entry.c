#include "entry.h"

void _start()
{
	// Инициализация системы обработки прерываний
	irq_init();
	// Очистка экрана от сообщения загрузчика и вывод сообщения и получении контроля ядром
	tty_clear_screen();
	tty_put_string("kernel receive control...");
	// Сканирование шины PCI и вывод на экран всех устройств
	pci_scan_bus();

	while (1);
}