#include "heap.h"
#include "paging.h"

u32int placement_addr = 0x400000;
HEAP * kernel_heap = NULL;

extern PAGE_DIRECTORY *kernel_dir;

BOOL heap_header_predicate(type_t a, type_t b)
{
    return (((HEAP_BLOCK_HEADER*)a)->size < ((HEAP_BLOCK_HEADER*)b)->size);
}

/*
    Функция поиска подходящего фрагмента памяти наименьшего размера
*/
static s32int heap_find_smallest_chunk(HEAP *heap, u8int align, u32int size)
{
    u32int iter;

    while (iter < heap->index.size)
    {
        HEAP_BLOCK_HEADER *header = (HEAP_BLOCK_HEADER*)array_find(&heap->index, iter);
        if (align)
        {
            u32int location = (u32int)header;
            s32int offset;

            if ((location + sizeof(HEAP_BLOCK_HEADER)) & 0xfffff000)
                offset = PAGE_SIZE - (location + sizeof(HEAP_BLOCK_HEADER)) % PAGE_SIZE;
            
            s32int chunk_size = (s32int)header->size - offset;
            // Может ли данный фрагмент памяти быть использован?
            if (chunk_size >= (s32int)size)
                break;
        }
        else if (header->size >= size)
            break;
        iter++;
    }

    if (iter == heap->index.size)
        return -1; // Был достигнут конец массива и ничего не найдено
    return iter;
}

/*
    Функция для расширения размера heap
*/
static void heap_expand(HEAP *heap, u32int new_size)
{
    ASSERT(new_size > heap->end_addr - heap->start_addr);

    if (new_size & 0xfffff000)
    {
        new_size &= 0xfffff000;
        new_size += PAGE_SIZE;
    }

    // Проверка на предмет возможности расширения heap
    ASSERT(heap->start_addr + new_size <= heap->max_addr);

    u32int old_size = heap->end_addr - heap->start_addr;

    for (u32int i = old_size; i < new_size; i += PAGE_SIZE)
    {
        PAGE *page = paging_get_page_by_address(heap->start_addr + i, TRUE, kernel_dir);
        ASSERT(page);
        paging_alloc_frame(page, heap->is_kernel_mode, heap->is_rw);
    }
    heap->end_addr = heap->start_addr + new_size;
}

