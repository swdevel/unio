#ifndef _IRQ_H_
#define _IRQ_H_

#include "../common/common.h"
#include "../keyboard/keyboard.h"

// Адрес таблицы дескрипторов прерываний (IDT)
#define IRQ_DESC_TABLE_ADDR		0x00000000
// Адрес регистра IDTR (служит для хранения адреса IDT и её длины в байтах)
#define IRQ_IDTR_ADDR			0x00100800

#define IRQ0	0x20	// Прерывание от системного таймера
#define IRQ1	0x21	// Прерывание от клавиатуры
#define IRQ2
#define IRQ3
#define IRQ4
#define IRQ5
#define IRQ6
#define IRQ7
#define IRQ8
#define IRQ9
#define IRQ10
#define IRQ11
#define IRQ12
#define IRQ13
#define IRQ14	0x0E	// Прерывание Page fault
#define IRQ15

// Типы шлюза
#define IRQ_GATEWAY_TYPE_INT	0x8E // Прерывание
#define IRQ_GATEWAY_TYPE_TRAP	0x8F // Ловушка

// Типы селектора
#define IRQ_SELECTOR_TYPE_CODE	0x08

void irq_init();

#endif /* _IRQ_H_ */