CFILES = $(wildcard src/*.c)
OFILES = $(CFILES:.c=.o)
GCCFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -mno-abicalls -fno-pic -march=mips32r2


all: clean main.bin


src/start.o: src/start.S
	mipsel-linux-gnu-gcc $(GCCFLAGS) -c src/start.S -o src/start.o


%.o: %.c
	mipsel-linux-gnu-gcc $(GCCFLAGS) -c $< -o $@

main.bin: src/start.o $(OFILES)
	mipsel-linux-gnu-gcc $(GCCFLAGS) -nostdlib -mxgot -G 0 -T link.lds src/start.o $(OFILES) -o main.elf
	mipsel-linux-gnu-objcopy -O binary main.elf main.bin
	mkbootimg --kernel main.bin \
	--base 0x10000000 \
	--kernel_offset 0x00008000 \
	--ramdisk_offset 0x01000000 \
	--tags_offset 0x00000100 \
	--pagesize 2048 \
	--cmdline "" --output cowboot.img

clean:
	/bin/rm main.elf *.o *.bin *.img > /dev/null 2> /dev/null || true