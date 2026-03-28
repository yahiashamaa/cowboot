#include "inc/regs.h"
#include "inc/io.h"
#include "inc/fb.h"
#include "inc/gpio.h"
#include "inc/timer.h"
#include "inc/lcdc.h"
#include "inc/bootmenu.h"

void _machine_restart(void)
{
    int time = 32768 / 64 * 4 / 1000;
    writel(TSCR_WDTSC, TCU_BASE + TCU_TSCR);
    writel(0, TCU_BASE + WDT_TCNT);
    writel(time, TCU_BASE + WDT_TDR);
    writel(TCSR_PRESCALE | TCSR_RTC_EN, TCU_BASE + WDT_TCSR);
    writel(0, TCU_BASE + WDT_TCER);
    writel(TCER_TCEN, TCU_BASE + WDT_TCER);
}

void power_off(void)
{
    /* TODO */
}

void relocate_code(unsigned int new_sp, void *gd, unsigned int dest);
void early_init(void)
{   
    early_lcdc_init();

    // We can draw logo here on top of the uboot logo
    draw_logo(245, 0xFF00FF00);
    relocate_code(0x80400000, 0, 0x81000000);

    // What the fuck are you doing here????
    while(1);
}

void main(void)
{   
    early_lcdc_init();

    // Boot menu
    clear_fb();
    cycle_menu(0, 30);
   
    // What the fuck are you doing here????
    while(1);
}