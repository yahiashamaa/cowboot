#include "io.h"
#include "mem.h"
#include "common/fb.h"
#include "drivers/gpio.h"
#include "drivers/timer.h"
#include "common/console.h"
#include "drivers/lcdc.h"
#include "boot/boot.h"
#include "boot/bootmenu.h"
#include "config/config.h"

#include "lib/nanoprintf.h"
#include "lib/string.h"

#include "boot/jz_bootimg.h"

#define RELOCATE_ADDR 0x8F000000
#define RELOC_SP        (RELOCATE_ADDR + 0xFFFF00)
#define HEAP_SIZE 0x4000000

static void init_bootmem(void)
{
    u32 heap_end     = RELOCATE_ADDR - 1;
    u32 heap_start   = heap_end - HEAP_SIZE + 1;

    boot_alloc_init(heap_start, heap_end);
}

void relocate_code(unsigned int new_sp, void *gd, unsigned int dest);
void early_init(void)
{   

    init_bootmem();

    board_early_init();

    early_lcdc_init();

#ifdef AUTOBOOT
    copy_uboot_logo();
    video_printf("Press key to interrupt autoboot");
    draw_logo(240, 0xFF00FF00);
#endif

    relocate_code(RELOC_SP, 0, RELOCATE_ADDR);

    // What the fuck are you doing here????
    while(1);
}

void main(void)
{   

    timer_init();

    // Board init goes here
    board_late_init();

#ifdef AUTOBOOT
    for (int i = 0; i <= 3; i++) {
        if (!gpio_get_value(KEY_GPIO)) {
            /* wait for release */
            udelay(300);
            clear_fb();
            cycle_menu(0, KEY_GPIO);
            while (1);
        }
        udelay(1 * 1000 * 1000);
    }
    
    continue_boot();
#else
    cycle_menu(0, KEY_GPIO);
#endif

   
    // What the fuck are you doing here????
    while(1);
}