## What is it?
It is a simple binary that runs as kernel on the Ingenic M200 SoC to help in hardware bringup.

## What does it do?
Currently, it only sets up and writes to framebuffer from the semi-working state handed to us by the bootloader, then triggers a watchdog reset.

## How to use?
To compile, use: 
```
make CROSS_COMPILE=mipsel-linux-gnu- all
```
To clean, use
```
make CROSS_COMPILE=mipsel-linux-gnu- clean
```
Simple enough, no?
