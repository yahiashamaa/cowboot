#ifndef QOGIR_H_
#define QOGIR_H_

#define XRES            360
#define YRES            360
#define KEY_GPIO        30
#define BOARD_CMDLINE \
    "console=ttyS3,115200n8 mem=245M@0x0 mem=255M@0x30000000 ip=off " \
    "root=/dev/ram0 rw rdinit=/init rd_start=0x81A00000 rd_size=0x00166F09 " \
    "androidboot.revision=00000000 androidboot.fastboot_unlock=1 " \
    "androidboot.battery_type=1 "

#define RECOVERY_LBA    24576
#define BOOT_LBA        90112

#define AUTOBOOT        1

#endif // QOGIR_H_