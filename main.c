/* This is a simple binary that runs as kernel just so it can
 *  help me test shit to migrate from JZ downstream drivers
 */

#include "regs.h"
#include "io.h"
#include "fb.h"
#include "gpio.h"
#include "lcdc.h"
#include "bootmenu.h"

// catfish made this <3
void _machine_restart(void)
{
    int time = 32768 / 64 * 4 / 1000;

    if (time > 65535)
        time = 65535;

    writel(TSCR_WDTSC, TCU_BASE + TCU_TSCR);
    writel(0, TCU_BASE + WDT_TCNT);
    writel(time, TCU_BASE + WDT_TDR);
    writel(TCSR_PRESCALE | TCSR_RTC_EN, TCU_BASE + WDT_TCSR);
    writel(0, TCU_BASE + WDT_TCER);
    writel(TCER_TCEN, TCU_BASE + WDT_TCER);
}

void main(void)
{
    // Init framebuffer
    early_lcdc_init();

    // We can draw logo here on top of the uboot logo
    // draw_logo(245, 0xFF00FF00);
    

    // Pretty self explainatory, no?
    clear_fb();   
    
    // Boot menu 
    cycle_menu(0, 30);
   
    // What the fuck are you doing here???????
    while(1);
}