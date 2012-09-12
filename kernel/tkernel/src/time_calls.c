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
 *	time_calls.c
 *	Time Management Function
 */

/** [BEGIN Common Definitions] */
#include "kernel.h"
#include "timer.h"
#include "task.h"
#include "wait.h"
#include "check.h"
#include "tkdev_timer.h"
#include "time_calls.h"
/** [END Common Definitions] */

#ifdef USE_FUNC_TK_SET_TIM
/*
 * Set system clock
 */
SYSCALL ER tk_set_tim_impl( SYSTIM *pk_tim )
{
	CHECK_PAR(pk_tim->hi >= 0);

	BEGIN_CRITICAL_SECTION;
	knl_real_time_ofs = ll_sub(knl_toLSYSTIM(pk_tim), knl_current_time);
	END_CRITICAL_SECTION;

	return E_OK;
}
#endif /* USE_FUNC_TK_SET_TIM */

#ifdef USE_FUNC_TK_GET_TIM
/*
 * Refer system clock
 */
SYSCALL ER tk_get_tim_impl( SYSTIM *pk_tim )
{
	BEGIN_CRITICAL_SECTION;
	*pk_tim = knl_toSYSTIM(real_time());
	END_CRITICAL_SECTION;

	return E_OK;
}
#endif /* USE_FUNC_TK_GET_TIM */

#ifdef USE_FUNC_TK_GET_OTM
/*
 * Refer system operating time
 */
SYSCALL ER tk_get_otm_impl( SYSTIM *pk_tim )
{
	BEGIN_CRITICAL_SECTION;
	*pk_tim = knl_toSYSTIM(knl_current_time);
	END_CRITICAL_SECTION;

	return E_OK;
}
#endif /* USE_FUNC_TK_GET_OTM */

#if USE_DBGSPT
#ifdef USE_FUNC_TD_GET_TIM
/*
 * Refer system clock
 */
SYSCALL ER td_get_tim_impl( SYSTIM *tim, UW *ofs )
{
	BEGIN_DISABLE_INTERRUPT;
	*ofs = knl_get_hw_timer_nsec();
	*tim = knl_toSYSTIM(real_time());
	END_DISABLE_INTERRUPT;

	return E_OK;
}
#endif /* USE_FUNC_TD_GET_TIM */

#ifdef USE_FUNC_TD_GET_OTM
/*
 * Refer system operating time
 */
SYSCALL ER td_get_otm_impl( SYSTIM *tim, UW *ofs )
{
	BEGIN_DISABLE_INTERRUPT;
	*ofs = knl_get_hw_timer_nsec();
	*tim = knl_toSYSTIM(knl_current_time);
	END_DISABLE_INTERRUPT;

	return E_OK;
}
#endif /* USE_FUNC_TD_GET_OTM */
#endif /* USE_DBGSPT */

/* ------------------------------------------------------------------------ */

#ifdef USE_FUNC_TK_DLY_TSK
/*
 * Definition of task delay wait specification
 */
LOCAL WSPEC knl_wspec_dly = { TTW_DLY, NULL, NULL };

/*
 * Task delay
 */
SYSCALL ER tk_dly_tsk_impl( RELTIM dlytim )
{
	ER	ercd = E_OK;

	CHECK_DISPATCH();

	if ( dlytim > 0 ) {
		BEGIN_CRITICAL_SECTION;
		knl_ctxtsk->wspec = &knl_wspec_dly;
		knl_ctxtsk->wid = 0;
		knl_ctxtsk->wercd = &ercd;
		knl_make_wait_reltim(dlytim, TA_NULL);
		QueInit(&knl_ctxtsk->tskque);
		END_CRITICAL_SECTION;
	}

	return ercd;
}
#endif /* USE_FUNC_TK_DLY_TSK */

/* ------------------------------------------------------------------------ */
/*
 *	Cyclic handler
 */

#if CFN_MAX_CYCID > 0

