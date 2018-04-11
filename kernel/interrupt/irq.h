#ifndef _IRQ_H_
#define _IRQ_H_

#include "../common/common.h"

// ����� ������� ������������ ���������� (IDT)
#define IRQ_DESC_TABLE_ADDR		0x00000000
// ����� �������� IDTR (������ ��� �������� ������ IDT � � ����� � ������)
#define IRQ_IDTR_ADDR			0x00100800

#define IRQ0	0x20	// ���������� �� ���������� �������
#define IRQ1	0x21	// ���������� �� ����������
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
#define IRQ14	0x0E	// ���������� Page fault
#define IRQ15

// ���� �����
#define IRQ_GATEWAY_TYPE_INT	0x8E // ����������
#define IRQ_GATEWAY_TYPE_TRAP	0x8F // �������

// ���� ���������
#define IRQ_SELECTOR_TYPE_CODE	0x08

void irq_init();

#endif /* _IRQ_H_ */