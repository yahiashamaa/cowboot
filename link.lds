OUTPUT_ARCH(mips)
ENTRY(_start)
SECTIONS
{
	. = 0x80F00000;

	.text : {
        start.o(.text)
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
	__image_copy_end = .;

	.rel.dyn : {
		__rel_dyn_start = .;
        *(.rel.dyn)
        *(.rel.data.rel.ro*)
        *(.rel.got*)
        __rel_dyn_end = .;
	}

    .dynsym : {
		*(.dynsym)
	}

	.bss __rel_dyn_start (OVERLAY) : {
		__bss_start = .;
		*(.sbss*)
		*(.bss*)
		*(COMMON)
		. = ALIGN(4);
		__bss_end = .;
	}

    /* This shit took me 30 mins to debug hehe */

	/DISCARD/ : {
        *(.dynbss)
		*(.dynstr)
		*(.dynamic)
    	*(.note*)
	}
}