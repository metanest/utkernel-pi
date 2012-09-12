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
 *	time_calls.h
 *	Time Management Function
 */

#ifndef _TIME_CALLS_H
#define _TIME_CALLS_H

/*
 * Cyclic handler control block
 */
typedef struct cyclic_handler_control_block {
	VP	exinf;		/* Extended information */
	ATR	cycatr;		/* Cyclic handler attribute */
	FP	cychdr;		/* Cyclic handler address */
	UINT	cycstat;	/* Cyclic handler state */
	RELTIM	cyctim;		/* Cyclic time */
	TMEB	cyctmeb;	/* Timer event block */
#if TA_GP
	VP	gp;		/* Global pointer */
#endif
#if USE_OBJECT_NAME
	UB	name[OBJECT_NAME_LENGTH];	/* name */
#endif
} CYCCB;

IMPORT CYCCB	knl_cyccb_table[];	/* Cyclic handler control block */
IMPORT QUEUE	knl_free_cyccb;	/* FreeQue */

#define get_cyccb(id)	( &knl_cyccb_table[INDEX_CYC(id)] )


/*
 * Next startup time
 */
Inline LSYSTIM knl_cyc_next_time( CYCCB *cyccb )
{
	LSYSTIM		tm;
	longlong	n;

	tm = ll_add(cyccb->cyctmeb.time, uitoll(cyccb->cyctim));

	if ( ll_cmp(tm, knl_current_time) <= 0 ) {

		/* Adjust time to be later than current time */
		tm = ll_sub(knl_current_time, cyccb->cyctmeb.time);
		n  = lui_div(tm, cyccb->cyctim);
		ll_inc(&n);
		tm = lui_mul(n, cyccb->cyctim);
		tm = ll_add(cyccb->cyctmeb.time, tm);
	}

	return tm;
}

IMPORT void knl_call_cychdr( CYCCB* cyccb );

/*
 * Register timer event queue
 */
Inline void knl_cyc_timer_insert( CYCCB *cyccb, LSYSTIM tm )
{
	knl_timer_insert_abs(&cyccb->cyctmeb, tm, (CBACK)knl_call_cychdr, cyccb);
}


/*
 * Alarm handler control block
 */
typedef struct alarm_handler_control_block {
	VP	exinf;		/* Extended information */
	ATR	almatr;		/* Alarm handler attribute */
	FP	almhdr;		/* Alarm handler address */
	UINT	almstat;	/* Alarm handler state */
	TMEB	almtmeb;	/* Timer event block */
#if TA_GP
	VP	gp;		/* Global pointer */
#endif
#if USE_OBJECT_NAME
	UB	name[OBJECT_NAME_LENGTH];	/* name */
#endif
} ALMCB;
/** [END Common Definitions] */

IMPORT ALMCB	knl_almcb_table[];	/* Alarm handler control block */
IMPORT QUEUE	knl_free_almcb;	/* FreeQue */

#define get_almcb(id)	( &knl_almcb_table[INDEX_ALM(id)] )

IMPORT void knl_call_almhdr( ALMCB *almcb );

/*
 * Register onto timer event queue
 */
Inline void knl_alm_timer_insert( ALMCB *almcb, RELTIM reltim )
{
	LSYSTIM	tm;

	/* To guarantee to start the handler after the specified time, 
	   add CFN_TIMER_PERIOD */
	tm = ll_add(knl_current_time, uitoll(reltim));
	tm = ll_add(tm, uitoll(CFN_TIMER_PERIOD));

	knl_timer_insert_abs(&almcb->almtmeb, tm, (CBACK)knl_call_almhdr, almcb);
}


#endif /* _TIME_CALLS_H */
