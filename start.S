#include "regdef.h"

	.set noreorder

	.globl _start
	.text
_start:
	/* Initialize $gp */
	bal	1f
	 nop
	.word	_gp
1:
	lw	gp, 0(ra)

	/* Set up temporary stack */
	li	sp, 0x80400000

	la  t9, main
	jr  t9
	 nop

