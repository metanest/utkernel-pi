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
 *	bitop.c (tstdlib)
 *	T-Kernel common standard library
 */

/** [BEGIN Common Definitions] */
#include <basic.h>
#include <tk/tkernel.h>
#include "utk_config.h"

/*** macros ***/
/* bit operation macro */
#if BIGENDIAN
#define _BIT_SET_N(n) ( (UB)0x80 >> ((n) & 7) )
#define _BIT_SHIFT(n) ( (UB)n >> 1 )
#else
#define _BIT_SET_N(n) ( (UB)0x01 << ((n) & 7) )
#define _BIT_SHIFT(n) ( (UB)n << 1 )
#endif
/** [END Common Definitions] */


/*** bit operation ***/
#ifdef USE_FUNC_TSTDLIB_BITCLR
/* tstdlib_bitclr : clear specified bit */
void
knl_tstdlib_bitclr( VP base, W offset )
{
	register UB *cp, mask;

	if (offset < 0) {
		return;
	}

	cp = (UB*)base;
	cp += offset / 8;

	mask = _BIT_SET_N(offset);

	*cp &= ~mask;
}
#endif /* USE_FUNC_TSTDLIB_BITCLR */

#ifdef USE_FUNC_TSTDLIB_BITSET
/* tstdlib_bitset : set specified bit */
void
knl_tstdlib_bitset( VP base, W offset )
{
	register UB *cp, mask;

	if (offset < 0) {
		return;
	}

	cp = (UB*)base;
	cp += offset / 8;

	mask = _BIT_SET_N(offset);

	*cp |= mask;
}
#endif /* USE_FUNC_TSTDLIB_BITSET */

#ifdef USE_FUNC_TSTDLIB_BITSEARCH1
/* tstdlib_bitsearch1 : perform 1 search on bit string */
W
knl_tstdlib_bitsearch1( VP base, W offset, W width )
{
	register UB *cp, mask;
	register W position;

	if ((offset < 0) || (width < 0)) {
		return -1;
	}

	cp = (UB*)base;
	cp += offset / 8;

	position = 0;
	mask = _BIT_SET_N(offset);

	while (position < width) {
		if (*cp) {		/* includes 1 --> search bit of 1 */
			while (1) {
				if (*cp & mask) {
					if (position < width) {
						return position;
					} else {
						return -1;
					}
				}
				mask = _BIT_SHIFT(mask);
				++position;
			}
		} else {		/* all bits are 0 --> 1 Byte skip */
			if (position) {
				position += 8;
			} else {
				position = 8 - (offset & 7);
				mask = _BIT_SET_N(0);
			}
			cp++;
		}
	}

	return -1;
}
#endif /* USE_FUNC_TSTDLIB_BITSEARCH1 */
