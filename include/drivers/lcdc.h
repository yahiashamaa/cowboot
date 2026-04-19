#ifndef LCDC_H_
#define LCDC_H_

#include "common/types.h"

#define SLCDC_CTRL 0xa4
#define SLCDC_CTRL_DMA_MODE	(1 << 2)
#define SLCDC_CTRL_DMA_EN	(1 << 0)

#define DESC_PHYS       0x01800000   
#define DESC_VIRT       0xa1800000  

#define FB_PHYS         0x08400000
#define FB_VIRT         0xa8400000
#define UBOOT_FB_VIRT   0xaf980000

#define LCDC_BASE       0xb3050000
#define LCDC_DA0	    0x40


#define LCDC_CMD_EOFINT		(1 << 30)	/* Enable end of frame interrupt */
#define LCDC_CMD_CMD		(1 << 29)	/* indicate command in slcd mode */
#define LCDC_CMD_FRM_EN		(1 << 26)	/* Indicate this frame is enable */

#define LCDC_CPOS_BPP_BIT	27	/* Bits Per Pixel of OSD channel 1 (cannot use palette) */
#define LCDC_CPOS_BPP_MASK	(0x07 << LCDC_CPOS_BPP_BIT)
#define LCDC_CPOS_BPP_16	(4 << LCDC_CPOS_BPP_BIT)	/* 15/16 bpp */
#define LCDC_CPOS_BPP_18_24	(5 << LCDC_CPOS_BPP_BIT)	/* 18/24/32 bpp */
#define LCDC_CPOS_BPP_CMPS_24	(6 << LCDC_CPOS_BPP_BIT)	/* 24 compress bpp */
#define LCDC_CPOS_BPP_30	(7 << LCDC_CPOS_BPP_BIT)	/* 30 bpp */

#define LCDC_CPOS_PREMULTI	(1 << 26)	/* Premulti enable of foreground 0,1 */
#define LCDC_CPOS_COEF_SLE_BIT	24	/* Select coefficient for foreground 0,1 */
#define LCDC_CPOS_COEF_SLE_MASK	(0x3 << LCDC_CPOS_COEF_SLE_BIT)
#define LCDC_CPOS_COEF_SLE_0	(0 << LCDC_CPOS_COEF_SLE_BIT)	/* 00:0 */
#define LCDC_CPOS_COEF_SLE_1	(1 << LCDC_CPOS_COEF_SLE_BIT)	/* 01:1 */
#define LCDC_CPOS_COEF_SLE_2	(2 << LCDC_CPOS_COEF_SLE_BIT)	/* 10:alpha1 */
#define LCDC_CPOS_COEF_SLE_3	(3 << LCDC_CPOS_COEF_SLE_BIT)	/* 11:1-alpha1 */

/* Foreground 0,1 Size Register */
#define LCDC_DESSIZE_ALPHA_BIT	24	/*  The global alpha value of foreground 0,1 */
#define LCDC_DESSIZE_ALPHA_MASK	(0xff << LCDC_DESSIZE_ALPHA_BIT)
#define LCDC_DESSIZE_HEIGHT_BIT	12	/* height of foreground 1 */
#define LCDC_DESSIZE_HEIGHT_MASK	(0xfff << LCDC_DESSIZE_HEIGHT_BIT)
#define LCDC_DESSIZE_WIDTH_BIT	0	/* width of foreground 1 */
#define LCDC_DESSIZE_WIDTH_MASK	(0xfff << LCDC_DESSIZE_WIDTH_BIT)

struct framedesc {
    u32 next;
    u32 databuf;
    u32 id;
    u32 cmd;
    u32 offsize;
    u32 page_width;
    u32 cpos;
    u32 desc_size;
};

void early_lcdc_init(void);

#endif // LCDC_H_