#ifdef USE_FUNC_CYCCB_TABLE
Noinit(EXPORT CYCCB knl_cyccb_table[NUM_CYCID]);	/* Cyclic handler control block */
Noinit(EXPORT QUEUE	knl_free_cyccb);	/* FreeQue */
#endif /* USE_FUNC_CYCCB_TABLE */


#ifdef USE_FUNC_CYCLICHANDLER_INITIALIZE
/*
 * Initialization of cyclic handler control block
 */
EXPORT ER knl_cyclichandler_initialize( void )
{
	CYCCB	*cyccb, *end;

	/* Get system information */
	if ( NUM_CYCID < 1 ) {
		return E_SYS;
	}

	/* Register all control blocks onto FreeQue */
	QueInit(&knl_free_cyccb);
	end = knl_cyccb_table + NUM_CYCID;
	for ( cyccb = knl_cyccb_table; cyccb < end; cyccb++ ) {
		cyccb->cychdr = NULL; /* Unregistered handler */
		QueInsert((QUEUE*)cyccb, &knl_free_cyccb);
	}

	return E_OK;
}
#endif /* USE_FUNC_CYCLICHANDLER_INITIALIZE */


#ifdef USE_FUNC_CALL_CYCHDR
/*
 * Cyclic handler routine
 */
EXPORT void knl_call_cychdr( CYCCB *cyccb )
{
	/* Set next startup time */
	knl_cyc_timer_insert(cyccb, knl_cyc_next_time(cyccb));

	/* Execute cyclic handler / Enable interrupt nest */
	ENABLE_INTERRUPT_UPTO(TIMER_INTLEVEL);
	CallUserHandlerP1(cyccb->exinf, cyccb->cychdr, cyccb);
	DISABLE_INTERRUPT;
}
#endif /* USE_FUNC_CALL_CYCHDR */

#ifdef USE_FUNC_TK_CRE_CYC
/*
 * Immediate call of cyclic handler 
 */
LOCAL void knl_immediate_call_cychdr( CYCCB *cyccb )
{
	/* Set next startup time */
	knl_cyc_timer_insert(cyccb, knl_cyc_next_time(cyccb));

	/* Execute cyclic handler in task-independent part
	   (Keep interrupt disabled) */
	ENTER_TASK_INDEPENDENT;
	CallUserHandlerP1(cyccb->exinf, cyccb->cychdr, cyccb);
	LEAVE_TASK_INDEPENDENT;
}

/*
 * Create cyclic handler 
 */
