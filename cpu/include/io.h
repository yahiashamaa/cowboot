#ifndef IO_H_
#define IO_H_

#include "common/types.h"

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

#define virt_to_phys(addr) ((u32)(addr) & 0x1fffffff)

#define __ALIGN_MASK(x, mask)	(((x) + (mask)) & ~(mask))
#define ALIGN(x, a)		__ALIGN_MASK((x), (typeof(x))(a) - 1)

#define KSEG0ADDR(a)		(virt_to_phys(a) | KSEG0)
#define KSEG1ADDR(a)		(virt_to_phys(a) | KSEG1)

#endif // IO_H_