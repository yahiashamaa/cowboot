#include "common/types.h"
#include "io.h"
#include "drivers/timer.h"


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

void _machine_restart(int poweroff, unsigned int signature)
{
    /* poweroff = 1 to poweroff, poweroff = 0 to reboot */
    if (poweroff == 0)
    {
        writel(0x5a5a, CPM_BASE + CPM_CPSPPR);
        /* 0 signature = normal, 1 = recovery*/
        if (signature != 0)
            writel(RECOVERY_SIGNATURE, CPM_BASE + CPM_CPPSR);
        else 
            writel(REBOOT_SIGNATURE, CPM_BASE + CPM_CPPSR);

        writel(0, CPM_BASE + CPM_CPSPPR);
    }

    writel(TSCR_WDTSC, TCU_BASE + TCU_TSCR);
    writel(0, TCU_BASE + WDT_TCNT);

    writel(WDT_TDR_TIME, TCU_BASE + WDT_TDR);
    writel(TCSR_PRESCALE | TCSR_RTC_EN, TCU_BASE + WDT_TCSR);
    writel(0, TCU_BASE + WDT_TCER);
    writel(TCER_TCEN, TCU_BASE + WDT_TCER);
}