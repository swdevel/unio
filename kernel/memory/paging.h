#ifndef _PAGING_H_
#define _PAGING_H_

#include "../common/common.h"
#include "../common/string.h"
#include "../tty/tty.h"
#include "heap.h"

/*
    Размер физической памяти (для примера, пусть будет равен 16 мб)
*/
#define PAGING_PHYS_MEMORY_END   0x1000000

#define INDEX_FROM_BIT(frame)   (frame / 32)
#define OFFSET_FROM_BIT(frame)  (frame % 32)


typedef struct
{
    u32int present  : 1;  // Признак того, что страница находится в памяти
    u32int rw       : 1;  // Признак того, что страница доступна для записи
    u32int user     : 1;  // Признак того, что страница доступа из user mode
    u32int accessed : 1;  // Признак того, что к странице было обращение с момента последнего обновления
    u32int dirty    : 1;  // Признак того, что в страницу были записаны данные с момента последнего обновления
    u32int unused   : 7;  // Не используется
    u32int frame    : 20; // Адрес кадра (со сдвигом вправо на 12 бит)
} PAGE;

typedef struct
{
    PAGE pages[1024];
} PAGE_TABLE;

typedef struct
{
    PAGE_TABLE *tables[1024];  // Массив указателей на таблицы страниц
    u32int tables_phys[1024];  // Массив указателей на физ. адреса таблиц страниц
    u32int phys_addr;          // Физ. адрес tables_phys
} PAGE_DIRECTORY;

void paging_init();
PAGE * paging_get_page_by_address(u32int addr, u8int create_if_not_exist, PAGE_DIRECTORY *dir);

#endif /* _PAGING_H_ */
