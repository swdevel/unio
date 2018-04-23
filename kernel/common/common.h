#ifndef _COMMON_H_
#define _COMMON_H_

#ifndef NULL
	#define NULL 0
#endif

#ifndef TRUE
	#define TRUE 1
#endif

#ifndef FALSE
	#define FALSE 0
#endif

#ifndef CHAR_BIT
	#define CHAR_BIT 8
#endif


typedef unsigned int   u32int;
typedef int            s32int;
typedef unsigned short u16int;
typedef short          s16int;
typedef unsigned char  u8int;
typedef char           s8int;

typedef unsigned char  uchar;
typedef unsigned int   size_t;

/* Структура для описания точки */
typedef struct
{
	u16int x;
	u16int y;
} Point;

inline static void outb(u16int port, u8int value)
{
	asm volatile("outb %1, %0" : : "dN" (port), "a" (value));
}

inline static u8int inb(u16int port)
{
	u8int value;
	asm volatile("inb %1, %0" : "=a" (value) : "dN" (port));
	return value;
}

inline static u16int inw(u16int port)
{
	u16int value;
	asm volatile("inw %1, %0" : "=a" (value) : "dN" (port));
	return value;
}

inline static void outw(u16int port, u16int value)
{
	asm volatile("outw %1, %0" : : "dN" (port), "a" (value));
}

/* Макросы для работы с битовыми масками */
#define SetFlag(v, f)       (v |= (1 << f))
#define ClearFlag(v, f)     (v &= ~(1 << f))
#define CheckFlag(v, f)     (v & (1 << f))

#endif /* _COMMON_H_ */