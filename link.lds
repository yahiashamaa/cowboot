OUTPUT_ARCH(mips)
ENTRY(_start)
SECTIONS
{
	. = 0x80F00000;

	. = ALIGN(4);
	.text : {
		start.o(.text)
		*(.text*)
	}

	. = ALIGN(4);
	.rodata : {
		*(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*)))
	}

	. = ALIGN(4);
	.data : {
		*(.data*)
	}

	. = .;
	_gp = ALIGN(16) + 0x7ff0;

	. = ALIGN(4);
	.sdata : {
		*(.sdata*)
	}
    /* This shit took me 30 mins to debug, why the fuck would 
	 * build id shit come before my actual instructs???
	 */
	/DISCARD/ : {
    	*(.note*)
	}
}