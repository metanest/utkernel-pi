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
 *	patch.c (AT91)
 *	System-dependent initialize process
 */

#include <basic.h>
#include <tk/tkernel.h>
#include <tk/sysdef.h>
#include "patch.h"


#if USE_SYSDEPEND_PATCH1
/*
 * System-dependent processes (before start_system)
 */
EXPORT void sysdepend_patch1( void )
{
	/* Do nothing */
}
#endif

#if USE_SYSDEPEND_PATCH2
/*
 * System-dependent processes (after start_system)
 */
EXPORT void sysdepend_patch2( void )
{
	/* Do nothing */
}
#endif
