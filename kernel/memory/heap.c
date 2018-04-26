#include "heap.h"

u32int placement_addr = 0x400000;
HEAP * kernel_heap = NULL;

BOOL heap_header_predicate(type_t a, type_t b)
{
    return (((HEAP_BLOCK_HEADER*)a)->size < ((HEAP_BLOCK_HEADER*)b)->size);
}

u32int kmalloc(u32int size, u8int align, u32int *phys)
{
    if (kernel_heap)
    {
        // TODO:
        PANIC("Not implemented yet!");
    }
    else
    {
        if (align && (placement_addr & 0xFFFFF000))
        {
            // Адрес будет выровнен по границе в 4 кб
            placement_addr &= 0xFFFFF000;
            placement_addr += PAGE_SIZE;
        }
        if (phys)
        {
            *phys = placement_addr;
        }
        u32int placement_addr_value = placement_addr;
        placement_addr += size;

        return placement_addr_value;
    }
}

HEAP * heap_create(u32int start_addr, u32int end_addr, u32int max_addr, u8int is_kernel_mode, u8int is_rw)
{
    HEAP * heap;

    ASSERT(!(start_addr % PAGE_SIZE));
    ASSERT(!(end_addr % PAGE_SIZE));
    
    heap = (HEAP*) kmalloc(sizeof(heap), FALSE, NULL);

    // Инициализация массива индексов
    array_init(&heap->index, (void*)start_addr, HEAP_INDEX_SIZE, &heap_header_predicate);

    // Получение нового свободного адреса после инициализации массива индексов
    start_addr += sizeof(type_t) * HEAP_INDEX_SIZE;

    // Адрес должен быть выровнен по границе страницы
    if (start_addr & 0xFFFFF000)
    {
        start_addr &= 0xFFFFF000;
        start_addr += PAGE_SIZE;
    }

    heap->start_addr = start_addr;
    heap->end_addr = end_addr;
    heap->max_addr = max_addr;
    heap->is_kernel_mode = is_kernel_mode;
    heap->is_rw = is_rw;

    // Создание одного большого блока памяти
    HEAP_BLOCK_HEADER * header = (HEAP_BLOCK_HEADER *)start_addr;
    header->size = end_addr - start_addr;
    header->magic = HEAP_MAGIC;
    header->is_free = TRUE;

    array_insert(&heap->index, (type_t*)header);

    return heap;
}
