#include "common/io.h"
#include "common/fb.h"
#include "drivers/timer.h"
#include "drivers/gpio.h"
#include "boot/boot.h"
#include "boot/bootmenu.h"
#include "config/config.h"


void draw_menu(int selection)
{   
    // clear_fb();
    for (int i = 0; i < board_boot_options_count; i++) {
        if (i == selection)
            draw_string(120, 180 + (i * 20), board_boot_options[i].title, 0x00000000, 0xFFFFFF);
        else
            draw_string(120, 180 + (i * 20), board_boot_options[i].title, 0xFFFFFF, 0x00000000);
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

// TODO: add more events other than key press
void cycle_menu(int selection, int key_gpio)
{
    draw_logo(60, 0xFF00FF00);
    draw_menu(selection);
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
                board_boot_options[selection].command();
            }
        } 
        else
        {
            if (was_pressed && held < 30) /* short press */
            {  
                selection = (selection + 1) % board_boot_options_count;
                draw_menu(selection);
            }
            was_pressed = 0;
            held = 0;
        }
    }
}