SYSCALL ID tk_cre_cyc_impl P1( T_CCYC *pk_ccyc )
{
#if CHK_RSATR
	const ATR VALID_CYCATR = {
		 TA_HLNG
		|TA_STA
		|TA_PHS
		|TA_GP
#if USE_OBJECT_NAME
		|TA_DSNAME
#endif
	};
#endif
	CYCCB	*cyccb;
	LSYSTIM	tm;
	ER	ercd = E_OK;

	CHECK_RSATR(pk_ccyc->cycatr, VALID_CYCATR);
	CHECK_PAR(pk_ccyc->cychdr != NULL);
	CHECK_PAR(pk_ccyc->cyctim > 0);

	BEGIN_CRITICAL_SECTION;
	/* Get control block from FreeQue */
	cyccb = (CYCCB*)QueRemoveNext(&knl_free_cyccb);
	if ( cyccb == NULL ) {
		ercd = E_LIMIT;
		goto error_exit;
	}

	/* Initialize control block */
	cyccb->exinf   = pk_ccyc->exinf;
	cyccb->cycatr  = pk_ccyc->cycatr;
	cyccb->cychdr  = pk_ccyc->cychdr;
	cyccb->cyctim  = pk_ccyc->cyctim;
#if USE_OBJECT_NAME
	if ( (pk_ccyc->cycatr & TA_DSNAME) != 0 ) {
		strncpy((char*)cyccb->name, (char*)pk_ccyc->dsname, OBJECT_NAME_LENGTH);
	}
#endif
#if TA_GP
	if ( (pk_ccyc->cycatr & TA_GP) != 0 ) {
		gp = pk_ccyc->gp;
	}
	cyccb->gp = gp;
#endif

	/* First startup time
	 *	To guarantee the start of handler after the specified time,
	 *	add CFN_TIMER_PERIOD. 
	 */
	tm = ll_add(knl_current_time, uitoll(pk_ccyc->cycphs));
	tm = ll_add(tm, uitoll(CFN_TIMER_PERIOD));

	if ( (pk_ccyc->cycatr & TA_STA) != 0 ) {
		/* Start cyclic handler */
		cyccb->cycstat = TCYC_STA;

		if ( pk_ccyc->cycphs == 0 ) {
			/* Immediate execution */
			cyccb->cyctmeb.time = tm;
			knl_immediate_call_cychdr(cyccb);
		} else {
			/* Register onto timer event queue */
			knl_cyc_timer_insert(cyccb, tm);
		}
	} else {
		/* Initialize only counter */
		cyccb->cycstat = TCYC_STP;
		cyccb->cyctmeb.time = tm;
	}

	ercd = ID_CYC(cyccb - knl_cyccb_table);

    error_exit:
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_CRE_CYC */

#ifdef USE_FUNC_TK_DEL_CYC
/*
 * Delete cyclic handler 
 */
SYSCALL ER tk_del_cyc_impl( ID cycid )
{
	CYCCB	*cyccb;
	ER	ercd = E_OK;

	CHECK_CYCID(cycid);

	cyccb = get_cyccb(cycid);

	BEGIN_CRITICAL_SECTION;
	if ( cyccb->cychdr == NULL ) { /* Unregistered handler */
		ercd = E_NOEXS;
	} else {
		if ( (cyccb->cycstat & TCYC_STA) != 0 ) {
			/* Delete timer event queue */
			knl_timer_delete(&cyccb->cyctmeb);
		}

		/* Return to FreeQue */
		QueInsert((QUEUE*)cyccb, &knl_free_cyccb);
		cyccb->cychdr = NULL; /* Unregistered handler */
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_DEL_CYC */

#ifdef USE_FUNC_TK_STA_CYC
/*
 * Start cyclic handler 
 */
SYSCALL ER tk_sta_cyc_impl( ID cycid )
{
	CYCCB	*cyccb;
	LSYSTIM	tm;
	ER	ercd = E_OK;

	CHECK_CYCID(cycid);

	cyccb = get_cyccb(cycid);

	BEGIN_CRITICAL_SECTION;
	if ( cyccb->cychdr == NULL ) { /* Unregistered handler */
		ercd = E_NOEXS;
		goto error_exit;
	}

	if ( (cyccb->cycatr & TA_PHS) != 0 ) {
		/* Continue cyclic phase */
		if ( (cyccb->cycstat & TCYC_STA) == 0 ) {
			/* Start cyclic handler */
			tm = cyccb->cyctmeb.time;
			if ( ll_cmp(tm, knl_current_time) <= 0 ) {
				tm = knl_cyc_next_time(cyccb);
			}
			knl_cyc_timer_insert(cyccb, tm);
		}
	} else {
		/* Reset cyclic interval */
		if ( (cyccb->cycstat & TCYC_STA) != 0 ) {
			/* Stop once */
			knl_timer_delete(&cyccb->cyctmeb);
		}

		/* FIRST ACTIVATION TIME
		 *	Adjust the first activation time with CFN_TIMER_PERIOD.
		 *	CFN_TIMER_PERIOD is Timer interrupt interval (millisecond).
		 */
		tm = ll_add(knl_current_time, uitoll(cyccb->cyctim));
		tm = ll_add(tm, uitoll(CFN_TIMER_PERIOD));

		/* Start cyclic handler */
		knl_cyc_timer_insert(cyccb, tm);
	}
	cyccb->cycstat |= TCYC_STA;

    error_exit:
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_STA_CYC */

#ifdef USE_FUNC_TK_STP_CYC
/*
 * Stop cyclic handler 
 */
SYSCALL ER tk_stp_cyc_impl( ID cycid )
{
	CYCCB	*cyccb;
	ER	ercd = E_OK;

	CHECK_CYCID(cycid);

	cyccb = get_cyccb(cycid);

	BEGIN_CRITICAL_SECTION;
	if ( cyccb->cychdr == NULL ) { /* Unregistered handler */
		ercd = E_NOEXS;
	} else {
		if ( (cyccb->cycstat & TCYC_STA) != 0 ) {
			/* Stop cyclic handler */
			knl_timer_delete(&cyccb->cyctmeb);
		}
		cyccb->cycstat &= ~TCYC_STA;
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_STP_CYC */

#ifdef USE_FUNC_TK_REF_CYC
/*
 * Refer cyclic handler state
 */
SYSCALL ER tk_ref_cyc_impl( ID cycid, T_RCYC* pk_rcyc )
{
	CYCCB	*cyccb;
	LSYSTIM	tm;
	ER	ercd = E_OK;

	CHECK_CYCID(cycid);

	cyccb = get_cyccb(cycid);

	BEGIN_CRITICAL_SECTION;
	if ( cyccb->cychdr == NULL ) { /* Unregistered handler */
		ercd = E_NOEXS;
	} else {
		tm = cyccb->cyctmeb.time;
		if ( (cyccb->cycstat & TCYC_STA) == 0 ) {
			if ( ll_cmp(tm, knl_current_time) <= 0 ) {
				tm = knl_cyc_next_time(cyccb);
			}
		}
		tm = ll_sub(tm, knl_current_time);
		tm = ll_sub(tm, uitoll(CFN_TIMER_PERIOD));
		if ( ll_sign(tm) < 0 ) {
			tm = ltoll(0);
		}

		pk_rcyc->exinf   = cyccb->exinf;
		pk_rcyc->lfttim  = lltoul(tm);
		pk_rcyc->cycstat = cyccb->cycstat;
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_REF_CYC */

#if USE_DBGSPT

#ifdef USE_FUNC_CYCLICHANDLER_GETNAME
#if USE_OBJECT_NAME
/*
 * Get object name from control block
 */
EXPORT ER knl_cyclichandler_getname(ID id, UB **name)
{
	CYCCB	*cyccb;
	ER	ercd = E_OK;

	CHECK_CYCID(id);

	BEGIN_DISABLE_INTERRUPT;
	cyccb = get_cyccb(id);
	if ( cyccb->cychdr == NULL ) {
		ercd = E_NOEXS;
		goto error_exit;
	}
	if ( (cyccb->cycatr & TA_DSNAME) == 0 ) {
		ercd = E_OBJ;
		goto error_exit;
	}
	*name = cyccb->name;

    error_exit:
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_OBJECT_NAME */
#endif /* USE_FUNC_CYCLICHANDLER_GETNAME */

#ifdef USE_FUNC_TD_LST_CYC
/*
 * Refer cyclic handler usage state
 */
SYSCALL INT td_lst_cyc_impl( ID list[], INT nent )
{
	CYCCB	*cyccb, *end;
	INT	n = 0;

	BEGIN_DISABLE_INTERRUPT;
	end = knl_cyccb_table + NUM_CYCID;
	for ( cyccb = knl_cyccb_table; cyccb < end; cyccb++ ) {
		/* Unregistered handler */
		if ( cyccb->cychdr == NULL ) {
			continue;
		}

		if ( n++ < nent ) {
			*list++ = ID_CYC(cyccb - knl_cyccb_table);
		}
	}
	END_DISABLE_INTERRUPT;

	return n;
}
#endif /* USE_FUNC_TD_LST_CYC */

#ifdef USE_FUNC_TD_REF_CYC
/*
 * Refer cyclic handler state
 */
SYSCALL ER td_ref_cyc_impl( ID cycid, TD_RCYC* pk_rcyc )
{
	CYCCB	*cyccb;
	LSYSTIM	tm;
	ER	ercd = E_OK;

	CHECK_CYCID(cycid);

	cyccb = get_cyccb(cycid);

	BEGIN_DISABLE_INTERRUPT;
	if ( cyccb->cychdr == NULL ) { /* Unregistered handler */
		ercd = E_NOEXS;
	} else {
		tm = cyccb->cyctmeb.time;
		if ( (cyccb->cycstat & TCYC_STA) == 0 ) {
			if ( ll_cmp(tm, knl_current_time) <= 0 ) {
				tm = knl_cyc_next_time(cyccb);
			}
		}
		tm = ll_sub(tm, knl_current_time);
		tm = ll_sub(tm, uitoll(CFN_TIMER_PERIOD));
		if ( ll_sign(tm) < 0 ) {
			tm = ltoll(0);
		}

		pk_rcyc->exinf   = cyccb->exinf;
		pk_rcyc->lfttim  = lltoul(tm);
		pk_rcyc->cycstat = cyccb->cycstat;
	}
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_FUNC_TD_REF_CYC */

#endif /* USE_DBGSPT */
#endif /* CFN_MAX_CYCID */

/* ------------------------------------------------------------------------ */
/*
 *	Alarm handler
 */

#if CFN_MAX_ALMID > 0

#ifdef USE_FUNC_ALMCB_TABLE
Noinit(EXPORT ALMCB knl_almcb_table[NUM_ALMID]);	/* Alarm handler control block */
Noinit(EXPORT QUEUE	knl_free_almcb);	/* FreeQue */
#endif /* USE_FUNC_ALMCB_TABLE */


#ifdef USE_FUNC_ALARMHANDLER_INITIALIZE
/*
 * Initialization of alarm handler control block 
 */
EXPORT ER knl_alarmhandler_initialize( void )
{
	ALMCB	*almcb, *end;

	/* Get system information */
	if ( NUM_ALMID < 1 ) {
		return E_SYS;
	}

	/* Register all control blocks onto FreeQue */
	QueInit(&knl_free_almcb);
	end = knl_almcb_table + NUM_ALMID;
	for ( almcb = knl_almcb_table; almcb < end; almcb++ ) {
		almcb->almhdr = NULL; /* Unregistered handler */
		QueInsert((QUEUE*)almcb, &knl_free_almcb);
	}

	return E_OK;
}
#endif /* USE_FUNC_ALARMHANDLER_INITIALIZE */


#ifdef USE_FUNC_CALL_ALMHDR
/*
 * Alarm handler start routine
 */
EXPORT void knl_call_almhdr( ALMCB *almcb )
{
	almcb->almstat &= ~TALM_STA;

	/* Execute alarm handler/ Enable interrupt nesting */
	ENABLE_INTERRUPT_UPTO(TIMER_INTLEVEL);
	CallUserHandlerP1(almcb->exinf, almcb->almhdr, almcb);
	DISABLE_INTERRUPT;
}
#endif /* USE_FUNC_CALL_ALMHDR */


#ifdef USE_FUNC_TK_CRE_ALM
/*
 * Create alarm handler
 */
SYSCALL ID tk_cre_alm_impl P1( T_CALM *pk_calm )
{
#if CHK_RSATR
	const ATR VALID_ALMATR = {
		 TA_HLNG
		|TA_GP
#if USE_OBJECT_NAME
		|TA_DSNAME
#endif
	};
#endif
	ALMCB	*almcb;
	ER	ercd = E_OK;

	CHECK_RSATR(pk_calm->almatr, VALID_ALMATR);
	CHECK_PAR(pk_calm->almhdr != NULL);

	BEGIN_CRITICAL_SECTION;
	/* Get control block from free queue */
	almcb = (ALMCB*)QueRemoveNext(&knl_free_almcb);
	if ( almcb == NULL ) {
		ercd = E_LIMIT;
		goto error_exit;
	}

	/* Initialize control block */
	almcb->exinf   = pk_calm->exinf;
	almcb->almatr  = pk_calm->almatr;
	almcb->almhdr  = pk_calm->almhdr;
	almcb->almstat = TALM_STP;
#if USE_OBJECT_NAME
	if ( (pk_calm->almatr & TA_DSNAME) != 0 ) {
		strncpy((char*)almcb->name, (char*)pk_calm->dsname, OBJECT_NAME_LENGTH);
	}
#endif
#if TA_GP
	if ( (pk_calm->almatr & TA_GP) != 0 ) {
		gp = pk_calm->gp;
	}
	almcb->gp = gp;
#endif

	ercd = ID_ALM(almcb - knl_almcb_table);

    error_exit:
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_CRE_ALM */

#ifdef USE_FUNC_TK_DEL_ALM
/*
 * Delete alarm handler
 */
SYSCALL ER tk_del_alm_impl( ID almid )
{
	ALMCB	*almcb;
	ER	ercd = E_OK;

	CHECK_ALMID(almid);

	almcb = get_almcb(almid);

	BEGIN_CRITICAL_SECTION;
	if ( almcb->almhdr == NULL ) { /* Unregistered handler */
		ercd = E_NOEXS;
	} else {
		if ( (almcb->almstat & TALM_STA) != 0 ) {
			/* Delete from timer event queue */
			knl_timer_delete(&almcb->almtmeb);
		}

		/* Return to FreeQue */
		QueInsert((QUEUE*)almcb, &knl_free_almcb);
		almcb->almhdr = NULL; /* Unregistered handler */
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_DEL_ALM */

#ifdef USE_FUNC_TK_STA_ALM
/*
 * Alarm handler immediate call
 */
LOCAL void knl_immediate_call_almhdr( ALMCB *almcb )
{
	almcb->almstat &= ~TALM_STA;

	/* Execute alarm handler in task-independent part
	   (Keep interrupt disabled) */
	ENTER_TASK_INDEPENDENT;
	CallUserHandlerP1(almcb->exinf, almcb->almhdr, almcb);
	LEAVE_TASK_INDEPENDENT;
}

/*
 * Start alarm handler
 */
SYSCALL ER tk_sta_alm_impl( ID almid, RELTIM almtim )
{
	ALMCB	*almcb;
	ER	ercd = E_OK;

	CHECK_ALMID(almid);

	almcb = get_almcb(almid);

	BEGIN_CRITICAL_SECTION;
	if ( almcb->almhdr == NULL ) { /* Unregistered handler */
		ercd = E_NOEXS;
		goto error_exit;
	}

	if ( (almcb->almstat & TALM_STA) != 0 ) {
		/* Cancel current settings */
		knl_timer_delete(&almcb->almtmeb);
	}

	if ( almtim > 0 ) {
		/* Register onto timer event queue */
		knl_alm_timer_insert(almcb, almtim);
		almcb->almstat |= TALM_STA;
	} else {
		/* Immediate execution */
		knl_immediate_call_almhdr(almcb);
	}

    error_exit:
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_STA_ALM */

#ifdef USE_FUNC_TK_STP_ALM
/*
 * Stop alarm handler
 */
SYSCALL ER tk_stp_alm_impl( ID almid )
{
	ALMCB	*almcb;
	ER	ercd = E_OK;

	CHECK_ALMID(almid);

	almcb = get_almcb(almid);

	BEGIN_CRITICAL_SECTION;
	if ( almcb->almhdr == NULL ) { /* Unregistered handler */
		ercd = E_NOEXS;
	} else {
		if ( (almcb->almstat & TALM_STA) != 0 ) {
			/* Stop alarm handler address */
			knl_timer_delete(&almcb->almtmeb);
			almcb->almstat &= ~TALM_STA;
		}
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_STP_ALM */

#ifdef USE_FUNC_TK_REF_ALM
/*
 * Refer alarm handler state
 */
SYSCALL ER tk_ref_alm_impl( ID almid, T_RALM *pk_ralm )
{
	ALMCB	*almcb;
	LSYSTIM	tm;
	ER	ercd = E_OK;

	CHECK_ALMID(almid);

	almcb = get_almcb(almid);

	BEGIN_CRITICAL_SECTION;
	if ( almcb->almhdr == NULL ) { /* Unregistered handler */
		ercd = E_NOEXS;
	} else {
		if ( (almcb->almstat & TALM_STA) != 0 ) {
			tm = almcb->almtmeb.time;
			tm = ll_sub(tm, knl_current_time);
			tm = ll_sub(tm, uitoll(CFN_TIMER_PERIOD));
			if ( ll_sign(tm) < 0 ) {
				tm = ltoll(0);
			}
		} else {
			tm = ltoll(0);
		}

		pk_ralm->exinf   = almcb->exinf;
		pk_ralm->lfttim  = lltoul(tm);
		pk_ralm->almstat = almcb->almstat;
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_REF_ALM */

#if USE_DBGSPT

#ifdef USE_FUNC_ALARMHANDLER_GETNAME
#if USE_OBJECT_NAME
/*
 * Get object name from control block
 */
EXPORT ER knl_alarmhandler_getname(ID id, UB **name)
{
	ALMCB	*almcb;
	ER	ercd = E_OK;

	CHECK_ALMID(id);

	BEGIN_DISABLE_INTERRUPT;
	almcb = get_almcb(id);
	if ( almcb->almhdr == NULL ) {
		ercd = E_NOEXS;
		goto error_exit;
	}
	if ( (almcb->almatr & TA_DSNAME) == 0 ) {
		ercd = E_OBJ;
		goto error_exit;
	}
	*name = almcb->name;

    error_exit:
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_OBJECT_NAME */
#endif /* USE_FUNC_ALARMHANDLER_GETNAME */

#ifdef USE_FUNC_TD_LST_ALM
/*
 * Refer alarm handler usage state
 */
SYSCALL INT td_lst_alm_impl( ID list[], INT nent )
{
	ALMCB	*almcb, *end;
	INT	n = 0;

	BEGIN_DISABLE_INTERRUPT;
	end = knl_almcb_table + NUM_ALMID;
	for ( almcb = knl_almcb_table; almcb < end; almcb++ ) {
		/* Unregistered handler */
		if ( almcb->almhdr == NULL ) {
			continue;
		}

		if ( n++ < nent ) {
			*list++ = ID_ALM(almcb - knl_almcb_table);
		}
	}
	END_DISABLE_INTERRUPT;

	return n;
}
#endif /* USE_FUNC_TD_LST_ALM */

#ifdef USE_FUNC_TD_REF_ALM
/*
 * Refer alarm handler state
 */
SYSCALL ER td_ref_alm_impl( ID almid, TD_RALM *pk_ralm )
{
	ALMCB	*almcb;
	LSYSTIM	tm;
	ER	ercd = E_OK;

	CHECK_ALMID(almid);

	almcb = get_almcb(almid);

	BEGIN_DISABLE_INTERRUPT;
	if ( almcb->almhdr == NULL ) { /* Unregistered handler */
		ercd = E_NOEXS;
	} else {
		if ( (almcb->almstat & TALM_STA) != 0 ) {
			tm = almcb->almtmeb.time;
			tm = ll_sub(tm, knl_current_time);
			tm = ll_sub(tm, uitoll(CFN_TIMER_PERIOD));
			if ( ll_sign(tm) < 0 ) {
				tm = ltoll(0);
			}
		} else {
			tm = ltoll(0);
		}

		pk_ralm->exinf   = almcb->exinf;
		pk_ralm->lfttim  = lltoul(tm);
		pk_ralm->almstat = almcb->almstat;
	}
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_FUNC_TD_REF_ALM */

#endif /* USE_DBGSPT */
#endif /* CFN_MAX_ALMID */
