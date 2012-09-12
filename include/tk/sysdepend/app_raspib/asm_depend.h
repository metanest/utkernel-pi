/*
 *----------------------------------------------------------------------
 *    micro T-Kernel
 *
 *    Copyright (C) 2006-2011 by Ken Sakamura. All rights reserved.
 *    micro T-Kernel is distributed under the T-License 2.0.
 *----------------------------------------------------------------------
 *
 *    Version:   1.01.01
 *    Released by T-Engine Forum(http://www.t-engine.org) at 2011/12/12.
 *
 *----------------------------------------------------------------------
 */

/*
 *	@(#)asm_depend.h (tk/AT91)
 *
 *	Assembler Macro for AT91M55800A
 */

#ifndef __TK_ASM_DEPEND_H__
#define __TK_ASM_DEPEND_H__

/* ------------------------------------------------------------------------ */
/*
 *	Exception restore
 */

/*
 * Exception return
 */
 .macro EXC_RETURN
	.arm
	ldmfd	sp!, {ip}
	msr	spsr_fsxc, ip
	ldmfd	sp!, {ip, pc}^
 .endm

/* ------------------------------------------------------------------------ */
/*
 *	tk_ret_int() system call
 */

/*
 * Return from handler by tk_ret_int()
 *	mode	Handler exception mode (Available on all modes including FIQ)
 *
 *	Use from SVC mode.
 *
 *	State of handler exception mode stack at Macro call
 *		+---------------+
 *	sp  ->	|R3		|
 *		|SPSR		|
 *		|R12=ip		|
 *		|R14=lr		|
 *		+---------------+
 */
 .macro TK_RET_INT_FIQ mode
	.arm
	mov	r3, lr				// r3 = lr_svc
	msr	cpsr_c, #PSR_I|PSR_F|\mode	// Return to previous exception mode
	swp	r3, r3, [sp]			// Save lr_svc and restore r3
	swi	SWI_RETINT
 .endm

/* ------------------------------------------------------------------------ */
#endif /* __TK_ASM_DEPEND_H__ */
