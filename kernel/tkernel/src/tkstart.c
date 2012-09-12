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
 *	tkstart.c
 *	micro T-Kernel Startup / Finalization
 */

#include "kernel.h"
#include "timer.h"
#include "task.h"
#include <tm/tmonitor.h>

/*
 * Start initial task
 */
LOCAL void knl_init_task_startup( T_CTSK *ctsk )
{
	ER	ercd;

#if TA_GP
	extern int _gp;
	ercd = tk_cre_tsk_impl(ctsk, 0,0,0,0,&_gp);
#else
	ercd = tk_cre_tsk_impl(ctsk);
#endif
	if ( ercd < E_OK ) {
		goto err_exit;
	}

	ercd = tk_sta_tsk_impl((ID)ercd, 0);
	if ( ercd < E_OK ) {
		goto err_exit;
	}

	return;

err_exit:
#if USE_KERNEL_MESSAGE
	tm_putstring((UB*)"init_task can not started\n");
#endif
	tm_monitor();
}

/*
 * Call module initialization
 */
LOCAL void knl_init_module( ER (*initfunc)( void ), UB *name )
{
	ER	ercd;

	ercd = (*initfunc)();
	if ( ercd < E_OK ) {
#if USE_KERNEL_MESSAGE
		tm_putstring(name);
		tm_putstring((UB*)" : module initialize error\n");
#endif
		tm_monitor();
	}
}
#define InitModule(name)	knl_init_module( knl_##name##_initialize, (UB*)#name )

/*
 * Initialize kernel and create/start initial task
 */
EXPORT void knl_t_kernel_main( T_CTSK *inittask )
{

	DISABLE_INTERRUPT;

	/*
	 * Target-dependent initialization
	 */
	InitModule(cpu);
	InitModule(tkdev);

	/*
	 * Each module initialization
	 */
	InitModule(task);
#if CFN_MAX_SEMID > 0
#ifdef USE_FUNC_SEMAPHORE_INITIALIZE
	InitModule(semaphore);
#endif
#endif
#if CFN_MAX_FLGID > 0
#ifdef USE_FUNC_EVENTFLAG_INITIALIZE
	InitModule(eventflag);
#endif
#endif
#if CFN_MAX_MBXID > 0
#ifdef USE_FUNC_MAILBOX_INITIALIZE
	InitModule(mailbox);
#endif
#endif
#if CFN_MAX_MBFID > 0
#ifdef USE_FUNC_MESSAGEBUFFER_INITIALIZE
	InitModule(messagebuffer);
#endif
#endif
#if CFN_MAX_PORID > 0
#ifdef USE_FUNC_RENDEZVOUS_INITIALIZE
	InitModule(rendezvous);
#endif
#endif
#if CFN_MAX_MTXID > 0
#ifdef USE_FUNC_MUTEX_INITIALIZE
	InitModule(mutex);
#endif
#endif
#if CFN_MAX_MPLID > 0
#ifdef USE_FUNC_MEMORYPOOL_INITIALIZE
	InitModule(memorypool);
#endif
#endif
#if CFN_MAX_MPFID > 0
#ifdef USE_FUNC_FIX_MEMORYPOOL_INITIALIZE
	InitModule(fix_memorypool);
#endif
#endif
#if CFN_MAX_CYCID > 0
#ifdef USE_FUNC_CYCLICHANDLER_INITIALIZE
	InitModule(cyclichandler);
#endif
#endif
#if CFN_MAX_ALMID > 0
#ifdef USE_FUNC_ALARMHANDLER_INITIALIZE
	InitModule(alarmhandler);
#endif
#endif
#if CFN_MAX_SSYID > 0
#ifdef USE_FUNC_SUBSYSTEM_INITIALIZE
	InitModule(subsystem);
#endif
#endif
	InitModule(timer);

	/*
	 * Create/start initial task
	 */
	knl_init_task_startup(inittask);

	knl_force_dispatch();
	/* No return */
}

#if USE_CLEANUP
/*
 * Finalization
 */
EXPORT void knl_t_kernel_shutdown( void )
{
	knl_timer_shutdown();
	knl_cpu_shutdown();
}

/*
 * Execute finalization and stop
 */
EXPORT void knl_t_kernel_exit( void )
{
	knl_t_kernel_shutdown();
	knl_tkdev_exit();
	/* No return */
}
#endif /* USE_CLEANUP */
