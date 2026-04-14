#include "inc/regs.h"
#include "inc/io.h"
#include "inc/fb.h"
#include "inc/gpio.h"
#include "inc/timer.h"
#include "inc/console.h"
#include "inc/lcdc.h"
#include "inc/bootmenu.h"

#define NANOPRINTF_IMPLEMENTATION
#define NANOPRINTF_USE_FIELD_WIDTH_FORMAT_SPECIFIERS	1
#define NANOPRINTF_USE_PRECISION_FORMAT_SPECIFIERS	1
#define NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS		0
#define NANOPRINTF_USE_SMALL_FORMAT_SPECIFIERS		1
#define NANOPRINTF_USE_LARGE_FORMAT_SPECIFIERS		0
#define NANOPRINTF_USE_BINARY_FORMAT_SPECIFIERS		1
#define NANOPRINTF_USE_WRITEBACK_FORMAT_SPECIFIERS	1
#define NANOPRINTF_USE_ALT_FORM_FLAG			    1
#include "inc/nanoprintf.h"

#include "inc/jz_bootimg.h"

#define WDT_TDR_TIME           32768 / 64 * 4 / 1000

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

void relocate_code(unsigned int new_sp, void *gd, unsigned int dest);
void early_init(void)
{   
    early_lcdc_init();

    // We can draw logo here on top of the uboot logo
    copy_uboot_logo();
    draw_logo(240, 0xFF00FF00);

    relocate_code(0x8FFFF000, 0, 0x8F000000);

    // What the fuck are you doing here????
    while(1);
}

void main(void)
{   
    early_lcdc_init();

    timer_init();

    // Boot menu
    clear_fb();
    cycle_menu(0, 30);
   
    // What the fuck are you doing here????
    while(1);
}

void recovery_boot(void)
{
    _machine_restart(0, 1);
}