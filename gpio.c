#include "regs.h"
#include "fb.h"
#include "io.h"
#include "gpio.h"
#include "bootmenu.h"

int gpio_get_value(u32 gpio)
{
	u32 gpio_regs = GPIO_BASE;
	int port = gpio / 32;
	int pin = gpio % 32;

	return readl(gpio_regs + GPIO_PXPIN(port)) & BIT(pin);
}


// TODO: Really? just a button press?
int gpio_test(u32 gpio)
{
	int val = 0;
	int was_pressed = 0;
	int held = 0;
	clear_fb();
	udelay(50000);
	draw_string(110, 200, "Press any key....", 0xFFFFFF, 0x00);
	while (1)
	{
		val = gpio_get_value(gpio);
		if (!val)
		{
			clear_fb();
			draw_string(85, 200, "Key PRESSED, TEST PASSED!", 0xFF00FF00, 0x00);
			draw_string(85, 220, "Hold key to return to menu.", 0xFFFFFF, 0x00);
			break;

		}
	}
	while (1)
	{
		val = gpio_get_value(gpio);
		if (!val)
        {
            was_pressed = 1;
            held++;
            udelay(5000);
            if (held == 30)
            {  
				clear_fb();
                cycle_menu(1, gpio);
            }
        } 
        else
        {
            if (was_pressed && held < 30) /* short press */
            {  
               if (!val)
				{
					draw_string(110, 250, "ON", 0xFF00FF00, 0x00);
				}
				else
				{
					draw_string(110, 250, "OFF", 0xFF000000, 0x00);
				}
            }
            was_pressed = 0;
            held = 0;
        }
		
	}
}