#include "paging.h"
#include "../tty/tty.h"
#include "../common/stdio.h"

/*
    Указатель на каталог страниц ядра
*/
PAGE_DIRECTORY *kernel_dir = NULL;

/*
    Указатель на текущий каталог страниц
*/
PAGE_DIRECTORY *current_dir = NULL;

/*
    Указатель определен в heap.c
*/
extern HEAP *kernel_heap;

/*
    Указатель определен в heap.c
*/
extern u32int placement_addr;

u32int *frames = NULL;

PAGE * paging_get_page_by_address(u32int addr, u8int create_if_not_exist, PAGE_DIRECTORY *dir)
{
    u32int index;

    addr /= 4096;
    index = addr / 1024;

    // Поиск таблицы страниц, к которой относится данный адрес
    if (dir->tables[index])
    {
        // Страница уже существует
        return &dir->tables[index]->pages[addr % 1024];
    }
    else if (create_if_not_exist)
    {
        u32int phys;

        // Создание страницы
        dir->tables[index] = (PAGE_TABLE*) kmalloc(sizeof(PAGE_TABLE), TRUE, &phys);
        memset(dir->tables[index], 0, 4096);
        dir->tables_phys[index] = phys | 0x07; // Находится в памяти, доступна для записи, user space
        
        return &dir->tables[index]->pages[addr % 1024];
    }
    return NULL;
}

static u32int paging_get_firts_free_frame_index()
{
    u32int n_frames = PAGING_PHYS_MEMORY_END / 4096;

    for (int i = 0; i < INDEX_FROM_BIT(n_frames); i++)
    {
        if (frames[i] == 0xffffffff)
        {
            // Отсутствуют свободные фреймы
            continue;
        }

        for (int j = 0; j < 32; j++)
        {
            u32int shifted = 0x01 << j;
            if (!(frames[i] & shifted))
            {
                return i * 32 + j;
            }
        }
    }
    return 0xffffffff;
}

static void paging_set_frame_present_bit(u32int frame_addr)
{
    u32int frame = frame_addr / 4096;
    u32int index = INDEX_FROM_BIT(frame);
    u32int offset = OFFSET_FROM_BIT(frame);

    frames[index] |= (0x01 << offset);
}

static void paging_clear_frame_present_bit(u32int frame_addr)
{
    u32int frame = frame_addr / 4096;
    u32int index = INDEX_FROM_BIT(frame);
    u32int offset = OFFSET_FROM_BIT(frame);

    frames[index] &= ~(0x01 << offset);
}

static void paging_alloc_frame(PAGE *page, u8int is_kernel, u8int is_writable)
{
    ASSERT(page != NULL);

    if (page->frame)
    {
        // Для данной страницы уже был создан фрейм
        return;
    }

    u32int index = paging_get_firts_free_frame_index();
    if (index == 0xffffffff)
    {
        PANIC("No free frames!");
    }

    paging_set_frame_present_bit(index * 4096);

    page->present = TRUE;
    page->rw = is_writable ? TRUE : FALSE;
    page->user = is_kernel ? FALSE : TRUE;
    page->frame = index;
}

static void paging_switch_page_directory(PAGE_DIRECTORY *dir)
{
    current_dir = dir;
    u32int cr0;

    asm volatile ("mov %0, %%cr3":: "r"(&dir->tables_phys));
    asm volatile("mov %%cr0, %0": "=r"(cr0));
    cr0 |= 0x80000000; // Включение страничной адресации
    asm volatile("mov %0, %%cr0":: "r"(cr0));
}

void paging_init()
{
    u32int n_frames;

    // Вычисление количества фреймов, которые могут быть размещены в физической памяти
    n_frames = PAGING_PHYS_MEMORY_END / 4096;

    frames = (u32int*) kmalloc(INDEX_FROM_BIT(n_frames), FALSE, NULL);
    memset(frames, 0, INDEX_FROM_BIT(n_frames));

    // Создание каталога страниц ядра
    kernel_dir = (PAGE_DIRECTORY*) kmalloc(sizeof(PAGE_DIRECTORY), TRUE, NULL);
    memset(kernel_dir, 0, sizeof(PAGE_DIRECTORY));
    current_dir = kernel_dir;

    for (int i = HEAP_BEGIN_ADDR; i < HEAP_BEGIN_ADDR + HEAP_INITIAL_SIZE; i += 4096)
    {
        paging_get_page_by_address(i, TRUE, kernel_dir);
    }

    // Создание карты идентичности (физ. адрес = вирт. адресу)
    for (int i = 0; i < placement_addr + 4096; i += 4096)
    {
        PAGE * page = paging_get_page_by_address(i, TRUE, kernel_dir);
        ASSERT(page != NULL);
        paging_alloc_frame(page, FALSE, FALSE);
    }

    for (int i = HEAP_BEGIN_ADDR; i < HEAP_BEGIN_ADDR + HEAP_INITIAL_SIZE; i += 4096)
    {
        PAGE * page = paging_get_page_by_address(i, TRUE, kernel_dir);
        ASSERT(page != NULL);
        paging_alloc_frame(page, FALSE, FALSE);
    }

    // Включение страничной адресации
    paging_switch_page_directory(kernel_dir);
    // Создание heap ядра (для примера, пусть максимальный размер будет равен 8 мб)
    kernel_heap = heap_create(HEAP_BEGIN_ADDR, HEAP_BEGIN_ADDR + HEAP_INITIAL_SIZE, 0x00800000, FALSE, FALSE);
}