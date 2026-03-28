OUTPUT_ARCH(mips)
ENTRY(_start)
SECTIONS
{
	. = 0x80F00000;

	.text : {
        src/start.o(.text)
        *(.text*)
        *(.rodata*)
    }

    .data : {
        *(.data*)
    }

    . = ALIGN(4);

    __bss_start = .;

    .bss : {
        *(.bss*)
        *(COMMON)
    }

    __bss_end = .;

    . = ALIGN(4);
	__image_copy_end = .;

    /* This shit took me 30 mins to debug, why the fuck would 
	 * build id shit come before my actual instructs???
	 */
	/DISCARD/ : {
    	*(.note*)
	}
}