static void * malloc(HEAP *heap, u8int align, u32int size)
{
    ASSERT(heap);

    // Вычисление нового размера для блока с учётом хедера и футера
    u32int new_size = size + sizeof(HEAP_BLOCK_HEADER) + sizeof(HEAP_BLOCK_FOOTER);
    // Попытка поиска наименьшего подходящего фрагмента памяти
    s32int iter = heap_find_smallest_chunk(kernel_heap, align, size);

    // Подходящий фрагмент памяти не был найден
    if (iter == -1)
    {
        u32int old_heap_length = heap->end_addr - heap->start_addr;
        u32int old_end_addr = heap->end_addr;

        // Необходимо выделить больше места
        heap_expand(heap, old_heap_length + size);
        u32int new_heap_length = heap->end_addr - heap->start_addr;

        // Поиск финального хедера
        u32int index = -1;
        u32int value = 0;

        for (iter = 0; iter < heap->index.size; iter++)
        {
            u32int tmp = (u32int)array_find(&heap->index, iter);
            if (tmp > value)
            {
                value = tmp;
                index = iter;
            }
        }

        // Если не был найден ни один хедер, то его необходимо создать и добавить
        if (index == -1)
        {
            HEAP_BLOCK_HEADER * header = (HEAP_BLOCK_HEADER*)old_end_addr;
            header->magic = HEAP_MAGIC;
            header->size = new_heap_length - old_heap_length;
            header->is_free = TRUE;

            HEAP_BLOCK_FOOTER * footer = (HEAP_BLOCK_FOOTER*)(old_end_addr + header->size - sizeof(HEAP_BLOCK_FOOTER));
            footer->magic = HEAP_MAGIC;
            footer->header = header;
            array_insert(&heap->index, (void*)header);
        }
        else
        {
            HEAP_BLOCK_HEADER * header = array_find(&heap->index, index);
            header->size += new_heap_length - old_heap_length;

            HEAP_BLOCK_FOOTER * footer = (HEAP_BLOCK_FOOTER*)(header + header->size - sizeof(HEAP_BLOCK_FOOTER));
            footer->magic = HEAP_MAGIC;
            footer->header = header;
        }
        // Теперь достаточно места. Рекурсивный вызов malloc
        return malloc(heap, align, size);
    }

    HEAP_BLOCK_HEADER * orig_chunk_header = array_find(&heap->index, iter);
    u32int orig_chunk_pos = (u32int)orig_chunk_header;
    u32int orig_chunk_size = orig_chunk_header->size;

    if (orig_chunk_size - new_size < sizeof(HEAP_BLOCK_HEADER) + sizeof(HEAP_BLOCK_FOOTER))
    {
        size += orig_chunk_size - new_size;
        new_size = orig_chunk_size;
    }

    if (align && orig_chunk_pos & 0xfffff000)
    {
        // Необходимо выровнять данные по границе страницы
        u32int new_footer_pos = orig_chunk_pos + PAGE_SIZE - (orig_chunk_pos & 0x00000fff) - sizeof(HEAP_BLOCK_HEADER);
        
        HEAP_BLOCK_HEADER * chunk_header = (HEAP_BLOCK_HEADER *)orig_chunk_pos;
        chunk_header->size = PAGE_SIZE - (orig_chunk_pos & 0x00000fff) - sizeof(HEAP_BLOCK_HEADER);
        chunk_header->magic = HEAP_MAGIC;
        chunk_header->is_free = TRUE;

        HEAP_BLOCK_FOOTER * chunk_footer = (HEAP_BLOCK_FOOTER*)(new_footer_pos - sizeof(HEAP_BLOCK_FOOTER));
        chunk_footer->magic = HEAP_MAGIC;
        chunk_footer->header = chunk_header;

        orig_chunk_pos = new_footer_pos;
        orig_chunk_size = orig_chunk_size - chunk_header->size;
    }
    else
    {
        array_remove(&heap->index, iter);
    }

    // Перезапись оригинального хедера и футера
    HEAP_BLOCK_HEADER * header = (HEAP_BLOCK_HEADER *)orig_chunk_pos;
    header->magic = HEAP_MAGIC;
    header->is_free = TRUE;
    header->size = new_size;

    HEAP_BLOCK_FOOTER * footer = (HEAP_BLOCK_FOOTER*)(orig_chunk_pos + sizeof(HEAP_BLOCK_HEADER) + size);
    footer->magic = HEAP_MAGIC;
    footer->header = header;

    // Необходимо записать новый фрагмент после выделения блока
    if (orig_chunk_size - new_size)
    {
        HEAP_BLOCK_HEADER * chunk_header = (HEAP_BLOCK_HEADER*)(orig_chunk_pos + sizeof(HEAP_BLOCK_HEADER) + size + sizeof(HEAP_BLOCK_FOOTER));
        chunk_header->magic = HEAP_MAGIC;
        chunk_header->is_free = TRUE;
        chunk_header->size = orig_chunk_size - new_size;

        HEAP_BLOCK_FOOTER * chunk_footer = (HEAP_BLOCK_FOOTER*)(chunk_header + orig_chunk_size - new_size - sizeof(HEAP_BLOCK_FOOTER));
        if ((u32int)chunk_footer < heap->end_addr)
        {
            chunk_footer->magic = HEAP_MAGIC;
            chunk_footer->header = chunk_header;
        }

        array_insert(&heap->index, (void*)chunk_header);
    }

    return (void*)(header + sizeof(HEAP_BLOCK_HEADER));
}

static void free(HEAP *heap, void *ptr)
{
    // TODO:
}

u32int kmalloc(u32int size, u8int align, u32int *phys)
{
    if (kernel_heap)
    {
        void *addr = malloc(kernel_heap, align, size);
        if (phys)
        {
            PAGE *page = paging_get_page_by_address((u32int)addr, FALSE, kernel_dir);
            ASSERT(page);
            *phys = page->frame * PAGE_SIZE + (u32int)addr & 0xfff;
        }
        return (u32int)addr;
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

void kfree(void *ptr)
{
    free(kernel_heap, ptr);
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
