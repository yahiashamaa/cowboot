/*
 * Copyright (C) 2007 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once
#include <stdint.h>
#include "common/types.h"
#define BOOT_MAGIC           "ANDROID!"
#define BOOT_MAGIC_SIZE      8
#define BOOT_NAME_SIZE       16
#define BOOT_ARGS_SIZE       512
#define BOOT_EXTRA_ARGS_SIZE 1024

/* When the boot image header has a version of 2, the structure of the boot
 * image is as follows:
 *
 * +---------------------+
 * | boot header         | 1 page
 * +---------------------+
 * | kernel              | n pages
 * +---------------------+
 * | ramdisk             | m pages
 * +---------------------+
 * | second stage        | o pages
 * +---------------------+
 * | recovery dtbo/acpio | p pages
 * +---------------------+
 * | dtb                 | q pages
 * +---------------------+
 * n = (kernel_size + page_size - 1) / page_size
 * m = (ramdisk_size + page_size - 1) / page_size
 * o = (second_size + page_size - 1) / page_size
 * p = (recovery_dtbo_size + page_size - 1) / page_size
 * q = (dtb_size + page_size - 1) / page_size
 *
 * 0. all entities are page_size aligned in flash
 * 1. kernel, ramdisk and DTB are required (size != 0)
 * 2. recovery_dtbo/recovery_acpio is required for recovery.img in non-A/B
 *    devices(recovery_dtbo_size != 0)
 * 3. second is optional (second_size == 0 -> no second)
 * 4. load each element (kernel, ramdisk, second, dtb) at
 *    the specified physical address (kernel_addr, etc)
 * 5. If booting to recovery mode in a non-A/B device, extract recovery
 *    dtbo/acpio and apply the correct set of overlays on the base device tree
 *    depending on the hardware/product revision.
 * 6. set up registers for kernel entry as required by your architecture
 * 7. if second_size != 0: jump to second_addr
 *    else: jump to kernel_addr
 */
struct boot_img_hdr_v2 {
    u8 magic[BOOT_MAGIC_SIZE];
    u32 kernel_size;
    u32 kernel_addr;
    
    u32 ramdisk_size;
    u32 ramdisk_addr;
    
    u32 second_size;
    u32 second_addr;
    
    u32 tags_addr;
    
    u32 page_size;
    
    u32 header_version;
    u32 os_version;
    u8 name[BOOT_NAME_SIZE];
    
    u8 cmdline[BOOT_ARGS_SIZE];
    u32 id[8];
    u8 extra_cmdline[BOOT_EXTRA_ARGS_SIZE];

    u32 recovery_dtbo_size;
    u64 recovery_dtbo_offset;
    
    u32 header_size;

    u32 dtb_size;
    u64 dtb_addr;
} __attribute__((packed));
