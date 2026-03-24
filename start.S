#include "regdef.h"

	.set noreorder

	.globl _start
	.text
_start:
	/* Set up temporary stack */
	li	sp, 0x80400000

	la  t9, main
	jr  t9
	 nop

