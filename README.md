## What is it?
Cowboot is a simple secondary bootloader for Ingenic SoCs.

## Supported SoCs
- Ingenic M200(s)

## Supported boards
- **Amazfit Verge (`qogir`)** — tested and working

> Other devices with the M200 SoC should work just fine.

## What does it do?
Currently, `cowboot` can:
- Write to the framebuffer, display a boot menu
- Reset and poweroff the device
- Boot an appended boot.img
- Boot a boot.img from mmc

## Building
To compile **normally**, use: 

```bash
make all BOARD=$(board)
```

which will produce the bootable `cowboot.img`.

To compile with the support of booting an appended `downstream.img`, use:

```bash
make all BOARD=$(board) APPEND=1
```

To clean, use
```bash
make clean
```
Simple enough, no?
