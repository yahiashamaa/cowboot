#include "inc/regs.h"
#include "inc/fb.h"
#include "inc/io.h"
#include "inc/gpio.h"
#include "inc/bootmenu.h"

int gpio_get_value(u32 gpio)
{
	u32 gpio_regs = GPIO_BASE;
	int port = gpio / 32;
	int pin = gpio % 32;

	return readl(gpio_regs + GPIO_PXPIN(port)) & BIT(pin);
}