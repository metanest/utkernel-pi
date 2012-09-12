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
 *	@(#)sysdef_depend.h
 *
 *	Definition about Raspberry Pi Type B
 *
 *	Included also from assembler program.
 */

#ifndef __TK_SYSDEF_DEPEND_H__
#define __TK_SYSDEF_DEPEND_H__

/*
 * Program status register (PSR)
 */
#define PSR_N		0x80000000	/* Condition flag Negative */
#define PSR_Z		0x40000000	/* Zero */
#define PSR_C		0x20000000	/* Carry */
#define PSR_V		0x10000000	/* Overflow */

#define PSR_I		0x00000080	/* Interrupt (IRQ) disable */
#define PSR_F		0x00000040	/* Fast Interrupt (FIQ) disable */
#define PSR_T		0x00000020	/* Thumb mode */

#define PSR_DI		(PSR_I | PSR_F)	/* All Interrupts disable */

#define PSR_M(n)	( n )		/* Processor mode 0-31 */
#define PSR_USR		PSR_M(16)	/* User mode */
#define PSR_FIQ		PSR_M(17)	/* Fast Interrupt (FIQ) mode */
#define PSR_IRQ		PSR_M(18)	/* Interrupt (IRQ) mode */
#define PSR_SVC		PSR_M(19)	/* Supervisor mode */
#define PSR_ABT		PSR_M(23)	/* Abort mode */
#define PSR_UND		PSR_M(27)	/* Undefined order mode */
#define PSR_SYS		PSR_M(31)	/* System mode */

/* ------------------------------------------------------------------------ */

/*
 * Software interrupt number for micro T-Kernel 
 */
#define SWI_SVC		100	/* micro T-Kernel system call/extension SVC */
#define SWI_RETINT	101	/* tk_ret_int() system call */
#define SWI_DISPATCH	102	/* Task dispatcher */
#define SWI_DEBUG	103	/* Debugger support function */

/* ------------------------------------------------------------------------ */

#define BCM2708_PERI_BASE 0x20000000
#define ARM_BASE (BCM2708_PERI_BASE + 0xB000)
#define ARMCTRL_IC_BASE (ARM_BASE + 0x200)
#define ARM_IRQ_PEND0 (ARMCTRL_IC_BASE + 0x0)
#define ARM_IRQ_PEND1 (ARMCTRL_IC_BASE + 0x4)
#define ARM_IRQ_PEND2 (ARMCTRL_IC_BASE + 0x8)
#define ARM_IRQ0_BASE 64
#define ARM_IRQ1_BASE 0
#define ARM_IRQ2_BASE 32

/* ------------------------------------------------------------------------ */

#define PL011_BASE 0x20201000
#define PL011_IO (0x20201000 + 0)
#define PL011_STAT (0x20201000 + 0x18)

#endif /* __TK_SYSDEF_DEPEND_H__ */
