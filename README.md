## What is it?
It is a simple secondary bootloader for Ingenic SoCs.

## Supported devices
The only supported SoC for now is the Ingenic M200.

All tests are done on the the Amazfit Verge (`qogir`),  
though other devices with the M200 SoC should also work fine.

## What does it do?
Currently, `cowboot` can:
- write to the framebuffer, display a boot menu
- reset and poweroff the device
- boot an appended downstream boot.img
- boot a boot.img from mmc

## How to build?
To compile **normally**, use: 
```
make all
```
which will produce the bootable `cowboot.img`.

To compile with the support of booting an appended `downstream.img`, use:
```
make all APPEND=1
```

To clean, use
```
make clean
```
Simple enough, no?
