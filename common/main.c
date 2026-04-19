#include "common/io.h"
#include "common/fb.h"
#include "drivers/gpio.h"
#include "drivers/timer.h"
#include "common/console.h"
#include "drivers/lcdc.h"
#include "boot/boot.h"
#include "boot/bootmenu.h"
#include "config/config.h"

#define NANOPRINTF_IMPLEMENTATION
#define NANOPRINTF_USE_FIELD_WIDTH_FORMAT_SPECIFIERS	1
#define NANOPRINTF_USE_PRECISION_FORMAT_SPECIFIERS	1
#define NANOPRINTF_USE_FLOAT_FORMAT_SPECIFIERS		0
#define NANOPRINTF_USE_SMALL_FORMAT_SPECIFIERS		1
#define NANOPRINTF_USE_LARGE_FORMAT_SPECIFIERS		0
#define NANOPRINTF_USE_BINARY_FORMAT_SPECIFIERS		1
#define NANOPRINTF_USE_WRITEBACK_FORMAT_SPECIFIERS	1
#define NANOPRINTF_USE_ALT_FORM_FLAG			    1
#include "common/nanoprintf.h"

#include "boot/jz_bootimg.h"

void relocate_code(unsigned int new_sp, void *gd, unsigned int dest);
void early_init(void)
{   

    board_early_init();

    early_lcdc_init();

#ifdef AUTOBOOT
    copy_uboot_logo();
    video_printf("Press key to interrupt autoboot");
    draw_logo(240, 0xFF00FF00);
#endif


    relocate_code(0x8FFFFF00, 0, 0x8F000000);

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