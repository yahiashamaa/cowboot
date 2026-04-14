#ifndef IO_H
#define IO_H

#include <stdarg.h>
#include "types.h"

#define KUSEG			0x00000000
#define KSEG0			0x80000000
#define KSEG1			0xa0000000

#define CKUSEG			0x00000000
#define CKSEG0			0x80000000
#define CKSEG1			0xa0000000

#define cache_op(op, addr)		\
	__asm__ __volatile__(		\
		".set	push\n"		\
		".set	noreorder\n"	\
		".set	mips3\n"	\
		"cache	%0, %1\n"	\
		".set	pop\n"		\
		:			\
		: "i" (op), "R" (*(unsigned char *)(addr)))


#define __sync()				\
	__asm__ __volatile__(			\
		".set	push\n\t"		\
		".set	noreorder\n\t"		\
		".set	mips2\n\t"		\
		"sync\n\t"			\
		".set	pop"			\
		: /* no output */		\
		: /* no input */		\
		: "memory")

#define __fast_iob()				\
	__asm__ __volatile__(			\
		".set	push\n\t"		\
		".set	noreorder\n\t"		\
		"lw	$0,%0\n\t"		\
		"nop\n\t"			\
		".set	pop"			\
		: /* no output */		\
		: "m" (*(int *)0xa0000000)	\
		: "memory")

#define fast_iob()				\
	do {					\
		__sync();			\
		__fast_iob();			\
	} while (0)

static inline void writel(u32 val, u32 addr)
{
    *(volatile u32 *)addr = val;
    asm volatile("sync" ::: "memory");
}

static inline void writew(u16 val, u32 addr)
{
    *(volatile u16 *)addr = val;
    asm volatile("sync" ::: "memory");
}

static inline u32 readl(u32 addr)
{
    u32 val = *(volatile u32 *)addr;
    asm volatile("sync" ::: "memory");
    return val;
}

static inline u16 readw(u32 addr)
{
    u16 val = *(volatile u16 *)addr;
    asm volatile("sync" ::: "memory");
    return val;
}

void *memset(void *s, int c, u32 n);
int memcmp(const void *cs, const void *ct, unsigned int count);
void * memcpy(void *dest, const void *src, unsigned int count);

int sprintf(char *buf, const char *fmt, ...);
u32 strlen(const char * s);

void flush_cache_all(void);

void udelay(unsigned long usec);
void _machine_restart(int poweroff, unsigned int signature);
#ifdef DOWNSTREAM_BOOT
void downstream_boot(void);
#endif
void recovery_boot(void);

#endif