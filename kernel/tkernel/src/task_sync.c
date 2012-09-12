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
 *	task_sync.c
 *	Task with Synchronize Function
 */

/** [BEGIN Common Definitions] */
#include "kernel.h"
#include "task.h"
#include "wait.h"
#include "check.h"
#include "limits.h"
#include "task_sync.h"
/** [END Common Definitions] */

#ifdef USE_FUNC_TK_SUS_TSK
/*
 * Suspend task
 */
SYSCALL ER tk_sus_tsk_impl( ID tskid )
{
	TCB	*tcb;
	TSTAT	state;
	ER	ercd = E_OK;

	CHECK_TSKID(tskid);
	CHECK_NONSELF(tskid);

	tcb = get_tcb(tskid);

	BEGIN_CRITICAL_SECTION;
	state = (TSTAT)tcb->state;
	if ( !knl_task_alive(state) ) {
		ercd = ( state == TS_NONEXIST )? E_NOEXS: E_OBJ;
		goto error_exit;
	}
	if ( tcb == knl_ctxtsk && knl_dispatch_disabled >= DDS_DISABLE ) {
		ercd = E_CTX;
		goto error_exit;
	}
	if ( tcb->suscnt == INT_MAX ) {
		ercd = E_QOVR;
		goto error_exit;
	}

	/* Update suspend request count */
	++tcb->suscnt;

	/* Move to forced wait state */
	if ( state == TS_READY ) {
		knl_make_non_ready(tcb);
		tcb->state = TS_SUSPEND;

	} else if ( state == TS_WAIT ) {
		tcb->state = TS_WAITSUS;
	}

    error_exit:
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_SUS_TSK */

#ifdef USE_FUNC_TK_RSM_TSK
/*
 * Resume task
 */
SYSCALL ER tk_rsm_tsk_impl( ID tskid )
{
	TCB	*tcb;
	ER	ercd = E_OK;

	CHECK_TSKID(tskid);
	CHECK_NONSELF(tskid);

	tcb = get_tcb(tskid);

	BEGIN_CRITICAL_SECTION;
	switch ( tcb->state ) {
	  case TS_NONEXIST:
		ercd = E_NOEXS;
		break;

	  case TS_DORMANT:
	  case TS_READY:
	  case TS_WAIT:
		ercd = E_OBJ;
		break;

	  case TS_SUSPEND:
		if ( --tcb->suscnt == 0 ) {
			knl_make_ready(tcb);
		}
		break;
	  case TS_WAITSUS:
		if ( --tcb->suscnt == 0 ) {
			tcb->state = TS_WAIT;
		}
		break;

	  default:
		ercd = E_SYS;
		break;
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_RSM_TSK */

#ifdef USE_FUNC_TK_FRSM_TSK
/*
 * Force resume task
 */
SYSCALL ER tk_frsm_tsk_impl( ID tskid )
{
	TCB	*tcb;
	ER	ercd = E_OK;

	CHECK_TSKID(tskid);
	CHECK_NONSELF(tskid);

	tcb = get_tcb(tskid);

	BEGIN_CRITICAL_SECTION;
	switch ( tcb->state ) {
	  case TS_NONEXIST:
		ercd = E_NOEXS;
		break;

	  case TS_DORMANT:
	  case TS_READY:
	  case TS_WAIT:
		ercd = E_OBJ;
		break;

	  case TS_SUSPEND:
		tcb->suscnt = 0;
		knl_make_ready(tcb);
		break;
	  case TS_WAITSUS:
		tcb->suscnt = 0;
		tcb->state = TS_WAIT;
		break;

	  default:
		ercd = E_SYS;
		break;
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_FRSM_TSK */

/* ------------------------------------------------------------------------ */

#ifdef USE_FUNC_WSPEC_SLP
/*
 * Definition of task wait specification
 */
EXPORT WSPEC knl_wspec_slp = { TTW_SLP, NULL, NULL };
#endif /* USE_FUNC_WSPEC_SLP */

#ifdef USE_FUNC_TK_SLP_TSK
/*
 * Move its own task state to wait state
 */
SYSCALL ER tk_slp_tsk_impl( TMO tmout )
{
	ER	ercd = E_OK;

	CHECK_TMOUT(tmout);
	CHECK_DISPATCH();

	BEGIN_CRITICAL_SECTION;

	if ( knl_ctxtsk->wupcnt > 0 ) {
		knl_ctxtsk->wupcnt--;
	} else {
		ercd = E_TMOUT;
		if ( tmout != TMO_POL ) {
			knl_ctxtsk->wspec = &knl_wspec_slp;
			knl_ctxtsk->wid = 0;
			knl_ctxtsk->wercd = &ercd;
			knl_make_wait(tmout, TA_NULL);
			QueInit(&knl_ctxtsk->tskque);
		}
	}

	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_SLP_TSK */

#ifdef USE_FUNC_TK_WUP_TSK
/*
 * Wakeup task
 */
SYSCALL ER tk_wup_tsk_impl( ID tskid )
{
	TCB	*tcb;
	TSTAT	state;
	ER	ercd = E_OK;

	CHECK_TSKID(tskid);
	CHECK_NONSELF(tskid);

	tcb = get_tcb(tskid);

	BEGIN_CRITICAL_SECTION;
	state = (TSTAT)tcb->state;
	if ( !knl_task_alive(state) ) {
		ercd = ( state == TS_NONEXIST )? E_NOEXS: E_OBJ;

	} else if ( (state & TS_WAIT) != 0 && tcb->wspec == &knl_wspec_slp ) {
		knl_wait_release_ok(tcb);

	} else if ( tcb->wupcnt == INT_MAX ) {
		ercd = E_QOVR;
	} else {
		++tcb->wupcnt;
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_WUP_TSK */

#ifdef USE_FUNC_TK_CAN_WUP
/*
 * Cancel wakeup request
 */
SYSCALL INT tk_can_wup_impl( ID tskid )
{
	TCB	*tcb;
	ER	ercd = E_OK;

	CHECK_TSKID_SELF(tskid);

	tcb = get_tcb_self(tskid);

	BEGIN_CRITICAL_SECTION;
	switch ( tcb->state ) {
	  case TS_NONEXIST:
		ercd = E_NOEXS;
		break;
	  case TS_DORMANT:
		ercd = E_OBJ;
		break;

	  default:
		ercd = tcb->wupcnt;
		tcb->wupcnt = 0;
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_CAN_WUP */
