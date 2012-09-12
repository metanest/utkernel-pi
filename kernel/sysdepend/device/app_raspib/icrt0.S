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
 * modified for Raspberry Pi Type B by Makoto Kishimoto
 */

/*
 *  @(#) icrt0.S
 *
 *    Start up module
 */

#include "hwconfig.h"
#include "utk_config.h"

#include <machine.h>
#include <tk/asm.h>

#if USE_IMALLOC
	/* Low level memory manager information */
	.comm	Csym(knl_lowmem_top), 4	// Head of area (Low address)
	.comm	Csym(knl_lowmem_limit), 4	// End of area (High address)
#endif

/*
 * Vector Table
 */
        .section .vector,"ax"
        .code 32
        .align 0
        .global __reset
__reset:
        b       start              /* reset */
        .global undef_vector
undef_vector:
        b       undef_vector       /* undefined operation */
        .global swi_vector
swi_vector:
        b       swi_handler        /* software interrupt */
        .global prefetch_vector
prefetch_vector:
        b       prefetch_vector    /* prefetch abort */
        .global data_abort_vector
data_abort_vector:
        b       data_abort_vector  /* data abort */
        .global reserved_vector
reserved_vector:
        b       reserved_vector    /* reserved */
        .global irq_vector
irq_vector:
        b       knl_irq_handler    /* IRQ: kernel handler */
        .global fiq_vector
fiq_vector:
        b       fiq_vector         /* FIQ */

/*
 * Start up routine
 */
        .section .ftext,"ax"
        .code 32
        .align     0

        .global    start
start:
	msr     cpsr, #(PSR_SVC|PSR_DI)

init_stacks:
        ldr  r1, =EXCEPTION_STACK_TOP
#if ABT_STACK_SIZE != 0
	msr  cpsr, #(PSR_ABT|PSR_DI)
	mov  sp, r1
	sub  r1, r1, #ABT_STACK_SIZE
#endif
#if UND_STACK_SIZE != 0
	msr  cpsr, #(PSR_UND|PSR_DI)
	mov  sp, r1
	sub  r1, r1, #UND_STACK_SIZE
#endif
#if USR_STACK_SIZE != 0
	msr  cpsr, #(PSR_USR|PSR_DI)
	mov  sp, =APPLICATION_STACK_TOP
#endif
#if FIQ_STACK_SIZE != 0
	msr  cpsr, #(PSR_FIQ|PSR_DI)
	mov  sp, r1
	sub  r1, r1, #FIQ_STACK_SIZE
#endif
	msr  cpsr, #(PSR_IRQ|PSR_DI)
	mov  sp, r1
	sub  r1, r1, #IRQ_STACK_SIZE

	msr  cpsr, #(PSR_SVC|PSR_DI)
	mov  sp, r1

#if USE_TMONITOR
	ldr     r0, =tm_init
	mov     lr, pc
	bx      r0
#endif

#if USE_NOINIT
	ldr     r9, =__noinit_end   /* dst address */
#else
	/* .bss */
	ldr     r9, =__bss_start   /* dst address */
#endif
	ldr     r10, =__bss_end
	subs    r10, r10, r9       /* r10 := data_size */
	beq     bss_done           /* if __bss_start == __bss_end */
	mov     r0, #0

bss_loop:
	stmia   r9!, {r0}
	subs    r10, r10, #4
	bne     bss_loop           /* if data_size != 0 */

bss_done:

#if USE_IMALLOC
	ldr     r5, =SYSTEMAREA_TOP
	cmp	r9, r5			// __bss_end or SYSTEMAREA_TOP
	movhi	r5, r9			// Either of High addresses
	ldr	ip, =knl_lowmem_top
	str	r5, [ip]		// knl_lowmem_top = __bss_end or SYSTEMAREA_TOP
	ldr     r5, =SYSTEMAREA_END
	ldr	ip, =knl_lowmem_limit
	str	r5, [ip]		// knl_lowmem_limit = SYSTEMAREA_END
#endif

kernel_start:
	ldr	ip, =Csym(main)
	mov	r0, #0
	mov	r14, pc
	bx	ip
l_end:
	b     l_end

#if USE_TMONITOR
tm_init:
	/* initialize serial I/O */
	ldr     r0, =sio_init
	bx      r0
	/* return directly to the place tm_init called from sio_init */
#endif

