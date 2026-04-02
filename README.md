## What is it?
It is a simple secondary bootloader that runs on the Ingenic M200 SoC after U-Boot.

## What does it do?
The only supported device for now is the Amazfit Verge (`qogir`).
Currently, `cowboot` can:
- write to the framebuffer, display a boot menu
- reset and poweroff of the device
- boot an appended downstream boot.img

## How to build?
To compile **normally**, use: 
```
make all
```
which will produce the bootable `cowboot.img`.

(UNTESTED) To compile with the support of booting an appended `downstream.img`, use:
```
make all APPEND=1
```

To clean, use
```
make clean
```
Simple enough, no?
