#include "types.h"

/* Don't ask why some values gets written to KSEG0 and
 * KSEG1 addresses, just do what jz wants
 */


/* GPIO */

#define	GPIO_BASE	0xb0010000
#define GPIO_PXPIN(n)	(0x00 + (n) * 0x100)


/* Panel */

#define SLCDC_CTRL 0xa4
#define SLCDC_CTRL_DMA_MODE	(1 << 2)
#define SLCDC_CTRL_DMA_EN	(1 << 0)

#define DESC_PHYS       0x08600000
#define DESC_VIRT       0xa8600000
#define FB_PHYS         0x08400000
#define FB_VIRT         0xa8400000
#define UBOOT_FB_VIRT   0xaf980000
#define LCDC_BASE       0xb3050000
#define LCDC_DA0	    0x40

// YEAH ITS SMALL, SO WHAT? >:<
#define XRES            360
#define YRES            360

