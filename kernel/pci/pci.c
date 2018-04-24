#include "pci.h"

static PCI_CLASS_NAME class_names[] =
{
	{ 0x00, "before PCI 2.0" },
	{ 0x01, "disk controller" },
	{ 0x02, "network interface" },
	{ 0x03, "graphics adapter" },
	{ 0x04, "multimedia controller" },
	{ 0x05, "memory controller" },
	{ 0x06, "bridge device" },
	{ 0x07, "communication controller" },
	{ 0x08, "system device" },
	{ 0x09, "input device" },
	{ 0x0A, "docking station" },
	{ 0x0B, "CPU" },
	{ 0x0C, "serial bus" },
	{ 0x0D, "wireless controller" },
	{ 0x0E, "intelligent I/O controller" },
	{ 0x0F, "satellite controller" },
	{ 0x10, "encryption controller" },
	{ 0x11, "signal processing controller" },
	{ 0xFF, "proprietary device" }
};

static char * pci_get_class_name_by_code(u32int code)
{
	for (int i = 0; i < COUNT(class_names); i++)
		if (class_names[i].class_code == code)
			return class_names[i].name;
	return NULL;
}

static void pci_read_config32(u32int bus, u32int device, u32int function, u32int reg, u32int *data)
{
	PCI_CONFIG_ADDR pci_config_addr;

	pci_config_addr.value = 0;
	pci_config_addr.enable_bit = 1;
	pci_config_addr.reg_num = reg;
	pci_config_addr.func_num = function;
	pci_config_addr.dev_num = device;
	pci_config_addr.bus_num = bus;

	outl(PCI_PORT_CONFIG, pci_config_addr.value);
	inl(PCI_PORT_DATA, data);
}

static int pci_read_device_header(PCI_DEV_HEADER *pci_dev_header, u32int bus, u32int device, u32int function)
{
	if (pci_dev_header == NULL)
		return FALSE;

	for (int i = 0; i < COUNT(pci_dev_header->header); i++)
		pci_read_config32(bus, device, function, i, &pci_dev_header->header[i]);

	if (pci_dev_header->option.vendor_id == 0x0000 || pci_dev_header->option.vendor_id == 0xffff ||
		pci_dev_header->option.device_id == 0xffff)
		return FALSE;

	return TRUE;
}

static void pci_trace_device_header(PCI_DEV_HEADER * pci_dev_header, u32int bus, u32int device, u32int function)
{
	char str[256] = {0};
	char *ptr;
	int len;

	len = sprintf(str, "PCI:%d:%d:%d VID = 0x%x, PID = 0x%x", bus, device, function,
					   pci_dev_header->option.vendor_id,
					   pci_dev_header->option.device_id);

	ptr = pci_get_class_name_by_code(pci_dev_header->option.class_code);
	if (ptr)
		sprintf(str + len, " class = %s", ptr);
	tty_put_string(str);
	tty_put_char('\n');
}

void pci_scan_bus()
{
	PCI_DEV_HEADER pci_dev_header;

	for (int bus = 0; bus < PCI_MAX_BUSES; bus++)
		for (int device = 0; device < PCI_MAX_DEVICES; device++)
		{
			if (pci_read_device_header(&pci_dev_header, bus, device, 0) == FALSE)
				continue;
			pci_trace_device_header(&pci_dev_header, bus, device, 0);

			// Multifunction device
			if (pci_dev_header.option.header_type & PCI_HEADERTYPE_MULTIFUNCTION)
				for (int function = 1; function < PCI_MAX_FUNCTIONS; function++)
				{
					if (pci_read_device_header(&pci_dev_header, bus, device, function) == FALSE)
						continue;
					pci_trace_device_header(&pci_dev_header, bus, device, function);
				}
		}
}