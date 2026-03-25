#include "inc/regdef.h"

	.set noreorder

	.globl _start
	.text
_start:
	/* Set up temporary stack */
	li	sp, 0x80400000

	la  t9, early_init
	jr  t9
	 nop


	.globl	relocate_code
	.ent	relocate_code
relocate_code:
	move	sp, a0			# set new stack pointer


	li	t0, 0x80F00000  # source (our textbase)
	move	t1, a2 		# destination (new textbase)
	la  t2, __image_copy_end # end of source

	sub t6, a2, t0 # relocation offset

	
# t0 = source address
# t1 = target address
# t2 = source end address

copy:
	lw	t3, 0(t0)
	sw	t3, 0(t1)
	addu	t0, 4
	blt	t0, t2, copy
	 addu	t1, 4


#  flush caches and invalidate icache here
move	t0,	a2	# Our new base

flush_dcache:
	cache	0x01, 0(t0)
	bne	t0, t1, flush_dcache
	 addi	t0, 32	# CONFIG_SYS_CACHELINE_SIZE

	sync

/* flush i-cache */
move	t0, a2	# Our new base

flush_icache:
	cache	0x00, 0(t0)
	bne	t0, t1, flush_icache
	 addi	t0, 32	# CONFIG_SYS_CACHELINE_SIZE



	la  t9, main
	addu t9, t6
	jr   t9
 	 move	a1, a2
	
	
	.end	relocate_code