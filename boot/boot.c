#include "io.h"
#include "cache.h"
#include "mem.h"
#include "common/types.h"
#include "common/fb.h"
#include "common/console.h"
#include "lib/nanoprintf.h"
#include "lib/string.h"
#include "lib/libfdt.h"
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

    u32 page_sz    = hdr->page_size;
    char final_cmdline[1024];

    u32 kernel_actual  = ALIGN(hdr->kernel_size, page_sz);
    u32 ramdisk_start = JZ_KERNEL_ADDR  + ALIGN(hdr->kernel_size, 0x100000);

    u8 *kernel_src  = bootimg + page_sz;
    u8 *ramdisk_src = bootimg + page_sz + kernel_actual;

    npf_snprintf(final_cmdline, sizeof(final_cmdline), "%s root=/dev/ram0 rd_start=0x%x rd_size=0x%x ", cmd, ramdisk_start, hdr->ramdisk_size);

    // Copy kernel
    memcpy((void *)JZ_KERNEL_ADDR, kernel_src, hdr->kernel_size);

    asm volatile("sync" ::: "memory");
    
    memcpy((u8 *)ramdisk_start, ramdisk_src, hdr->ramdisk_size);
    
    asm volatile("sync" ::: "memory");


    u8 *param_base = (u8 *)(ramdisk_start  + ALIGN(hdr->kernel_size, 8));
    memset(param_base, 0, 256);
    
    u32 *param = (u32 *)param_base;
    
    param[5] = (u32)(param_base + 32);
    param[6] = JZ_KERNEL_ADDR;
    char *cmdline = (char *)(param_base + 32);
    for (u32 i = 0; i <= strlen(final_cmdline); i++)
        cmdline[i] = final_cmdline[i];


    flush_cache_all();
    asm volatile("sync" ::: "memory");

    ((void (*)(int, char **, char *))JZ_KERNEL_ADDR)(2,
        (char **)(param_base + 16),
        (char *)param_base);
}

u8 *mmc_load_bootimg(u32 lba)
{
    u8 buf[8192]; 
    u32 sectors;

    /* should be a fallback for both header versions */
    struct boot_img_hdr_v2 *hdr = (struct boot_img_hdr_v2 *)buf;

    // Read header only first hehe
    mmc_read_data(lba, 8, buf); 
    u32 page_sz    = hdr->page_size;
    
    if (memcmp(hdr->magic, BOOT_MAGIC, BOOT_MAGIC_SIZE))
    {
        video_printf("ERROR: invalid boot.img magic");
        return NULL;
    }

    if (hdr->header_version == 2)
    {   
        u32 kernel_offs = page_sz;
        u32 ramdisk_offs = kernel_offs + ALIGN(hdr->kernel_size, page_sz);
        u32 second_offs = ramdisk_offs + ALIGN(hdr->ramdisk_size, page_sz);
        u32 recovery_dtbo_offs = second_offs + ALIGN(hdr->second_size, page_sz);
        u32 dtb_offs = recovery_dtbo_offs + ALIGN(hdr->recovery_dtbo_size, page_sz); 
        u32 total = dtb_offs + ALIGN(hdr->dtb_size, page_sz); 
        sectors = ALIGN(total, 512) / 512;

    }
    else
    {
        u32 kernel_actual  = ALIGN(hdr->kernel_size, page_sz);
        u32 ramdisk_actual = ALIGN(hdr->ramdisk_size, page_sz);

        u32 total = page_sz + kernel_actual + ramdisk_actual;
        sectors =  ALIGN(total, 512) / 512;
    }

    // Read full image
    u8 *bootimg = malloc(sectors * 512);
    mmc_read_data(lba, sectors, bootimg);

    return bootimg;
}

#define KERNEL_ADDR 0x80100000
#define KERNEL_ENTRY  (KERNEL_ADDR + 0x400)

void boot_v2_bootimg(u32 bootimg_addr) {
    struct boot_img_hdr_v2 *hdr = (struct boot_img_hdr_v2 *)bootimg_addr;
    int chosen;
    void *fdt;
    u32 page_sz, ramdisk_start, dtb_start;
   
    if (hdr->header_version != 2)
        video_printf("ERROR: boot.img hdr ver is not 2");

    page_sz = hdr->page_size;

    /*
    1. header
    2. kernel
    3. ramdisk
    4. second - unused for mainline
    5. recovery dtbo - unused for mainline
    6. dtb
    */

    u32 kernel_offs = page_sz;
    u32 ramdisk_offs = kernel_offs + ALIGN(hdr->kernel_size, page_sz);
    u32 second_offs = ramdisk_offs + ALIGN(hdr->ramdisk_size, page_sz);
    u32 recovery_dtbo_offs = second_offs + ALIGN(hdr->second_size, page_sz);
    u32 dtb_offs = recovery_dtbo_offs + ALIGN(hdr->recovery_dtbo_size, page_sz);


    ramdisk_start = KERNEL_ADDR  + ALIGN(hdr->kernel_size, 0x100000); 
    dtb_start = ramdisk_start + ALIGN(hdr->ramdisk_size, 8);

    memcpy((void *)KERNEL_ADDR, (void *)bootimg_addr + kernel_offs, hdr->kernel_size);
    memcpy((void *)ramdisk_start, (void *)bootimg_addr + ramdisk_offs, hdr->ramdisk_size);
    memcpy((void *)dtb_start, (void *)bootimg_addr + dtb_offs, hdr->dtb_size);

    flush_cache_all();
    asm volatile("sync" ::: "memory");

    fdt = (void *)dtb_start;
    if (fdt_check_header(fdt))
        video_printf("ERROR: invalid image fdt");

    if (fdt_open_into(fdt, fdt, fdt_totalsize(fdt) + 1024))
        video_printf("fdt_open_into failed");
    

    chosen = fdt_path_offset(fdt, "/chosen");
    if (chosen < 0)
        chosen = fdt_add_subnode(fdt, 0, "chosen");    


#ifndef IGNORE_BOOTIMG_CMD

    const char *bootimg_cmdline = (const char *)hdr->cmdline;
    char final_cmdline[1024];

    const char *bootargs = fdt_getprop(fdt, chosen, "bootargs", NULL);
    
    npf_snprintf(final_cmdline, sizeof(final_cmdline), "%s %s", bootargs, bootimg_cmdline);

    fdt_setprop_string(fdt, chosen, "bootargs", final_cmdline);

#endif

    fdt_setprop_u32(fdt, chosen, "linux,initrd-start", virt_to_phys(ramdisk_start));
    fdt_setprop_u32(fdt, chosen, "linux,initrd-end",   virt_to_phys(ramdisk_start + hdr->ramdisk_size));

    flush_cache_all();
    asm volatile("sync" ::: "memory");

    ((void (*)(int, void *, int))KERNEL_ENTRY)(-2, fdt, 0);
}


void reboot_recovery(void)
{
    _machine_restart(0, 1);
}
