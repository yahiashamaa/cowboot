#include "./include/cache.h" 

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