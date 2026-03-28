#ifndef TIMER_H_
#define TIMER_H_

/* TCU/OST */

#define TCU_BASE            0x10002000

#define TCU_TSCR            0x3c
#define TCU_TESR			0x14
#define TCU_OSTDR			0xe0
#define TCU_OSTCNTL			0xe4
#define TCU_OSTCNTH			0xe8
#define TCU_OSTCSR			0xec
#define TCU_OSTCNTHBUF			0xfc

#define TER_OSTEN			(1 << 15)
#define OST_DIV				4

#define OSTCSR_CNT_MD			(1 << 15)
#define OSTCSR_SD			(1 << 9)
#define OSTCSR_PRESCALE		(1 << 3)


#define OSTCSR_EXT_EN			(1 << 2)

/* WDT */

#define WDT_TCSR    0xc
#define TCU_TSCR    0x3c
#define WDT_TCNT    0x8
#define WDT_TDR     0x0
#define WDT_TCER	0x4
#define TCER_TCEN BIT(0)
#define TCSR_PRESCALE (3 << 3)
#define TSCR_WDTSC BIT(16)
#define TCSR_RTC_EN BIT(1)

void reset_timer(void);
int timer_init(void);
void udelay(unsigned long usec);

#endif