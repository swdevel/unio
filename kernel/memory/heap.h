#ifndef _HEAP_H_
#define _HEAP_H_

#include "../common/common.h"
#include "../common/array.h"

#define HEAP_BEGIN_ADDR     0xC0000000
#define HEAP_MIN_SIZE       0x00400000
#define HEAP_INITIAL_SIZE   0x00100000
#define HEAP_INDEX_SIZE     0x00020000
#define HEAP_MAGIC          0xdeadbeef  // "Магическая" константа

#define PAGE_SIZE           0x1000

typedef struct
{
    u32int magic;   // "Магическое" число для контроля ошибок
    u8int is_free;  // Признак того, что этот фрагмент памяти не используется
    u32int size;    // Размер блока
} HEAP_BLOCK_HEADER;

typedef struct
{
    u32int magic;   // "Магическое" число для контроля ошибок
    HEAP_BLOCK_HEADER *header; // Указатель на заголовок блока
} HEAP_BLOCK_FOOTER;

typedef struct
{
    ARRAY index;            // Массив для хранения указателей на блоки памяти

    u32int start_addr;      // Начало выделяемого пространства памяти
    u32int end_addr;        // Конец выделяемого пространства памяти
    u32int max_addr;        // Максимальный адрес, до которого heap может быть расширен 
    u8int is_kernel_mode;   // Признак того, что запрашиваемые страницы могут использоваться только ядром 
    u8int is_rw;            // Признак того, что запрашиваемые страницы доступны для записи
} HEAP;

HEAP * heap_create(u32int start_addr, u32int end_addr, u32int max_addr, u8int is_kernel_mode, u8int is_rw);
u32int kmalloc(u32int size, u8int align, u32int *phys);
void kfree(void *ptr);

#endif /* _HEAP_H_ */