BOARD ?= qogir
CROSS_COMPILE ?= mipsel-linux-gnu-

CC      := $(CROSS_COMPILE)gcc
LD      := $(CROSS_COMPILE)gcc
OBJCOPY := $(CROSS_COMPILE)objcopy

CFLAGS = -Wall -O2 -ffreestanding -fpic -mabicalls -pie -mxgot -G0 -march=mips32r2
CFLAGS += -fno-builtin-malloc
CFLAGS += -Iinclude -Icpu/include


ifeq ($(BOARD),)
$(error No board specified)
endif

ifneq ($(APPEND),)
$(info Appended downstream boot)

ifeq ($(wildcard downstream.img),)
$(error downstream.img not found!)
endif
CFLAGS += -DDOWNSTREAM_BOOT
endif

CFLAGS += -DBOARD_$(BOARD)
OFILES = $(CFILES:.c=.o)

SUBDIRS := cpu boot lib init drivers board/$(BOARD)
include $(addsuffix /Makefile,$(SUBDIRS))

OBJS := start.o $(obj-y)

all: clean cowboot.img

start.o: cpu/start.S
	$(CC) $(CFLAGS) -I include -c $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -I include -c $< -o $@

main.elf: $(OBJS)
	$(CC) $(CFLAGS) -T cpu/link.lds $^ -o $@

main.bin: main.elf
	$(OBJCOPY) -O binary $< $@

cowboot.img: main.bin
	python3 scripts/mkbootimg.py --kernel main.bin \
	--base 0x10000000 \
	--kernel_offset 0x00008000 \
	--ramdisk_offset 0x01000000 \
	--tags_offset 0x00000100 \
	--pagesize 2048 \
	--cmdline "" --output cowboot.img

clean:
	rm -f $(OBJS)
	rm -f main.elf main.bin cowboot.img