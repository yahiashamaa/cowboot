CFILES = $(wildcard *.c)
OFILES = $(CFILES:.c=.o)
GCCFLAGS = -Wall -O2 -ffreestanding -nostdinc -nostdlib -mno-abicalls -fno-pic


all: clean main.bin


start.o: start.S
	mipsel-linux-gnu-gcc $(GCCFLAGS) -c start.S -o start.o


%.o: %.c
	mipsel-linux-gnu-gcc $(GCCFLAGS) -c $< -o $@

main.bin: start.o $(OFILES)
	mipsel-linux-gnu-gcc $(GCCFLAGS) -nostdlib -mxgot -G 0 -T link.lds start.o $(OFILES) -o main.elf
	mipsel-linux-gnu-objcopy -O binary main.elf main.bin


clean:
	/bin/rm main.elf *.o *.bin > /dev/null 2> /dev/null || true