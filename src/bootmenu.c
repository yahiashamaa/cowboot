#include "inc/regs.h"
#include "inc/io.h"
#include "inc/timer.h"
#include "inc/gpio.h"
#include "inc/fb.h"
#include "inc/jz_bootimg.h"
#include "inc/bootmenu.h"

const char *bootoptions[] = {
    #ifdef DOWNSTREAM_BOOT
    "Boot embedded image",
    #endif
    "Continue boot",
    "Boot from recovery",
    "Reset",
    "Reset to recovery",
    "Power Off",
};

#define MENU_COUNT (sizeof(bootoptions) / sizeof(bootoptions[0]))

void draw_menu(int selection)
{   
    // clear_fb();
    for (int i = 0; i < MENU_COUNT; i++) {
        if (i == selection)
            draw_string(120, 180 + (i * 20), bootoptions[i], 0x00000000, 0xFFFFFF);
        else
            draw_string(120, 180 + (i * 20), bootoptions[i], 0xFFFFFF, 0x00000000);
    }
}

const char *logo =
"   ___            ___           _   \n"
"  / __|_____ __ _| _ ) ___  ___| |_ \n"
" | (__/ _ \\ V  V / _ \\/ _ \\/ _ \\  _|\n"
"  \\___\\___/\\_/\\_/|___/\\___/\\___/\\__|\n"
"                                    \n";

/* For the logo above, its 36, if we ever change the logo, then we can uncomment and recalculate.
void get_logo_length(char *s)
{   
    int length = 0;
    int max_length = 0;
    for (int i = 0; s[i] != '\0'; i++)
    {
        if (s[i] == '\n')
        {
            if (length > max_length)
            {
                max_length = length;
            }
            length = 0;
        }
        else
        {
            length++;
        }
    }
}
*/
void draw_logo(int y, u32 color)
{
    /* (XRES - (calculated_logo_length * font_width)) /2*/
    int centered_x = (XRES - (36 * 8)) / 2;
    draw_string(centered_x, y, logo, color, 0);
}

// TODO: move to a boot.c and actually implement mmc boot
void (*uboot)(void)= (void (*)(void))0x80100000;

#ifdef DOWNSTREAM_BOOT
extern char downstream_start[];
extern char downstream_end[];
#endif

// TODO: add more events other than key press
void cycle_menu(int selection, int key_gpio)
{
    draw_logo(60, 0xFF00FF00);
    draw_menu(selection);
    u8 *bootimg;
    const char *cmdline =
    "console=ttyS3,115200n8 mem=245M@0x0 mem=255M@0x30000000 ip=off "
    "root=/dev/ram0 rw rdinit=/init rd_start=0x81A00000 rd_size=0x00166F09 "
    "androidboot.revision=00000000 androidboot.fastboot_unlock=1 "
    "androidboot.battery_type=1";
    int held = 0;
    int was_pressed = 0;

    while (1) {
        if (!gpio_get_value(key_gpio))
        {
            was_pressed = 1;
            held++;
            udelay(10000);
            if (held == 30)
            { 
            #ifdef DOWNSTREAM_BOOT
                switch (selection) {
                    case 0: // downstream boot
                        boot_jz_image((u8 *)downstream_start, cmdline);
                        break;
                    case 1: // continue boot
                        bootimg = mmc_load_bootimg(90112);
                        boot_jz_image(bootimg, cmdline);
                        break;
                    case 2: // boot from recovery partition
                        bootimg = mmc_load_bootimg(24576);
                        boot_jz_image(bootimg, cmdline);
                        break;
                    case 3: // reset
                        _machine_restart(0, 0);
                        break;
                    case 4: // reset u-boot recovery
                        recovery_boot();
                        break;
                    case 5: // poweroff
                        _machine_restart(1, 0);
                        break;
                }
            }

            #else
                switch (selection) {
                    case 0: // continue boot
                        bootimg = mmc_load_bootimg(90112);
                        boot_jz_image(bootimg, cmdline);
                        break;
                    case 1: // boot from recovery partition
                        bootimg = mmc_load_bootimg(24576);
                        boot_jz_image(bootimg, cmdline);
                        break;
                    case 2: // reset
                        _machine_restart(0, 0);
                        break;
                    case 3: // reset u-boot recovery
                        recovery_boot();
                        break;
                    case 4: // poweroff
                        _machine_restart(1, 0);
                        break;
                }
        }
            #endif
        } 
        else
        {
            if (was_pressed && held < 30) /* short press */
            {  
                selection = (selection + 1) % MENU_COUNT;
                draw_menu(selection);
            }
            was_pressed = 0;
            held = 0;
        }
    }
}