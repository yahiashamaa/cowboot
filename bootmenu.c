/* Now realistically, this is where the test part ends, We dont  
 * need a boot menu, we can't even really boot into anything, 
 * the only useful thing we can get out of this, is to use
 * it as a shim and deal with quirks before jumping to linux 
 * or uboot, but I'm bored and can't find anything else to do
 */
#include "regs.h"
#include "io.h"
#include "gpio.h"
#include "fb.h"
#include "bootmenu.h"

const char *bootoptions[] = {
    "FB Test",
    "GPIO Test",
    "Jump to U-Boot",
    "Power Off",
};

#define MENU_COUNT 4

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

// TODO: add more events other than key press
void cycle_menu(int selection, int key_gpio)
{
    draw_logo(60, 0xFF00FF00);
    draw_menu(selection);
    int held = 0;
    int was_pressed = 0;

    /* YOU CANNOT IMAGINE HOW LONG IT TOOK ME TO FIGURE OUT THIS SHIT */
    while (1) {
        if (!gpio_get_value(key_gpio))
        {
            was_pressed = 1;
            held++;
            udelay(5000);
            if (held == 30)
            {  
                switch (selection) {
                    case 0:
                        framebuffer_test();
                        draw_logo(60, 0xFF00FF00);
                        draw_menu(0);
                        break;
                    case 1:
                        gpio_test(key_gpio); 
                        break;
                    case 2: 
                        (*uboot)();
                        break;
                    case 3:
                        _machine_restart();
                        break;
                }
            }
        } 
        else
        {
            if (was_pressed && held < 30) /* short press */
            {  
                selection = (selection + 1) % MENU_COUNT; // pls work plsplsplspls. Edit: it works :>
                draw_menu(selection);
            }
            was_pressed = 0;
            held = 0;
        }
    }
}
