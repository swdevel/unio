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

/*
    Макросы для работы с bitset
*/
#define INDEX_FROM_BIT(frame)   (frame / 32)
#define OFFSET_FROM_BIT(frame)  (frame % 32)

/*
    Количество страниц в одной таблице
*/
#define PAGES_PER_TABLE     1024
/*
    Количество таблиц в одном каталоге
*/
#define TABLES_PER_DIR      1024

/*
    Структура для описания страницы
*/
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

/*
    Структура для описания таблицы страниц
*/
typedef struct
{
    PAGE pages[PAGES_PER_TABLE];
} PAGE_TABLE;

/*
    Структура для описания каталога с таблицами страниц
*/
typedef struct
{
    PAGE_TABLE *tables[TABLES_PER_DIR];  // Массив указателей на таблицы страниц
    u32int tables_phys[TABLES_PER_DIR];  // Массив указателей на физ. адреса таблиц страниц
    u32int phys_addr;                    // Физ. адрес tables_phys
} PAGE_DIRECTORY;

/*
    Функция настройки каталогов страниц и включения страничной адресации
*/
void paging_init();
/*
    Функция получения страницы по заданному адресу
*/
PAGE * paging_get_page_by_address(u32int addr, u8int need_to_create, PAGE_DIRECTORY *dir);
/*
    Функция выделения фрейма
*/
void paging_alloc_frame(PAGE *page, u8int is_kernel, u8int is_writable);

#endif /* _PAGING_H_ */
