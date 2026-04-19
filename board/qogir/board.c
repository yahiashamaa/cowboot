#include "common/types.h"
#include "drivers/timer.h"
#include "boot/boot.h"
#include "common/console.h"
#include "boot/bootmenu.h"
#include "config/config.h"


// So far we do not need any board specific init for qogir
int board_early_init(void)
{
    return 0;
}

int board_late_init(void)
{   
    return 0;
}

#ifdef DOWNSTREAM_BOOT
extern char downstream_start[];
extern char downstream_end[];

void embed_boot(void)
{
    boot_jz_image((u8 *)downstream_start, BOARD_CMDLINE);
}

#endif

void continue_boot(void)
{
    u8 *bootimg = mmc_load_bootimg(BOOT_LBA);
    boot_jz_image(bootimg, BOARD_CMDLINE);
}

static void boot_recovery(void)
{
    u8 *bootimg = mmc_load_bootimg(RECOVERY_LBA);
    boot_jz_image(bootimg, BOARD_CMDLINE);
}

static void machine_reset(void)
{ 
    _machine_restart(0, 0); 
}

static void machine_poweroff(void)
{ 
    _machine_restart(1, 0);
}

const bootmenu_entry board_boot_options[] = {
#ifdef DOWNSTREAM_BOOT
    { "Boot embedded image", embed_boot },
#endif
    { "Continue boot",       continue_boot },
    { "Boot from recovery",  boot_recovery },
    { "Reset",               machine_reset },
    { "Reset to recovery",   reboot_recovery },
    { "Power Off",           machine_poweroff },
};

const int board_boot_options_count = sizeof(board_boot_options) / sizeof(board_boot_options[0]);
