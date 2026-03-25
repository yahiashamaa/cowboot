#include "inc/regs.h"
#include "inc/io.h"
#include "inc/lcdc.h"

// Reference: 
/*
[    6.894318] framedesc[0] phys:       0x28223000
[    6.894323] framedesc[0].next:       0x08223040
[    6.894328] framedesc[0].databuf:    0x08400000
[    6.894334] framedesc[0].id:         0x00da0da0
[    6.894338] framedesc[0].cmd:        0x4401fa40
[    6.894344] framedesc[0].offsize:    0x00000000
[    6.894348] framedesc[0].page_width: 0x00000000
[    6.894354] framedesc[0].cpos:       0x2d000000
[    6.894359] framedesc[0].desc_size:  0xff167167
[    6.894364] framedesc[1] phys:       0x28223020
[    6.894369] framedesc[1].next:       0x08223000
[    6.894374] framedesc[1].databuf:    0x00000000
[    6.894378] framedesc[1].id:         0x00da0da1
[    6.894384] framedesc[1].cmd:        0x24000000
[    6.894388] framedesc[1].offsize:    0x00000000
[    6.894393] framedesc[1].page_width: 0x00000000
[    6.894398] framedesc[1].cpos:       0x00000000
[    6.894402] framedesc[1].desc_size:  0x00000000
[    6.894408] framedesc[2] phys:       0x28223040
[    6.894412] framedesc[2].next:       0x08223000
[    6.894418] framedesc[2].databuf:    0x08224000
[    6.894422] framedesc[2].id:         0x00da0da2
[    6.894428] framedesc[2].cmd:        0x24000000
[    6.894432] framedesc[2].offsize:    0x00000000
[    6.894437] framedesc[2].page_width: 0x00000000
[    6.894442] framedesc[2].cpos:       0x00000000
[    6.894447] framedesc[2].desc_size:  0x00000000
*/


/*
 * Note that most of these ^^ are not actually used, I
 * just kept wacking them one by one till it stopped
 * working and left those who are needed
 */


/* jzfb_config_smart_lcd_dma + jzfb_prepare_dma_desc
 * in jz driver, this is essentially 
 * the same shit, but just cleaner
 */

void early_lcdc_init(void)
{
    // Thankfully, we run as kernel. so bootloader (U-Boot) has already done some heavy lifting
    
    // Copy U-Boot logo to our framebuffer
    u32 *src = (u32 *)UBOOT_FB_VIRT;
    u32 *dst = (u32 *)FB_VIRT;
    for (int i = 0; i < (XRES * YRES); i++)
        dst[i] = src[i];
    
    struct framedesc *desc = (struct framedesc *)DESC_VIRT;

    /* With this memset, we actually zero out desc[0], desc[1]
     * and desc[2], but desc[1] is never used, however, not 
     * zeroing it out made the framebuffer turn BLUE instead 
     * of white sooooo lets just do what jz wants
     */
    memset(desc, 0, sizeof(struct framedesc) * 3);

    desc[0].next       = DESC_PHYS + LCDC_DA0;
    desc[2].next       = DESC_PHYS;

    desc[0].databuf    = FB_PHYS;

    // Idk why and I'm too lazy to check how this was set in jz driver
    desc[0].cpos       = 0x2d000000;
    desc[0].desc_size  = 0xff167167;
    desc[0].cmd        = 0x4401fa40;
    desc[2].cmd        = 0x24000000;

    writel(DESC_PHYS + LCDC_DA0, LCDC_BASE + LCDC_DA0);

    /* Setup continuous DMA refresh by clearing the
     * SLCDC_CTRL_DMA_MODE bit and setting SLCDC_CTRL_DMA_EN
     */
    int smart_ctrl = readl(LCDC_BASE + SLCDC_CTRL);
    smart_ctrl &= ~SLCDC_CTRL_DMA_MODE;  
    smart_ctrl |= SLCDC_CTRL_DMA_EN; 
    writel(smart_ctrl, LCDC_BASE + SLCDC_CTRL);
}
