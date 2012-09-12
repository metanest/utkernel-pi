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
 *	@(#)int.c (libtk/AT91)
 *
 *	Interrupt controller
 */

#include <stdint.h>

#include <basic.h>
#include <tk/syslib.h>
#include <tk/sysdef.h>

/*
 * Enable interrupt 
 *	Enables the interrupt specified in intvec.
 *	FIQ and IRQ interrupt can be specified. 
 */
EXPORT void EnableInt( INTVEC intvec )
{
	UINT	imask;
	uint32_t volatile *base = (uint32_t volatile *)0x2000b210;

	DI(imask);

	out_w(base + ((intvec >> 3) & ~3), 1U << (intvec & 31));

	EI(imask);
}

/*
 * Disable interrupt 
 *	Disables the interrupt specified in intvec.
 *	FIQ and IRQ interrupt can be specified.
 */
EXPORT void DisableInt( INTVEC intvec )
{
	UINT	imask;
	uint32_t volatile *base = (uint32_t volatile *)0x2000b21c;

	DI(imask);

	out_w(base + ((intvec >> 3) & ~3), 1U << (intvec & 31));

	EI(imask);
}
