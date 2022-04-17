#include "entry.h"

void _start()
{
    // Инициализация системы обработки прерываний
    irq_init();

    // Очистка экрана от сообщения загрузчика и вывод сообщения и получении контроля ядром
    tty_init();
    tty_put_string("Kernel received control from boot loader\n");

    // Сканирование шины PCI и вывод на экран всех устройств
    tty_put_string("PCI bus devices:\n");
    pci_scan_bus();

    // Включение страничной адресации
    paging_init();

    tty_put_string("Ready to test keyboard...\n");

    while (1);
}