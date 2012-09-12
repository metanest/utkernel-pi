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

IMPORT void sio_send_frame( const UB* buf, INT size );
IMPORT void sio_recv_frame( UB* buf, INT size );

/*
 * Ctrl-C is not supported
 */
EXPORT INT tm_putstring( UB *buff )
{
	const UB* p = buff;
	static const char CR = 0x0d;
	INT imask;

	DI(imask);

	while (*p != NULL) {
		if (*p == 0x0a) { /* LF */
			sio_send_frame((const VP)&CR, 1);
			sio_send_frame((const VP)p, 1);
		} else {
			sio_send_frame((const VP)p, 1);
		}
		p++;
	}

	EI(imask);

	return 0;
}

