#include "inc/io.h"
#include "inc/nanoprintf.h"

void *memset(void *s, int c, u32 n)
{
    u8 *p = (u8 *)s;
    while(n--) *p++ = (u8)c;
    return s;
}

int memcmp(const void *cs, const void *ct, unsigned int count)
{
	const unsigned char *su1, *su2;
	int res = 0;

	for( su1 = cs, su2 = ct; 0 < count; ++su1, ++su2, count--)
		if ((res = *su1 - *su2) != 0)
			break;
	return res;
}

void * memcpy(void *dest, const void *src, unsigned int count)
{
	unsigned long *dl = (unsigned long *)dest, *sl = (unsigned long *)src;
	char *d8, *s8;

	if (src == dest)
		return dest;

	/* while all data is aligned (common case), copy a word at a time */
	if ( (((unsigned long)dest | (unsigned long)src) & (sizeof(*dl) - 1)) == 0) {
		while (count >= sizeof(*dl)) {
			*dl++ = *sl++;
			count -= sizeof(*dl);
		}
	}
	/* copy the reset one byte at a time */
	d8 = (char *)dl;
	s8 = (char *)sl;
	while (count--)
		*d8++ = *s8++;

	return dest;
}

void flush_cache_all(void)
{

	u32 addr, t = 0;

	for (addr = CKSEG0; addr < CKSEG0 + 32768;
	     addr += 32) {
		cache_op(0, addr);
		cache_op(1, addr);
	}

	fast_iob();

	/* invalidate btb */
	__asm__ __volatile__(
		".set mips32\n\t"
		"mfc0 %0, $16, 7\n\t"
		"nop\n\t"
		"ori %0,2\n\t"
		"mtc0 %0, $16, 7\n\t"
		".set mips2\n\t"
		:
		: "r" (t));
}

int sprintf(char *buf, const char *fmt, ...)
{
	va_list args;
	int i;

	va_start(args, fmt);
	i = npf_vsnprintf(buf, 1024, fmt, args);
	va_end(args);
	return i;
}