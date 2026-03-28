#include "inc/regdef.h"

	.set noreorder

	.globl _start
	.text
_start:
	li	sp, 0x80400000

	la  t9, main
	jr  t9
	 nop


	.globl	relocate_code
	.ent	relocate_code
relocate_code:
	move	sp, a0			# set new stack pointer


	li	t0, 0x80F00000  # source (our textbase)
	move	t1, a2 		# destination (new textbase)
	la  t2, __image_copy_end # end of source

	sub	t3, t2, t0		# length
	add	t4, t1, t3		# dst end
	sub	t5, a2, t0		# relocation offset

copy:
	lw	t6, 0(t0)
	sw	t6, 0(t1)
	addu	t0, 4
	blt	t0, t2, copy
	 addu	t1, 4


	# flush dcache
	li	t0, 0x80000000  		# start
	li t1, 0x80000000 + 32768

flush_dcache:
	cache	0x01, 0(t0)
	addi	t0, t0, 32	# CONFIG_SYS_CACHELINE_SIZE
	 blt	t0, t1, flush_dcache
	  nop
	sync

	# flush icache
	li	t0, 0x80000000
	

flush_icache:
	cache	0x00, 0(t0)
	addi	t0, t0, 32
	blt	t0, t1, flush_icache
	 nop

	mfc0	t0, CP0_CONFIG, 7
	nop
	ori	t0, 2
	mtc0	t0, CP0_CONFIG, 7
	nop


	# clear bss
	la	t0, __bss_start
	la	t1, __bss_end

clear_bss:
	sw	zero, 0(t0)
	addi	t0, t0, 4
	blt	t0, t1, clear_bss
	 nop


	# jump
	la	t9, main
	addu	t9, t5
	jr	t9
	 nop

	.end relocate_code
