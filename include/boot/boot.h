#ifndef BOOT_H_
#define BOOT_H_

void boot_jz_image(u8 *bootimg, const char *cmd);
u8 *mmc_load_bootimg(u32 lba);
void reboot_recovery(void);

#endif