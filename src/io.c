#include "inc/io.h"

void *memset(void *s, int c, u32 n)
{
    u8 *p = (u8 *)s;
    while(n--) *p++ = (u8)c;
    return s;
}

// not accurate
void udelay(u32 us)
{
    volatile u32 i = us * 500;
    while(i--);
}
