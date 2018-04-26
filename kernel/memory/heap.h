#ifndef _HEAP_H_
#define _HEAP_H_

#include "../common/common.h"
#include "../common/array.h"

#define HEAP_BEGIN_ADDR     0xC0000000
#define HEAP_MIN_SIZE       0x00400000
#define HEAP_INITIAL_SIZE   0x00100000
#define HEAP_INDEX_SIZE     0x00020000
#define HEAP_MAGIC          0xdeadbeef

typedef struct
{
    u32int magic;
    u8int is_free;
    u32int size;
} HEAP_BLOCK_HEADER;

typedef struct
{
    u32int magic;
    HEAP_BLOCK_HEADER *header;
} HEAP_BLOCK_FOOTER;

typedef struct
{
    ARRAY index;
    u32int start_addr;
    u32int end_addr;
    u32int max_addr;
    u8int is_kernel_mode;
    u8int is_rw;
} HEAP;

HEAP * heap_create(u32int start_addr, u32int end_addr, u32int max_addr, u8int is_kernel_mode, u8int is_rw);
u32int kmalloc(u32int size, u8int align, u32int *phys);

#endif /* _HEAP_H_ */