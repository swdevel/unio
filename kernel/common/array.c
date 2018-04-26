#include "array.h"
#include "string.h"
#include "../memory/heap.h"

BOOL array_less_than_predicate_default(type_t a, type_t b)
{
    return (a < b);
}

void array_init(ARRAY *array, void *addr, u32int max_size, predicate_less_than_t predicate)
{
    array->array = addr;
    memset(addr, 0, max_size * sizeof(type_t));
    array->predicate = predicate;
    array->max_size = max_size;
    array->size = 0;
}

void array_insert(ARRAY *array, type_t *item)
{
    u32int iter = 0;

    ASSERT(array && item);
    
    if (array->size == array->max_size)
    {
        PANIC("The array is full!");
    }

    while (iter < array->size && array->predicate(array->array[iter], item))
    {
        iter++;
    }

    if (iter == array->size)
        array->array[array->size++] = item; // Добавление элемента в конец массива
    else
    {
        // Сортировка массива при вставке нового элемента
        type_t tmp = array->array[iter];
        while (iter < array->size)
        {
            type_t tmp2 = array->array[++iter];
            array->array[iter] = tmp;
            tmp = tmp2;
        }
        array->size++;
    }
}