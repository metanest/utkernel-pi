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
 *	sysinit_main.c (sysinit)
 *	Initialize System
 */

#include "kernel.h"
#include "sysinit.h"
#include "patch.h"
#include <tk/util.h>
#include <sys/debug.h>

#include "utk_config.h"

/*
 * Initialize sequence before micro T-Kernel starts
 *	Perform preparation necessary to start micro T-Kernel.
 */
IMPORT void knl_init_system( void );

/*
 * Start micro T-Kernel
 *	Start micro T-Kernel and the initial task specified by 'ctsk'.
 */
IMPORT void knl_t_kernel_main( T_CTSK *ctsk );

/*
 * Start System
 *	At this point, start each subsystem and each device driver.
 *	Return from function after starting.
 */
IMPORT void knl_start_system( void );

#if USE_CLEANUP
/*
 * Stop System
 *	Never return from this function.
 *
 *	fin  =	 0 : Power off
 *		-1 : reset and re-start	(Reset -> Boot -> Start)
 *		-2 : fast re-start		(Start)
 *		-3 : Normal re-start		(Boot -> Start)
 *
 *	fin are not always supported.
 */
IMPORT void knl_shutdown_system( INT fin );
#endif /* USE_CLEANUP */

/*
 * Main initial task sequence (sysmain)
 */
IMPORT INT knl_init_task_main( void );

/* ------------------------------------------------------------------------ */

/*
 * Initial task
 */
EXPORT void knl_init_task(void)
{
	INT fin;

#if USE_SYSDEPEND_PATCH1
	/* System-dependent processes (before knl_start_system) */
	knl_sysdepend_patch1();
#endif

	/* Start system */
	knl_start_system();

#if USE_SYSDEPEND_PATCH2
	/* System-dependent processes (after knl_start_system) */
	knl_sysdepend_patch2();
#endif

	/* Initial task main */
	fin = knl_init_task_main();

	/* Stop System */
#if USE_CLEANUP
	knl_shutdown_system(fin);	/* Never return */
#else
	DISABLE_INTERRUPT;
	for(;;) {
		;
	}
#endif /* USE_CLEANUP */
}

/*
 * Initial task creation parameter
 */
IMPORT const T_CTSK knl_c_init_task;

/*
 * Entry for starting Kernel
 */
EXPORT int main( void )
{
	/* Initialize sequence before T-Kernel starts */
	knl_init_system();

	/* Start T-Kernel */
	knl_t_kernel_main((T_CTSK *)&knl_c_init_task);	/* Never return */

	return 0;
}

