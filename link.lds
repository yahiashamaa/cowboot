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

    . = .;
	_gp = ALIGN(16) + 0x7ff0;

	.got : {
		*(.got)
	}
    num_got_entries = SIZEOF(.got) >> 2;


    . = ALIGN(4);

    __bss_start = .;

    .bss : {
        *(.bss*)
        *(COMMON)
    }

    __bss_end = .;

    . = ALIGN(4);
	__image_copy_end = .;

    /* This shit took me 30 mins to debug hehe */

	/DISCARD/ : {
    	*(.note*)
	}
}