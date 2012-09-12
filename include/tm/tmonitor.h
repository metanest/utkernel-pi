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
 *	@(#)tmonitor.h (tm)
 *
 *	T-Monitor equivalent function
 */

#ifndef __TM_TMONITOR_H__
#define __TM_TMONITOR_H__

#include <basic.h>

#ifdef __cplusplus
extern "C" {
#endif


/*
 * Monitor service function
 */
IMPORT void tm_monitor( void );
IMPORT INT  tm_getchar( INT wait );
IMPORT INT  tm_putchar( INT c );
IMPORT INT  tm_getline( UB *buff );
IMPORT INT  tm_putstring( const UB *buff );
IMPORT INT  tm_command( const UB *buff );
IMPORT void tm_exit( INT mode );

#ifdef __cplusplus
}
#endif
#endif /* __TM_TMONITOR_H__ */
