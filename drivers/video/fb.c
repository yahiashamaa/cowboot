#include "io.h"
#include "lib/string.h"
#include "drivers/timer.h"
#include "drivers/lcdc.h"
#include "lib/font.h"
#include "common/fb.h"
#include "config/config.h"


int cur_x = 50;
int cur_y = 50;

void clear_fb(void)
{
    memset((void *)FB_VIRT, 0x00, 0x1fb000);
}


void draw_pixel(int x, int y, u32 color)
{
    u32 *fb = (u32 *)FB_VIRT;
    fb[y * XRES + x] = color;
}

void draw_char(int x, int y, unsigned char ch, u32 color, u32 bg)
{
    int idx = font_index(ch);
    if (idx < 0 || idx >= 256) 
        return;
    
    /* This might be retarded but I think it's better than drawing rect + string 
     * 10000000, 010000000, 00100000, 00010000, 00001000, 00000100, 00000010, 00000001
     */
    int mask[8] = {0x80, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01}; 

    for (int row = 0; row < FONTH; row++)     // move DOWN
    {
        u8 bits = letters[idx][row];

        for (int col = 0; col < FONTW; col++)  // move RIGHT
        {
            if (bits & mask[col]) 
                draw_pixel(x + col, y + row, color); // ON so draw text color
            else
                draw_pixel(x + col, y + row, bg); // OFF so draw bg color
        }
    }
}

void draw_string(int x, int y, const char *s, u32 textcolor, u32 bgcolor)
{
    int start_x = x;
    while (*s) {
        switch (*s)
        {
            // Forgot to check for this :>
            case '\n':
                y += FONTH;
                x = start_x;
                s++;
                break;
            default:
                draw_char(x, y, *s++, textcolor, bgcolor);
                x += FONTW;
                break;
        }
        
    }
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
    
    /* It's dirty, it's disgusting, but colours! :) */
    
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

    /* Clear the framebuffer incase we want to write to it later */
    clear_fb();
}

void copy_uboot_logo(void)
{
    // Copy U-Boot logo to our framebuffer
    u32 *src = (u32 *)UBOOT_FB_VIRT;
    u32 *dst = (u32 *)FB_VIRT;
    for (int i = 0; i < (XRES * YRES); i++)
        dst[i] = src[i];
}