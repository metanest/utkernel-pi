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
 *	klock.c
 *	Kernel Lock
 */

#include "kernel.h"
#include "task.h"
#include "ready_queue.h"

/*
 * Object lock
 *	Do not call from critical section
 */
EXPORT void knl_LockOBJ( OBJLOCK *loc )
{
	BOOL	klocked;

  retry:
	BEGIN_CRITICAL_SECTION;
	klocked = knl_ctxtsk->klocked;
	if ( !klocked ) {
		if ( loc->wtskq.next == NULL ) {
			/* Lock */
			QueInit(&loc->wtskq);

			knl_ctxtsk->klocked = klocked = TRUE;
			knl_ready_queue.klocktsk = knl_ctxtsk;
		} else {
			/* Ready for lock */
			knl_ready_queue_delete(&knl_ready_queue, knl_ctxtsk);
			knl_ctxtsk->klockwait = TRUE;
			QueInsert(&knl_ctxtsk->tskque, &loc->wtskq);

			knl_schedtsk = knl_ready_queue_top(&knl_ready_queue);
			knl_dispatch_request();
		}
	}
	END_CRITICAL_SECTION;
	/* Since wait could be freed without getting lock, 
	   need to re-try if lock is not got */
	if ( !klocked ) {
		goto retry;
	}
}

/*
 * Object unlock
 *	It may be called from a critical section.
 */
EXPORT void knl_UnlockOBJ( OBJLOCK *loc )
{
	TCB	*tcb;

	BEGIN_CRITICAL_SECTION;
	knl_ctxtsk->klocked = FALSE;
	knl_ready_queue.klocktsk = NULL;

	tcb = (TCB*)QueRemoveNext(&loc->wtskq);
	if ( tcb == NULL ) {
		/* Free lock */
		loc->wtskq.next = NULL;
	} else {
		/* Wake lock wait task */
		tcb->klockwait = FALSE;
		tcb->klocked = TRUE;
		knl_ready_queue_insert_top(&knl_ready_queue, tcb);
	}

	knl_schedtsk = knl_ready_queue_top(&knl_ready_queue);
	if ( knl_ctxtsk != knl_schedtsk ) {
		knl_dispatch_request();
	}
	END_CRITICAL_SECTION;
}
