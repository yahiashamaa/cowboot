#ifndef UTILS_H
#define UTILS_H

#include "types.h"

// Soo, I actually dont know if this works. Edit: it does :)
static inline void writel(u32 val, u32 addr)
{
    *(volatile u32 *)addr = val;
    asm volatile("sync" ::: "memory");
}

static inline u32 readl(u32 addr)
{
    u32 val = *(volatile u32 *)addr;
    asm volatile("sync" ::: "memory");
    return val;
}

void *memset(void *s, int c, u32 n);
void udelay(u32 us);

#endif