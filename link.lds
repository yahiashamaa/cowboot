OUTPUT_ARCH(mips)
ENTRY(_start)
SECTIONS
{
	. = 0x80F00000;

	.text : {
        start.o(.text)
        *(.text*)
    }

    .data : {
        *(.data*)
    }

    /* This shit took me 30 mins to debug, why the fuck would 
	 * build id shit come before my actual instructs???
	 */
	/DISCARD/ : {
    	*(.note*)
	}
}