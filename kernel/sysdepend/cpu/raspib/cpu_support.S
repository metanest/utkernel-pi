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
 *	cpu_support.S (AT91)
 *	Device-Dependent CPU Operation
 */

#define	_in_asm_source_

#include <machine.h>
#include <tk/errno.h>
#include <tk/sysdef.h>
#include <tk/asm.h>
#include <sys/sysinfo.h>

#include "config.h"
#include "utk_config.h"
#if USE_TRAP | (USE_DBGSPT & USE_HOOK_TRACE)
#include "isysconf.h"
#endif
#include "tkdev_conf.h"
#include "offset.h"

/* ------------------------------------------------------------------------ */
/*
 * Dispatcher
 *	dispatch_to_schedtsk:
 *		Throw away the current contexts and forcibly dispatch
 *		to 'schedtsk.'
 *		Called directly by jump (bx) but do not return.
 *		Called on the undefined stack state (undefined 'ssp').
 *		Called on the interrupt disable state.
 *	dispatch_entry:
 *		Normal dispatch processing.
 *	_ret_int_dispatch:
 *		Called when dispatch is required by 'tk_ret_int().'
 *
 *	Contexts to save
 *	Save registers except for ssp(R13_svc) to a stack. Save 'ssp' to TCB.
 *
 *		+---------------+
 *	ssp ->	| R0 - R11	|
 *		| taskmode	|
 *		| R13_usr = usp	| Available only for RNG 1-3
 *		| R14_usr	|
 *		+---------------+
 *		| R14_svc	| R14_svc before interrupt
 *		|		| (Available only for tk_ret_int)
 *		+---------------+			-
 *		| SPSR_svc	|			| Save by interrupt 
 *		| R12     = ip	|			| entry routine
 *		| R14_svc = lr	| Return address (pc)	|
 *		+---------------+			-
 */


	.text
	.balign	4
	.globl	Csym(knl_dispatch_to_schedtsk)
	.globl	Csym(knl_dispatch_entry)

Csym(knl_dispatch_to_schedtsk):
	/* Interrupt is disabled(CPSR.I=1 F=1),during SVC mode */
	ldr	sp, =(Csym(knl_tmp_stack) + TMP_STACK_SZ)	// Set temporal stack

	ldr	ip, =Csym(knl_dispatch_disabled)
	ldr	r0, =1
	str	r0, [ip]			// Dispatch disable

	ldr	r4, =Csym(knl_ctxtsk)		// R4 = &ctxtsk
	ldr	r0, =0
#if USE_DBGSPT
	ldr	r8, [r4]
#endif
	str	r0, [r4]			// ctxtsk = NULL

	msr	cpsr_c, #PSR_SVC		// Interrupt enable
	b	l_dispatch0

Csym(knl_dispatch_entry):
	/* Interrupt is disabled(CPSR.I=1 F=1),during SVC mode */
#if USE_TRAP	/* Enable interrupt in previous CPSR on stack */
	ldr	ip, [sp]
	bic	ip, ip, #PSR_DI
	str	ip, [sp]			// SPSR_svc compensation
#else
	stmfd	sp!, {ip, lr}
	mrs	ip, cpsr
	bic	ip, ip, #PSR_DI
	stmfd	sp!, {ip}
#endif
	stmfd	sp!, {lr}			// Context save (R14_svc)
						// The contents are invalid due to number adjustment
