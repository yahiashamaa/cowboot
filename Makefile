GCCFLAGS = -Wall -O2 -ffreestanding -fpic -mabicalls -pie -mxgot -G0 -march=mips32r2

all: clean main.bin

BOARD ?= qogir

ifeq ($(BOARD),)
$(error No board specified)
endif

ifneq ($(APPEND),)
$(info appended downstream boot)

ifeq ($(wildcard downstream.img),)
$(error downstream.img not found)
endif

GCCFLAGS += -DDOWNSTREAM_BOOT

else
$(info no appended downstream boot)
endif


CFILES = \
    $(wildcard common/*.c) \
    $(wildcard drivers/*.c) \
    $(wildcard board/$(BOARD)/*.c)
GCCFLAGS += -DBOARD_$(BOARD)
OFILES = $(CFILES:.c=.o)


start.o: start.S
	mipsel-linux-gnu-gcc $(GCCFLAGS) -I include -c start.S -o start.o


%.o: %.c
	mipsel-linux-gnu-gcc $(GCCFLAGS) -I include -c $< -o $@

main.bin: start.o $(OFILES)
	mipsel-linux-gnu-gcc $(GCCFLAGS) -T link.lds start.o $(OFILES) -o main.elf
	mipsel-linux-gnu-objcopy -O binary main.elf main.bin
	~/nokia/mkbootimg/mkbootimg.py --kernel main.bin \
	--base 0x10000000 \
	--kernel_offset 0x00008000 \
	--ramdisk_offset 0x01000000 \
	--tags_offset 0x00000100 \
	--pagesize 2048 \
	--cmdline "" --output cowboot.img

clean:
	/bin/rm main.elf $(OFILES) start.o *.bin cowboot.img > /dev/null 2> /dev/null || true
