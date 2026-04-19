#ifndef GPIO_H
#define GPIO_H

#define	GPIO_BASE	0xb0010000

#define GPIO_PXPIN(n)	(0x00 + (n) * 0x100)
int gpio_get_value(u32 gpio);

#endif