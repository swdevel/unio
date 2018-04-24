#ifndef _PCI_H_
#define _PCI_H_

#include "../common/common.h"
#include "../common/stdio.h"
#include "../tty/tty.h"

#define PCI_PORT_CONFIG     0x0cf8
#define PCI_PORT_DATA       0x0cfc

#define PCI_MAX_BUSES       255
#define PCI_MAX_DEVICES     32
#define PCI_MAX_FUNCTIONS   8

#define PCI_HEADERTYPE_NORMAL           0x00
#define PCI_HEADERTYPE_BRIDGE           0x01
#define PCI_HEADERTYPE_CARDBUS          0x02
#define PCI_HEADERTYPE_MULTIFUNCTION    0x80

typedef union
{
	struct
	{
		u16int vendor_id;
		u16int device_id;
		u16int command_reg;
		u16int status_reg;
		u8int  revision_id;
		u8int  prog_if;
		u8int  sub_class_code;
		u8int  class_code;
		u8int  cache_line_size;
		u8int  latency;
		u8int  header_type;
		u8int  BIST;
	}  __attribute__((packed)) option;
	u32int	header[4];
} __attribute__((packed)) PCI_DEV_HEADER;

typedef struct
{
	u32int class_code;
	char name[32];
} PCI_CLASS_NAME;

typedef union
{
	struct
	{
		u32int zero : 2;
		u32int reg_num : 6;
		u32int func_num : 3;
		u32int dev_num : 5;
		u32int bus_num : 8;
		u32int reserved : 7;
		u32int enable_bit : 1;
	};
	u32int value;
} PCI_CONFIG_ADDR;

void pci_scan_bus();

#endif /* _PCI_H_ */