_ret_int_dispatch:
	/* Interrupt is disabled(CPSR.I=1 F=1),during SVC mode */
	ldr	ip, =Csym(knl_dispatch_disabled)
	ldr	lr, =1
	str	lr, [ip]			// Dispatch disable

	msr	cpsr_c, #PSR_SVC		// Interrupt enable

	ldr	ip, =Csym(knl_taskmode)
	ldr	ip, [ip]
	sub	sp, sp, #15*4
	stmia	sp, {r0-r11, ip, sp, lr}^	// Context save 

	ldr	r4, =Csym(knl_ctxtsk)		// R4 = &ctxtsk
	ldr	r0, =0
	ldr	r8, [r4]
	str	sp, [r8, #TCB_tskctxb + CTXB_ssp] // Save 'ssp' to TCB
	str	r0, [r4]			// ctxtsk = NULL

  l_dispatch0:
	/* During interrupt enable CPSR.I=0 F=0 */
#if USE_DBGSPT & USE_HOOK_TRACE
	ldr	ip, =hook_stop_jmp		// Hook processing
	ldr	pc, [ip]
  ret_hook_stop:
#endif

	ldr	r5, =Csym(knl_schedtsk)		// R5 = &schedtsk
	ldr	r6, =Csym(knl_lowpow_discnt)	// R6 = &lowpow_discnt

  l_dispatch1:
	msr	cpsr_c, #PSR_SVC|PSR_DI		// Interrupt disable

	ldr	r8, [r5]			// R8 = schedtsk
	cmp	r8, #0				// Is there 'schedtsk'?
	bne	l_dispatch2

	/* Because there is no task that should be executed, move to the power-saving mode */
	ldr	ip, [r6]			// Is 'low_pow' disabled?
	cmp	ip, #0
	bleq	Csym(knl_low_pow)			// call low_pow()

	msr	cpsr_c, #PSR_SVC		// Interrupt enable
	b	l_dispatch1

  l_dispatch2:					// Switch to 'schedtsk'
	/* During interrupt disable CPSR.I=1 F=1 */
	str	r8, [r4]			// ctxtsk = schedtsk
	ldr	sp, [r8, #TCB_tskctxb + CTXB_ssp] // Restore 'ssp' from TCB

#if USE_DBGSPT & USE_HOOK_TRACE
	ldr	ip, =hook_exec_jmp		// Hook processing
	ldr	pc, [ip]
  ret_hook_exec:
#endif

	ldr	ip, =Csym(knl_dispatch_disabled)
	ldr	lr, =0
	str	lr, [ip]			// Dispatch enable 

	ldmia	sp, {r0-r11, ip, sp, lr}^	// Context restore 
	nop
	add	sp, sp, #15*4
	ldr	lr, =Csym(knl_taskmode)
	str	ip, [lr]

	ldmfd	sp!, {lr}			// R14_svc restore

	EXC_RETURN


#if USE_DBGSPT & USE_HOOK_TRACE
/*
 * Task dispatcher hook routine call
 *	void stop( ID tskid, INT lsid, UINT tskstat )
 *	void exec( ID tskid, INT lsid )
 */
	.text
	.balign	4
hook_stop:
	cmp	r8, #0			// r8 = ctxtsk
	beq	l_notask

	ldrb	r2, [r8, #TCB_state]			// tskstat
	mov	r2, r2, lsl #1
	ldr	r0, [r8, #TCB_tskid]			// tskid

	ldr	ip, =Csym(knl_hook_stopfn)
	ldr	ip, [ip]
	mov	lr, pc
	bx	ip			// call stop(tskid, lsid, tskstat)

  l_notask:
	b	ret_hook_stop

hook_exec:
					// r8 = ctxtsk
	ldr	r0, [r8, #TCB_tskid]			// tskid

	ldr	ip, =Csym(knl_hook_execfn)
	ldr	ip, [ip]
	mov	lr, pc
	bx	ip			// call exec(tskid, lsid)

	b	ret_hook_exec

/*
 * Set/Free task dispatcher hook routine
 */
	.text
	.balign	4
	.globl	Csym(knl_hook_dsp)
Csym(knl_hook_dsp):
	ldr	r0, =hook_exec_jmp
	ldr	r1, =hook_stop_jmp
	ldr	r2, =hook_exec
	ldr	r3, =hook_stop
	str	r2, [r0]
	str	r3, [r1]
	bx	lr

	.globl	Csym(knl_unhook_dsp)
Csym(knl_unhook_dsp):
	ldr	r0, =hook_exec_jmp
	ldr	r1, =hook_stop_jmp
	ldr	r2, =ret_hook_exec
	ldr	r3, =ret_hook_stop
	str	r2, [r0]
	str	r3, [r1]
	bx	lr

			.data
			.balign	4
  hook_exec_jmp:	.long	ret_hook_exec
  hook_stop_jmp:	.long	ret_hook_stop

#endif /* USE_DBGSPT & USE_HOOK_TRACE */

#if USE_HLL_INTHDR
/* ------------------------------------------------------------------------ */
/*
 * High level programming language routine for interrupt handler
 *	Called by interrupt entry routine on the state saved in the interrupt 
 *	stack as shown below.
 *		+---------------+
 *	ssp ->	| R3		| Only FIQ, IRQ
 *		+---------------+
 *	ssp ->	| SPSR		|
 *		| R12 = ip	|
 *		| R14 = lr	|
 *		+---------------+
 *
 *	The vector table address is set in 'ip.'
 *	(ip - EIT_VECTBL) / 4 = Vector number
 */
	.text
	.balign	4
	.globl	Csym(knl_inthdr_startup)
	.globl	Csym(knl_exchdr_startup)
Csym(knl_exchdr_startup):
	/* Unknown mode/During interrupt disable CPSR.I=1 F=? */
	stmfd	sp!, {r3}		// Register save
	b	l_inthdr1

Csym(knl_inthdr_startup):
	/* Unknown mode/During interrupt disable CPSR.I=1 F=? */
	mrs	lr, cpsr
	and	lr, lr, #PSR_M(31)
	cmp	lr, #PSR_SVC
	stmeqfd	sp!, {r3}		// If it is SWI, also save 'r3'
  l_inthdr1:
	stmfd	sp!, {r0-r2}		// Register save

	ldr	r3, =Csym(knl_intvec)
	sub	r3, ip, r3		// r3 = Vector table offset

					// Argument of handler
	mov	r0, r3, lsr #2		// r0 = dintno
	add	r1, sp, #4*4		// r1 = sp

  l_inthdr2:
	mrs	r2, cpsr		// r2 = CPSR save
#if USE_TRAP
	msr	cpsr_c, #PSR_SVC|PSR_DI	// Move to SVC mode/Interrupt disable

	stmfd	sp!, {r2, lr}		// SVC mode register save
	stmfd	sp!, {ip}		// In normal situation, save only for FIQ
#else
	msr	cpsr_c, #PSR_IRQ|PSR_DI	// Move to IRQ mode/Interrupt disable

	stmfd	sp!, {r2, lr}
	stmfd	sp!, {ip}
#endif

	ldr	ip, =Csym(knl_taskindp)		// Task independent part
	ldr	lr, [ip]
	add	lr, lr, #1
	str	lr, [ip]

#if USE_DBGSPT & USE_HOOK_TRACE
	ldr	ip, =hook_ienter_jmp
	ldr	pc, [ip]
  ret_hook_ienter:
#endif

	ldr	ip, =Csym(knl_hll_inthdr)
	ldr	ip, [ip, r3]
	mov	lr, pc
	bx	ip			// call hll_inthdr[n](dintno, sp)

#if USE_DBGSPT & USE_HOOK_TRACE
	ldr	ip, =hook_ileave_jmp
	ldr	pc, [ip]
  ret_hook_ileave:
#endif

#if USE_TRAP
	ldmfd	sp!, {ip}		// SVC mode register restore
	ldmfd	sp!, {r2, r3}		// r2 = Original mode
					// r3 = R14_svc
	orr	r2, r2, #PSR_DI
	msr	cpsr_c, r2		// Return to original mode/Interrupt disable
#else
	ldmfd	sp!, {ip}		// IRQ mode register restore
	ldmfd	sp!, {r2, r3}		// r2 = Original mode
					// r3 = R14_irq
	orr	r2, r2, #PSR_DI
	msr	cpsr_c, r2
#endif

	ldr	ip, =Csym(knl_taskindp)
	ldr	lr, [ip]
	sub	lr, lr, #1
	str	lr, [ip]

	ldmfd	sp!, {r0-r2}		// Register restore
	swp	r3, r3, [sp]		// r3 restore, R14_svc/irq save
#if USE_TRAP
	swi	SWI_RETINT
#else
	bl	tk_ret_int_impl
#endif


#if USE_DBGSPT & USE_HOOK_TRACE
/*
 * Interrupt handler hook routine call
 */
	.text
	.balign	4
hook_ienter:
	stmfd	sp!, {r0, r1}		// Register save
	stmfd	sp!, {r3}

	ldr	ip, =Csym(knl_hook_ienterfn)
	ldr	ip, [ip]
	mov	lr, pc
	bx	ip			// call enter(dintno, sp)

	ldmfd	sp!, {r3}		// Register restore
	ldmfd	sp, {r0, r1}		// Leave 'dintno,' 'sp' on stack
	b	ret_hook_ienter

hook_ileave:
	ldmfd	sp!, {r0, r1}		// Restore 'dintno,' 'sp' 

	ldr	ip, =Csym(knl_hook_ileavefn)
	ldr	ip, [ip]
	mov	lr, pc
	bx	ip			// call leave(dintno, info)

	b	ret_hook_ileave

/*
 * Set/Free interrupt handler hook routine
 */
	.text
	.balign	4
	.globl	Csym(knl_hook_int)
Csym(knl_hook_int):
	ldr	r0, =hook_ienter_jmp
	ldr	r1, =hook_ileave_jmp
	ldr	r2, =hook_ienter
	ldr	r3, =hook_ileave
	str	r2, [r0]
	str	r3, [r1]
	bx	lr

	.globl	Csym(knl_unhook_int)
Csym(knl_unhook_int):
	ldr	r0, =hook_ienter_jmp
	ldr	r1, =hook_ileave_jmp
	ldr	r2, =ret_hook_ienter
	ldr	r3, =ret_hook_ileave
	str	r2, [r0]
	str	r3, [r1]
	bx	lr

			.data
			.balign	4
  hook_ienter_jmp:	.long	ret_hook_ienter
  hook_ileave_jmp:	.long	ret_hook_ileave

#endif /* USE_DBGSPT & USE_HOOK_TRACE */
#endif /* USE_HLL_INTHDR */

/*
 * tk_ret_int_impl()
 *
 *	When called, the interrupt stack is configured as shown below.
 *		+---------------+
 *	ssp ->	| SPSR_svc	| Save by 'swi SWI_RETINT'
 *		| R12_usr	|
 *		| R14_svc	|
 *		+---------------+
 *
 *		+---------------+
 *	isp ->	| R14_svc	| Save when calling 'tk_ret_int'
 *		+---------------+
 *		| SPSR_xxx	| Save when the interrupt occurs
 *		| R12_xxx	|
 *		| R14_xxx	| <- Return address
 *		+---------------+
 */
	.text
	.balign	4
	.globl	Csym(tk_ret_int_impl)
Csym(tk_ret_int_impl):
#if USE_TRAP
	ldr	ip, [sp]		// ip = SPSR
#else
	mrs	ip, cpsr
#endif

	and	lr, ip, #PSR_M(31)
	cmp	lr, #PSR_SVC
	beq	l_retint_svc		// Is it 'tk_ret_int' from SWI?

	/* from IRQ/FIQ mode */
#if USE_TRAP
	stmfd	sp!, {r2, r3}		// Save 'r2' for work (Save 'r3' for acquiring location)
	add	r2, sp, #4          /* r2 -> r3 */
#else
	msr	cpsr, #PSR_SVC|PSR_DI
	// Save 'r2, lr' for work (Save 'r3-' for acquiring location)
	stmfd	sp!, {r4, r5, r6}
	stmfd	sp!, {lr}
	stmfd	sp!, {r2}
	add	r2, sp, #4          /* r2 -> lr_svc */
	msr	cpsr, ip
#endif

	orr	ip, ip, #PSR_DI
	bic	ip, ip, #PSR_T
	cmp	lr, #PSR_FIQ
	msr	cpsr_c, ip		// Move to interrupted mode/Interrupt disable
	/*  r2=ssp, sp=isp*/


#if USE_TRAP
	ldmfd	sp!, {ip, lr}		// Copy from 'isp' to 'ssp'/Free 'isp'
	str	ip, [r2, #0*4]		// R14_svc
	str	lr, [r2, #1*4]		// SPSR_xxx
	ldmfd	sp!, {ip, lr}
	strne	ip, [r2, #2*4]		// R12_xxx (except for FIQ)
	str	lr, [r2, #3*4]		// R14_xxx (Return address)
#else
	ldmfd	sp!, {ip, lr}		// Copy from 'isp' to 'ssp'/Free 'isp'
					// trash R14_irq
	str	lr, [r2, #1*4]		// SPSR_xxx
	ldmfd	sp!, {ip, lr}
	strne	ip, [r2, #2*4]		// R12_xxx (except for FIQ)
	str	lr, [r2, #3*4]		// R14_xxx (Return address)
#endif


	msr	cpsr_c, #PSR_SVC|PSR_DI	// Move to SVC mode/Interrupt disable

	ldmfd	sp!, {r2}		// r2 restore
	b	l_retint1

  l_retint_svc:
#if USE_TRAP
	add	sp, sp, #3*4		// Trash parts saved by 'swi SWI_RETINT'
#endif
	msr	cpsr_c, #PSR_SVC|PSR_DI	// Interrupt disable

  l_retint1:

	ldr	ip, =Csym(knl_taskindp)	// Is it a nesting interrupt?
	ldr	ip, [ip]
	cmp	ip, #0
	bne	l_nodispatch

	ldr	ip, =Csym(knl_dispatch_disabled)	// Is it during dispatch disable?
	ldr	ip, [ip]
	cmp	ip, #0
	bne	l_nodispatch

	ldr	ip, [sp, #4]		// SPSR
	tst	ip, #PSR_I|PSR_F	// Is it an exception during interrupt disable?
	bne	l_nodispatch

	ldr	ip, =Csym(knl_ctxtsk)	// Is dispatch required?
	ldr	lr, =Csym(knl_schedtsk)
	ldr	ip, [ip]
	ldr	lr, [lr]
	cmp	ip, lr
	bne	_ret_int_dispatch	// To dispatch processing

  l_nodispatch:
	ldmfd	sp!, {lr}		// lr restore 
	EXC_RETURN


/* ------------------------------------------------------------------------ */

#if USE_TRAP | USE_DBGSPT | (CFN_MAX_SSYID > 0)
/*
 * Unsupported system call
 */
	.text
	.balign	4
	.globl	Csym(knl_no_support)
Csym(knl_no_support):
	ldr	r0, =E_RSFN
	bx	lr

/*
 * System call entry table
 */
#if USE_TRAP | (USE_DBGSPT & USE_HOOK_TRACE)
	.text
	.balign	4
_svctbl:
	.int	Csym(knl_no_support)
#define	tk_ret_int_impl	knl_no_support
#include <sys/svc/tksvctbl.h>
#undef	tk_ret_int_impl
#endif /* USE_TRAP */

/*
 * System call entry
 *	Do not need to save the temporary register.
 *	The compiler saves the permanent register.
 *		+---------------+
 *	ssp ->	| SPSR		|
 *		| ip		| Function code
 *		| lr		| Return address
 *		+---------------+
 */
	.text
	.balign	4
	.globl	Csym(knl_call_entry)
Csym(knl_call_entry):

#if USE_TRAP
	/* SVC mode/During interrupt disable CPSR.I=1 F=? */
	ldr	ip, [sp]
	and	ip, ip, #PSR_DI
	orr	ip, ip, #PSR_SVC
	msr	cpsr_c, ip		// Return interrupt disable state to caller's state
#else 

	cmp	ip, #0		//	< 0: System call
				//	>= 0: Extended SVC
	bge	l_no_more_stack	// don't create stack

	stmfd	sp!, {ip, lr}
	mrs	ip, cpsr
	stmfd	sp!, {ip}
  l_no_more_stack:
#endif

	stmfd	sp!, {r10, fp}		// Save register for work
	add	fp, sp, #2*4

	ldr	ip, =Csym(knl_taskmode)		// Task mode flag update
	ldr	r10, [ip]
	stmfd	sp!, {r10}		// taskmode save 
	mov	lr, r10, lsl #16
	str	lr, [ip]

#if USE_DBGSPT & USE_HOOK_TRACE
	ldr	ip, =hook_enter_jmp	// Hook processing
	ldr	pc, [ip]
  ret_hook_enter:
#endif

	ldr	lr, [fp, #4]		// lr = Function code
	cmp	lr, #0			//	< 0: System call
	bge	l_esvc_function		//	>= 0: Extended SVC

#if USE_TRAP | (USE_DBGSPT & USE_HOOK_TRACE)
	/* micro T-Kernel System Call */
	mov	r10, lr, asr #16	// r10 = Function number
	ldr	ip, =N_TFN + 0xffff8000
	cmp	r10, ip
	bgt	l_illegal_svc

	mov	lr, lr, lsr #8
	and	lr, lr, #0xff		// lr = Number of arguments
	cmp	lr, #5
	bne	l_nocopy
	ldr	ip, [r4]		// Copy fifth argument
	stmfd	sp!, {ip}
  l_nocopy:

	ldr	ip, =_svctbl - (0xffff8000 << 2)
	mov	lr, pc
	ldr	pc, [ip, r10, lsl #2]	// micro T-Kernel system call
#else
	b	l_illegal_svc
#endif

	sub	sp, fp, #3*4
  l_retsvc:
#if USE_DBGSPT & USE_HOOK_TRACE
	ldr	ip, =hook_leave_jmp	// Hook processing
	ldr	pc, [ip]
  ret_hook_leave:
#endif

	ldmfd	sp!, {r1, r10, fp}	// Restore register for work

	ldr	ip, =Csym(knl_taskmode)		// Task mode restore
	str	r1, [ip]

	EXC_RETURN

  l_illegal_svc:
	ldr	r0, =E_RSFN
	b	l_retsvc

  l_esvc_function:
#if CFN_MAX_SSYID > 0
	/* Extended SVC */
	mov	r1, lr			// r1 = Function code
	bl	Csym(knl_svc_ientry)	// svc_ientry(pk_para, fncd)
	b	l_retsvc
#else
	ldr	r0, =E_SYS
	b	l_retsvc
#endif /* CFN_MAX_SSYID > 0 */


#if USE_DBGSPT & USE_HOOK_TRACE
/*
 * System call/Extended SVC hook routine call
 *	VP enter( FN fncd, TD_CALINF *calinf, ... )
 *	void leave( FN fncd, INT ret, VP exinf )
 *
 *	typedef struct td_calinf {
 *		VP	ssp;	System stack pointer
 *		VP	r11;	Flame pointer when calling
 *	} TD_CALINF;
 */
	.text
	.balign	4
hook_enter:
	stmfd	sp!, {r0-r3, r8-r9}	// Save argument and register for work
	mov	r8, sp			// r8 = Keep stack position

	ldr	ip, [fp, #-4]		// Flame pointer when calling
	stmfd	sp!, {fp, ip}		// Create TD_CALINF
	mov	r9, sp			// r9 = &TD_CALINF

	ldr	lr, [fp, #4]		// lr = Function code
	cmp	lr, #0			//	< 0: System call
	bge	l_hooksvc		//	>= 0: Extended SVC

	mov	lr, lr, lsr #8
	and	lr, lr, #0xff		// Number of arguments

	cmp	lr, #5
	ldrge	ip, [r4]
	stmgefd	sp!, {ip}		// Fifth argument
	cmp	lr, #4
	stmgefd	sp!, {r3}		// Fourth argument
	cmp	lr, #3
	stmgefd	sp!, {r2}		// Third argument
	mov	r3, r1			// Second argument
  l_hooksvc:
	mov	r2, r0			// First argument
	mov	r1, r9			// calinf
	ldr	r0, [fp, #4]		// fncd
	ldr	ip, =Csym(knl_hook_enterfn)
	ldr	ip, [ip]
	mov	lr, pc
	bx	ip			// exinf = enter(fncd, ...)
	mov	r9, r0			// Temporarily save 'exinf'

	mov	sp, r8			// Return stack position
	ldmfd	sp!, {r0-r3, r8}	// Restore argument and register for work
	swp	r9, r9, [sp]		// Restore 'r9' and save 'exinf' in it
	b	ret_hook_enter

hook_leave:
	mov	r1, r0			// r1 = ret

	mov	lr, #0
	ldr	r0, [fp, #4]		// r0 = Function code
	cmp	r0, #0			//	< 0: System call
	bge	l_hooksvc2		//	>= 0: Extended SVC

	mov	lr, r0, lsr #8
	and	lr, lr, #0xff		// Number of arguments
	subs	lr, lr, #4
	movlt	lr, #0
  l_hooksvc2:

	add	lr, lr, #3		// Whether 'hook_enter' is executed
	sub	ip, sp, fp		// Check by stack usage
	sub	sp, fp, #4*4		// Location in which 'sp = exinf' is saved
	cmp	lr, ip, lsr #2		// If 'hook_enter' is executed,
	ldrne	r2, [sp]		// Get 'exinf' from stack
	ldreq	r2, =0			// If 'exinf' is not saved, 0

	str	r9, [sp]		// r9 save
	mov	r9, r1			// Save 'ret' in 'r9'

	ldr	ip, =Csym(knl_hook_leavefn)
	ldr	ip, [ip]
	mov	lr, pc
	bx	ip			// call leave(fncd, ret, exinf)

	mov	r0, r9			// r0 = ret restore
	ldmfd	sp!, {r9}		// r9 restore
	b	ret_hook_leave

/*
 * Set/Free system call/extended SVC hook routine 
 */
	.text
	.balign	4
	.globl	Csym(knl_hook_svc)
Csym(knl_hook_svc):
	ldr	r0, =hook_enter_jmp
	ldr	r1, =hook_leave_jmp
	ldr	r2, =hook_enter
	ldr	r3, =hook_leave
	str	r2, [r0]
	str	r3, [r1]
	bx	lr

	.globl	Csym(knl_unhook_svc)
Csym(knl_unhook_svc):
	ldr	r0, =hook_enter_jmp
	ldr	r1, =hook_leave_jmp
	ldr	r2, =ret_hook_enter
	ldr	r3, =ret_hook_leave
	str	r2, [r0]
	str	r3, [r1]
	bx	lr

			.data
			.balign	4
  hook_enter_jmp:	.long	ret_hook_enter
  hook_leave_jmp:	.long	ret_hook_leave

#endif /* USE_DBGSPT & USE_HOOK_TRACE */
#endif /* USE_TRAP | USE_DBGSPT | CFN_MAX_SSYID > 0 */

/* ------------------------------------------------------------------------ */

#if USE_DBGSPT & (USE_TRAP | USE_HOOK_TRACE)
/*
 * Debugger support function service call entry table
 */

	.text
	.balign	4
_tdsvctbl:
	.int	Csym(knl_no_support)
#include <sys/svc/tdsvctbl.h>

/*
 * Debugger support function service call entry
 */
	.text
	.balign	4
	.globl	Csym(knl_call_dbgspt)
Csym(knl_call_dbgspt):
#if USE_TRAP
	/* SVC mode/During interrupt disable CPSR.I=1 F=? */
	ldr	ip, [sp]
	and	ip, ip, #PSR_DI
	orr	ip, ip, #PSR_SVC
	msr	cpsr_c, ip		// Return interrupt disable state to caller's state
#else
	stmfd	sp!, {ip, lr}
	mrs	ip, cpsr
	stmfd	sp!, {ip}
#endif

	stmfd	sp!, {r10, fp}		// Save register for work
	add	fp, sp, #2*4

	ldr	lr, [fp, #4]		// lr = Function code
	mov	r10, lr, asr #16
	ldr	ip, =N_TDFN + 0xffff8000
	cmp	r10, ip
	bgt	b_illegal_svc

	ldr	ip, =_tdsvctbl - (0xffff8000 << 2)
	mov	lr, pc
	ldr	pc, [ip, r10, lsl #2]	// micro T-Kernel service call (DS)

  b_retsvc:
	ldmfd	sp!, {r10, fp}		// Restore register for work
	EXC_RETURN


  b_illegal_svc:
	ldr	r0, =E_RSFN
	b	b_retsvc

#endif /* USE_DBGSPT & (USE_TRAP | USE_HOOK_TRACE) */



/* ------------------------------------------------------------------------ */
/*
 * High level programming language routine for timer handler
 */
	.text
	.balign	4
	.globl	Csym(knl_timer_handler_startup)
Csym(knl_timer_handler_startup):
	/* IRQ mode/During interrupt disable CPSR.I=1 F=? */
#if USE_TRAP
	msr	cpsr_c, #PSR_SVC|PSR_DI	// Move to SVC mode/Interrupt disable
#else
	msr	cpsr_c, #PSR_IRQ|PSR_DI	// Move to IRQ mode/Interrupt disable
#endif

	stmfd	sp!, {r0-r2, r4-r5, lr}	// Register save

	ldr	r4, =Csym(knl_taskindp)		// Enter task independent part
	ldr	r5, [r4]
	add	r0, r5, #1
	str	r0, [r4]

	bl	Csym(knl_timer_handler)	// call timer_handler()
	/* Return by interrupt disable CPSR.I=1 F=1 */

	str	r5, [r4]		// Leave task independent part

	ldmfd	sp!, {r0-r2, r4-r5, lr}	// Register restore

#if USE_TRAP
	TK_RET_INT_FIQ PSR_IRQ		// tk_ret_int_impl()
#else
	swp	r3, lr, [sp]		// r3 restore, R14_irq save
	bl	tk_ret_int_impl
#endif

/* ------------------------------------------------------------------------ */
