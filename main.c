/* This is a simple binary that runs as kernel just so it can
 *  help me test shit to migrate from JZ downstream drivers
 */


typedef unsigned char u8;
typedef unsigned int u32;

#define BIT(n) (1 << (n))

#define TCU_BASE     0x10002000

#define WDT_TCSR    0xc
#define TCU_TSCR    0x3c
#define WDT_TCNT    0x8
#define WDT_TDR     0x0
#define WDT_TCER			0x4

#define TCER_TCEN BIT(0)
#define TCSR_PRESCALE (3 << 3)

#define TSCR_WDTSC BIT(16)
#define TCSR_RTC_EN BIT(1)

#define SLCDC_CTRL 0xa4
#define SLCDC_CTRL_DMA_MODE	(1 << 2)
#define SLCDC_CTRL_DMA_EN	(1 << 0)


/* Don't ask why some values gets written to KSEG0 and
 * KSEG1 addresses, just do what jz wants
 */
#define DESC_PHYS       0x08600000
#define DESC_VIRT       0xa8600000
#define FB_PHYS         0x08400000
#define FB_VIRT         0xa8400000
#define LCDC_BASE       0xb3050000
#define LCDC_DA0	    0x40

// YEAH ITS SMALL, SO WHAT? >:<
#define XRES            360
#define YRES            360

/* Note that id, offsize, page_width are not actually
 * used, but removing them changes the size of the 
 * struct and just breaks the entire thing soo,
 * just do what jz wants
 */
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


// Soo, I actually dont know if this works. Edit: it does :)
static inline void writel(u32 val, u32 addr)
{
    *(volatile u32 *)addr = val;
    asm volatile("sync" ::: "memory");
}

static inline u32 readl(u32 addr)
{
    u32 val = *(volatile u32 *)addr;
    asm volatile("sync" ::: "memory");
    return val;
}

void *memset(void *s, int c, u32 n)
{
    u8 *p = (u8 *)s;
    while(n--) *p++ = (u8)c;
    return s;
}

// I kanged ts from somewhere, but I don't think it's accurate 
void udelay(u32 us)
{
    volatile u32 i = us * 500;
    while(i--);
}

// catfish made this <3
void _machine_restart(void)
{
    int time = 32768 / 64 * 4 / 1000;

    if (time > 65535)
        time = 65535;

    writel(TSCR_WDTSC, TCU_BASE + TCU_TSCR);
    writel(0, TCU_BASE + WDT_TCNT);
    writel(time, TCU_BASE + WDT_TDR);
    writel(TCSR_PRESCALE | TCSR_RTC_EN, TCU_BASE + WDT_TCSR);
    writel(0, TCU_BASE + WDT_TCER);
    writel(TCER_TCEN, TCU_BASE + WDT_TCER);
}


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


    /* You know that I'm restarted because I commented out this 
     * line accidentally and spent 10 mins trying to figure it out
     */
    writel(DESC_PHYS + LCDC_DA0, LCDC_BASE + LCDC_DA0);


    /* Setup continuous DMA refresh by clearing the
     * SLCDC_CTRL_DMA_MODE bit and setting SLCDC_CTRL_DMA_EN
     */

    int smart_ctrl = readl(LCDC_BASE + SLCDC_CTRL);
    smart_ctrl &= ~SLCDC_CTRL_DMA_MODE;  
    smart_ctrl |= SLCDC_CTRL_DMA_EN; 
    writel(smart_ctrl, LCDC_BASE + SLCDC_CTRL);

}

void framebuffer_test(void)
{
    int count = 5;
    int next_frm = 0;
    
    // White flash test
    while (count--) {

        if (next_frm)
        memset((void *)FB_VIRT, 0xff, 0x1fb000);
        else
        memset((void *)FB_VIRT, 0x00, 0x1fb000);
        
        next_frm = !next_frm;
        
        // Look ma! No manual frame updates!
        
        udelay(100000);
    }
    
    udelay(10000);
    
    /* It's dirty, it's disgusting, but colours are fun :) */
    
    u32 *fb = (u32 *)FB_VIRT;
    int i;

    // Horizontal bars
    for (i = 0; i < XRES * 90; i++) 
    {
        *fb++ = 0x00FF0000; /* red */
    }
    for (i = 0; i < XRES * 90; i++) 
    {
        *fb++ = 0x0000FF00; /* green */
    }
    for (i = 0; i < XRES * 90; i++) 
    {
        *fb++ = 0x000000FF; /* blue */
    }
    for (i = 0; i < XRES * 90; i++) 
    {
        *fb++ = 0xFFFFFFFF; /* white */
    }

    udelay(100000);
}


void main(void)
{
    // Init framebuffer
    early_lcdc_init();

    // Simple framebuffer test    
    framebuffer_test();

    // Goodbye!
    _machine_restart();


    // What the fuck are you doing here???????
    while(1);
}