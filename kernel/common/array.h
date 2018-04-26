#ifndef _ARRAY_H_
#define _ARRAY_H_

#include "common.h"

typedef void * type_t;

/*
    Предикат возвращает значение, отличное от нуля, если первый аргумент 
    меньше чем второй.
*/
typedef BOOL(*predicate_less_than_t) (type_t, type_t);

typedef struct
{
    type_t *array;
    u32int size;
    u32int max_size;

    predicate_less_than_t predicate; // Указатель на функцию-предикат
} ARRAY;

void array_init(ARRAY *array, void *addr, u32int max_size, predicate_less_than_t predicate);
void array_insert(ARRAY *array, type_t *item);

BOOL array_less_than_predicate_default(type_t a, type_t b);

#endif /* _ARRAY_H_ */
