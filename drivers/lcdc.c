#include "common/io.h"
#include "drivers/lcdc.h"
#include "config/config.h"

void early_lcdc_init(void)
{
    // Thankfully, we run as kernel. so bootloader (U-Boot) has already done some heavy lifting
    
    
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

    desc[0].cpos = LCDC_CPOS_BPP_18_24 |
               LCDC_CPOS_PREMULTI   |
               LCDC_CPOS_COEF_SLE_1;

    desc[0].desc_size = (0xff << LCDC_DESSIZE_ALPHA_BIT) |
                    ((YRES - 1) << LCDC_DESSIZE_HEIGHT_BIT) |
                    ((XRES - 1) << LCDC_DESSIZE_WIDTH_BIT);

    desc[0].cmd = LCDC_CMD_EOFINT |
              LCDC_CMD_FRM_EN |
              0x1fa40;

    desc[2].cmd = LCDC_CMD_CMD | LCDC_CMD_FRM_EN;

    writel(DESC_PHYS + LCDC_DA0, LCDC_BASE + LCDC_DA0);

    /* Setup continuous DMA refresh by clearing the
     * SLCDC_CTRL_DMA_MODE bit and setting SLCDC_CTRL_DMA_EN
     */
    int smart_ctrl = readl(LCDC_BASE + SLCDC_CTRL);

    smart_ctrl &= ~SLCDC_CTRL_DMA_MODE;  
    smart_ctrl |= SLCDC_CTRL_DMA_EN; 
    
    writel(smart_ctrl, LCDC_BASE + SLCDC_CTRL);
}
