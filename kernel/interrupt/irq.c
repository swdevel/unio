#include "irq.h"

/*
	Функция установки вектора прерывания.
*/
static void irq_set_interrupt_handler(uchar vector, uchar type, void(*func))
{
	char *ptr = (char*)IRQ_DESC_TABLE_ADDR;
	char byte[8];
	int i;

	if (type != IRQ_GATEWAY_TYPE_INT &&
		type != IRQ_GATEWAY_TYPE_TRAP)
		return;

	if (!func)
		return;

	byte[0] = (u32int)func & 0x000000FF;
	byte[1] = ((u32int)func & 0x0000FF00) >> 8;
	byte[2] = IRQ_SELECTOR_TYPE_CODE;
	byte[3] = 0;
	byte[4] = 0;
	byte[5] = type;
	byte[6] = ((u32int)func & 0x00FF0000) >> 16;
	byte[7] = ((u32int)func & 0xFF000000) >> 24;

	for (i = 0; i < 8; i++)
		*(ptr + vector * 8 + i) = byte[i];
}

/*
	Обработчик прерывания от системного таймера.
*/
static void irq_on_timer_handler()
{
	asm("pusha");

	// TODO:

	asm("popa");
	asm("movb $0x20, %al");
	asm("outb %al, $0x20");
	asm("leave");
	asm("iret");
}

/*
	Обработчик прерывания от клавиатуры.
*/
static void irq_on_keyboard_handler()
{
	asm("pusha");

	// TODO:

	asm("popa");
	asm("movb $0x20, %al");
	asm("outb %al, $0x20");
	asm("leave");
	asm("iret");
}

/*
	Обработчик прерывания page fault
*/
static void irq_on_page_fault_handler()
{
	asm("pusha");

	// TODO:

	asm("popa");
	asm("movb $0x20, %al");
	asm("outb %al, $0x20");
	asm("leave");
	asm("iret");
}

void irq_init()
{
	s16int *limit;
	s16int *addr;

	// Запрет прерываний
	asm("cli");

	// Инициализация векторов прерываний обработчиками
	irq_set_interrupt_handler(IRQ0, IRQ_GATEWAY_TYPE_INT, irq_on_timer_handler);
	irq_set_interrupt_handler(IRQ1, IRQ_GATEWAY_TYPE_INT, irq_on_keyboard_handler);
	irq_set_interrupt_handler(IRQ14, IRQ_GATEWAY_TYPE_INT, irq_on_page_fault_handler);

	// Запись в регистр IDTR адреса и размера IDT
	limit = (s16int*)IRQ_IDTR_ADDR;
	addr = (s16int*)IRQ_IDTR_ADDR + 2;
	*limit = 256 * 8 - 1;
	*addr = IRQ_DESC_TABLE_ADDR;
	asm("lidt 0(,%0,)" :: "a" (IRQ_IDTR_ADDR));

	// Разрешение прерываний
	asm("sti");
}