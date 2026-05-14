#include "io.h"
#include "cache.h"
#include "common/types.h"
#include "common/fb.h"
#include "common/console.h"
#include "lib/nanoprintf.h"
#include "lib/string.h"
#include "drivers/timer.h"
#include "drivers/lcdc.h"
#include "drivers/mmc.h"
#include "boot/jz_bootimg.h"
#include "boot/v2bootimg.h"
#include "boot/boot.h"
#include "boot/bootmenu.h"
#include "config/config.h"

void boot_jz_image(u8 *bootimg, const char *cmd)
{
    struct jz_boot_img_hdr *hdr = (struct jz_boot_img_hdr *)bootimg;

    u32 page_size    = hdr->page_size;
    u32 page_mask    = page_size - 1;

    u32 kernel_actual  = (hdr->kernel_size  + page_mask) & ~page_mask;

    u8 *kernel_src  = bootimg + page_size;
    u8 *ramdisk_src = bootimg + page_size + kernel_actual;

    // Copy kernel
    memcpy((void *)KERNEL_ADDR, kernel_src, hdr->kernel_size);

    asm volatile("sync" ::: "memory");
    
    memcpy((u8 *)RAMDISK_ADDR, ramdisk_src, hdr->ramdisk_size);
    
    asm volatile("sync" ::: "memory");

    u32 *param = (u32 *)PARAM_BASE;
    memset(param, 0, 256);
    
    param[5] = PARAM_BASE + 32;
    param[6] = KERNEL_ADDR;
    
    char *cmdline = (char *)(PARAM_BASE + 32);
    for (u32 i = 0; i <= strlen(cmd); i++)
        cmdline[i] = cmd[i];


    flush_cache_all();
    asm volatile("sync" ::: "memory");

    ((void (*)(int, char **, char *))0x80F00000)(2,
        (char **)(PARAM_BASE + 16),
        (char *)PARAM_BASE);
}

u8 *mmc_load_bootimg(u32 lba)
{
    u8 *bootimg = (u8 *)0xA5000000;
    struct jz_boot_img_hdr *hdr = (struct jz_boot_img_hdr *)bootimg;

    // Read header only first hehe
    mmc_read_data(lba, 8, bootimg); 

    u32 page_size    = hdr->page_size;
    u32 page_mask    = page_size - 1;

    u32 kernel_actual  = (hdr->kernel_size  + page_mask) & ~page_mask;
    u32 ramdisk_actual = (hdr->ramdisk_size + page_mask) & ~page_mask;

    u32 total = page_size + kernel_actual + ramdisk_actual;
    u32 sectors = (total + 511) / 512;

    // Read full image
    mmc_read_data(lba, sectors, bootimg);

    return bootimg;
}

void reboot_recovery(void)
{
    _machine_restart(0, 1);
}
