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
 *	wait.c
 *	Common Routine for Synchronization
 */

/** [BEGIN Common Definitions] */
#include "kernel.h"
#include "task.h"
#include "wait.h"
/** [END Common Definitions] */

#ifdef USE_FUNC_WAIT_RELEASE_OK
EXPORT void knl_wait_release_ok( TCB *tcb )
{
	knl_wait_release(tcb);
	*tcb->wercd = E_OK;
}
#endif /* USE_FUNC_WAIT_RELEASE_OK */

#ifdef USE_FUNC_WAIT_RELEASE_OK_ERCD
EXPORT void knl_wait_release_ok_ercd( TCB *tcb, ER ercd )
{
	knl_wait_release(tcb);
	*tcb->wercd = ercd;
}
#endif /* USE_FUNC_WAIT_RELEASE_OK_ERCD */

#ifdef USE_FUNC_WAIT_RELEASE_NG
EXPORT void knl_wait_release_ng( TCB *tcb, ER ercd )
{
	knl_wait_release(tcb);
	if ( tcb->wspec->rel_wai_hook != NULL ) {
		(*tcb->wspec->rel_wai_hook)(tcb);
	}
	*tcb->wercd = ercd;
}
#endif /* USE_FUNC_WAIT_RELEASE_NG */

#ifdef USE_FUNC_WAIT_RELEASE_TMOUT
EXPORT void knl_wait_release_tmout( TCB *tcb )
{
	QueRemove(&tcb->tskque);
	knl_make_non_wait(tcb);
	if ( tcb->wspec->rel_wai_hook != NULL ) {
		(*tcb->wspec->rel_wai_hook)(tcb);
	}
}
#endif /* USE_FUNC_WAIT_RELEASE_TMOUT */

#ifdef USE_FUNC_MAKE_WAIT
/*
 * Change the active task state to wait state and connect to the
 * timer event queue.
 *	Normally, 'knl_ctxtsk' is in the RUN state, but when an interrupt
 *	occurs during executing system call, 'knl_ctxtsk' may become the
 *	other state by system call called in the interrupt handler.
 *	However, it does not be in WAIT state.
 *
 *	"include/tk/typedef.h"
 *	typedef	W		TMO;
 *	typedef UW		RELTIM;
 *	#define TMO_FEVR	(-1)
 */
EXPORT void knl_make_wait( TMO tmout, ATR atr )
{
	switch ( knl_ctxtsk->state ) {
	  case TS_READY:
		knl_make_non_ready(knl_ctxtsk);
		knl_ctxtsk->state = TS_WAIT;
		break;
	  case TS_SUSPEND:
		knl_ctxtsk->state = TS_WAITSUS;
		break;
	}
	knl_timer_insert(&knl_ctxtsk->wtmeb, tmout, (CBACK)knl_wait_release_tmout, knl_ctxtsk);
}
#endif /* USE_FUNC_MAKE_WAIT */

#ifdef USE_FUNC_MAKE_WAIT_RELTIM
EXPORT void knl_make_wait_reltim( RELTIM tmout, ATR atr )
{
	switch ( knl_ctxtsk->state ) {
	  case TS_READY:
		knl_make_non_ready(knl_ctxtsk);
		knl_ctxtsk->state = TS_WAIT;
		break;
	  case TS_SUSPEND:
		knl_ctxtsk->state = TS_WAITSUS;
		break;
	}
	knl_timer_insert_reltim(&knl_ctxtsk->wtmeb, tmout, (CBACK)knl_wait_release_tmout, knl_ctxtsk);
}
#endif /* USE_FUNC_MAKE_WAIT_RELTIM */

#ifdef USE_FUNC_WAIT_DELETE
/*
 * Release all tasks connected to the wait queue, and define it
 * as E_DLT error.
 */
EXPORT void knl_wait_delete( QUEUE *wait_queue )
{
	TCB	*tcb;

	while ( !isQueEmpty(wait_queue) ) {
		tcb = (TCB*)wait_queue->next;
		knl_wait_release(tcb);
		*tcb->wercd = E_DLT;
	}
}
#endif /* USE_FUNC_WAIT_DELETE */

#ifdef USE_FUNC_WAIT_TSKID
/*
 * Get ID of the head task in the wait queue.
 */
EXPORT ID knl_wait_tskid( QUEUE *wait_queue )
{
	if ( isQueEmpty(wait_queue) ) {
		return 0;
	}

	return ((TCB*)wait_queue->next)->tskid;
}
#endif /* USE_FUNC_WAIT_TSKID */

#ifdef USE_FUNC_GCB_MAKE_WAIT
/*
 * Change the active task state to wait state and connect to the timer wait 
 * queue and the object wait queue. Also set 'wid' in 'knl_ctxtsk'.
 */
EXPORT void knl_gcb_make_wait( GCB *gcb, TMO tmout )
{
	*knl_ctxtsk->wercd = E_TMOUT;
	if ( tmout != TMO_POL ) {
		knl_ctxtsk->wid = gcb->objid;
		knl_make_wait(tmout, gcb->objatr);
		if ( (gcb->objatr & TA_TPRI) != 0 ) {
			knl_queue_insert_tpri(knl_ctxtsk, &gcb->wait_queue);
		} else {
			QueInsert(&knl_ctxtsk->tskque, &gcb->wait_queue);
		}
	}
}
#endif /* USE_FUNC_GCB_MAKE_WAIT */

#ifdef USE_FUNC_GCB_CHANGE_PRIORITY
/*
 * When the task priority changes, adjust the task position at the wait queue.
 * It is called only if the object attribute TA_TPRI is specified.
 *
 */
EXPORT void knl_gcb_change_priority( GCB *gcb, TCB *tcb )
{
	QueRemove(&tcb->tskque);
	knl_queue_insert_tpri(tcb, &gcb->wait_queue);
}
#endif /* USE_FUNC_GCB_CHANGE_PRIORITY */

#ifdef USE_FUNC_GCB_TOP_OF_WAIT_QUEUE
/*
 * Search the first task of wait queue include "tcb" with target.
 * (Not insert "tcb" into wait queue.)
 *
 */
EXPORT TCB* knl_gcb_top_of_wait_queue( GCB *gcb, TCB *tcb )
{
	TCB	*q;

	if ( isQueEmpty(&gcb->wait_queue) ) {
		return tcb;
	}

	q = (TCB*)gcb->wait_queue.next;
	if ( (gcb->objatr & TA_TPRI) == 0 ) {
		return q;
	}

	return ( tcb->priority < q->priority )? tcb: q;
}
#endif /* USE_FUNC_GCB_TOP_OF_WAIT_QUEUE */