swi_handler:
	str    lr, [sp, #-4]!
	str    ip, [sp, #-4]!
	mrs    ip, spsr
	str    ip, [sp, #-4]!

	ldr    ip, [lr, #-4]   /* load SWI No. */
	bic    ip, ip, #(0xff << 24)

	ldr    lr, =Csym(knl_intvec) /* exception vector table */
	add    ip, lr, ip, LSL #2 /* lr := lr + ip*4 = vecaddr */
	ldr    lr, [ip]
	bx     lr

	.global knl_irq_handler
knl_irq_handler:
	sub    lr, lr, #4
	stmfd  sp!, {lr}  /* sp-> lr_xxx */
	stmfd  sp!, {ip} /* sp-> ip, lr_xxx */
	mrs    ip, spsr
	stmfd  sp!, {ip} /* sp-> spsr_xxx, ip, lr_xxx */
	stmfd  sp!, {r3} /* sp-> r3, spsr_xxx, ip, lr_xxx */

	/*
	 * based on Broadcom BCM2835 Peripherals datasheet p. 111
	 */

#define BCM2708_PERI_BASE 0x20000000
#define BCM2708_ARM_BASE (BCM2708_PERI_BASE + 0xB000)
#define BCM2708_ARMCTRL_IC_BASE (BCM2708_ARM_BASE + 0x200)
#define BCM2708_ARM_IRQ_PEND0 (BCM2708_ARMCTRL_IC_BASE + 0x0)
#define BCM2708_ARM_IRQ_PEND1 (BCM2708_ARMCTRL_IC_BASE + 0x4)
#define BCM2708_ARM_IRQ_PEND2 (BCM2708_ARMCTRL_IC_BASE + 0x8)
#define BCM2708_ARM_IRQ0_BASE 64
#define BCM2708_ARM_IRQ1_BASE 0
#define BCM2708_ARM_IRQ2_BASE 32

	.macro	get_irqnr_preamble base, tmp
	ldr	\base, =ARMCTRL_IC_BASE
	.endm

	.macro	get_irqnr_and_base irqnr, irqstat, base, tmp
	ldr	\irqstat, [\base, #(ARM_IRQ_PEND0 - ARMCTRL_IC_BASE)] @ get masked status
	mov	\irqnr, #(ARM_IRQ0_BASE + 31)
	and	\tmp, \irqstat, #0x300		@ save bits 8 and 9
	bics	\irqstat, \irqstat, #0x300	@ clear bits 8 and 9, and test
	bne	1010f
	tst	\tmp, #0x100
	ldrne	\irqstat, [\base, #(ARM_IRQ_PEND1 - ARMCTRL_IC_BASE)]
	movne	\irqnr, #(ARM_IRQ1_BASE + 31)
	@ Mask out the interrupts also present in PEND0 - see SW-5809
	bicne	\irqstat, #((1<<7) | (1<<9) | (1<<10))
	bicne	\irqstat, #((1<<18) | (1<<19))
	bne	1010f
	tst	\tmp, #0x200
	ldrne	\irqstat, [\base, #(ARM_IRQ_PEND2 - ARMCTRL_IC_BASE)]
	movne	\irqnr, #(ARM_IRQ2_BASE + 31)
	@ Mask out the interrupts also present in PEND0 - see SW-5809
	bicne	\irqstat, #((1<<21) | (1<<22) | (1<<23) | (1<<24) | (1<<25))
	bicne	\irqstat, #((1<<30))
	beq	1020f
1010:
	@ For non-zero x, LSB(x) = 31 - CLZ(x^(x-1))
	@ N.B. CLZ is an ARM5 instruction.
	sub	\tmp, \irqstat, #1
	eor	\irqstat, \irqstat, \tmp
	clz	\tmp, \irqstat
	sub	\irqnr, \tmp
1020: @ EQ will be set if no irqs pending
	.endm

	stmfd  sp!, {r0-r1}

	get_irqnr_preamble r0, r1
	get_irqnr_and_base lr, r3, r0, r1

	ldmfd	sp!, {r0-r1}

	ldr    ip, =Csym(knl_intvec) /* exception vector table */
	add    ip, ip, lr, LSL #2 /* ip := &vector[IRQ No.] */
	ldr    r3, [ip] /* r3 := vector[IRQ No.] */
	mov    lr, pc
	bx     r3


