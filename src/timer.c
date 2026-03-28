#include "inc/types.h"
#include "inc/io.h"
#include "inc/timer.h"


unsigned int multiple __attribute__ ((section(".data")));

void reset_timer(void)
{
	writel(0, TCU_BASE + TCU_OSTCNTH);
	writel(0, TCU_BASE + TCU_OSTCNTL);
	writel(0, TCU_BASE + TCU_OSTDR);
}

int timer_init(void)
{

	multiple = 24 / OST_DIV;

	reset_timer();
	writel(OSTCSR_CNT_MD | OSTCSR_PRESCALE | OSTCSR_EXT_EN, TCU_BASE + TCU_OSTCSR);
	writew(TER_OSTEN, TCU_BASE + TCU_TESR);

	return 0;
}



static u64 get_timer64(void)
{
	u32 low = readl(TCU_BASE + TCU_OSTCNTL);
	u32 high = readl(TCU_BASE + TCU_OSTCNTHBUF);
	return ((u64)high << 32) | low;
}

void udelay(unsigned long usec)
{
	/* OST count increments at 3MHz */
	u64 end = get_timer64() + ((u64)usec * multiple);
	while (get_timer64() < end);
}