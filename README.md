## What is it?
It is a simple secondary bootloader that runs on the Ingenic M200 SoC after U-Boot.

## What does it do?
Currently, it can:
  - set up and write to framebuffer from the semi-working state handed to us by the primary bootloader
  - trigger a watchdog "reset".
  - read gpio values
  - display a simple boot menu
  - jump back to the primary bootloader
  

## How to use?
To compile, use: 
```
make all
```
which will produce `cowboot.img` that can be booted on the device.

To clean, use
```
make clean
```
Simple enough, no?
