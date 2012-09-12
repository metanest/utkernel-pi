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

#include <typedef.h>
#include <stddef.h>
#include <tk/syslib.h>

IMPORT void sio_recv_frame( UB* buf, INT size );

/*
 * supported only on wait != 0 (polling not supported)
 */
EXPORT INT tm_getchar( INT wait )
{
	UB	p;
	UINT	imask;

	DI(imask);

	sio_recv_frame(&p, 1);

	EI(imask);

	return (INT)p;
}


