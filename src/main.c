#include "inc/regs.h"
#include "inc/io.h"
#include "inc/fb.h"
#include "inc/gpio.h"
#include "inc/timer.h"
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
/* jz downstream boot.img */
#define CONFIG_KERNEL_ENTRY    0x80f00000
#define CONFIG_PARAM_BASE      0x80000800
#define CONFIG_RAMDISK_DST     0x81a00000

void _machine_restart(int poweroff, unsigned int signature)
{
    /* poweroff = 1 to poweroff, poweroff = 0 to reboot */
    if (poweroff == 0)
    {
        writel(0x5a5a, CPM_BASE + CPM_CPSPPR);
        /* 0 signature = normal, 1 = recovery*/
        if (signature != 0)
            writel(RECOVERY_SIGNATURE, CPM_BASE + CPM_CPSPPR);
        else 
            writel(REBOOT_SIGNATURE, CPM_BASE + CPM_CPSPPR);

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
    draw_logo(245, 0xFF00FF00);
    relocate_code(0x80400000, 0, 0x80500000);

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

#ifdef DOWNSTREAM_BOOT
extern char downstream_start[];
extern char downstream_end[];

void downstream_boot(void)
{
    struct jz_boot_img_hdr *hdr = (struct jz_boot_img_hdr *)downstream_start;
    unsigned long *params = (unsigned long*)CONFIG_PARAM_BASE;
	char *cmd_dst = (char *)CONFIG_PARAM_BASE + 32;
    int page_sz, kernel_actual;

    /* check for ANDROID! magic */
    if (memcmp(hdr->magic, BOOT_MAGIC, BOOT_MAGIC_SIZE))
        while (1);

    page_sz = hdr->page_size;
    kernel_actual = (hdr->kernel_size + page_sz -1 ) & ~(page_sz -1);
    memcpy((void*)CONFIG_KERNEL_ENTRY, (char*)downstream_start + page_sz, hdr->kernel_size);
    #if 0
    if (hdr->ramdisk_size > 0) {
        memcpy((void*)CONFIG_RAMDISK_DST, 
        (char*)downstream_start + page_sz + kernel_actual, hdr->ramdisk_size);
    }
    #endif

    memset(params, 0, 128);
    params[0] = 0;
    params[5] = (unsigned long)cmd_dst;
    params[6] = (unsigned long)downstream_start;
    sprintf(cmd_dst, "%s rd_start=0x%x rd_size=0x%x", 
        (char*)hdr->cmdline, CONFIG_RAMDISK_DST, hdr->ramdisk_size);

    flush_cache_all();
    ((void (*)(int, char**, char *))CONFIG_KERNEL_ENTRY)(2, (char **)(params + 4), (char *)params);
}
#endif