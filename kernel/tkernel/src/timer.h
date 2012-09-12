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
 *	timer.h
 *	Timer Module Definition
 */

#ifndef _TIMER_
#define _TIMER_

#include <longlong.h>

/*
 * SYSTIM internal expression and conversion
 */
typedef	longlong	LSYSTIM;	/* SYSTIM int. expression */

Inline LSYSTIM knl_toLSYSTIM( SYSTIM *time )
{
	LSYSTIM		ltime;

	hilo_ll(ltime, time->hi, time->lo);

	return ltime;
}

Inline SYSTIM knl_toSYSTIM( LSYSTIM ltime )
{
	SYSTIM		time;

	ll_hilo(time.hi, time.lo, ltime);

	return time;
}

/*
 * Definition of timer event block 
 */
typedef void	(*CBACK)(VP);	/* Type of callback function */

typedef struct timer_event_block {
	QUEUE	queue;		/* Timer event queue */
	LSYSTIM	time;		/* Event time */
	CBACK	callback;	/* Callback function */
	VP	arg;		/* Argument to be sent to callback function */
} TMEB;

/*
 * Current time (Software clock)
 */
IMPORT LSYSTIM	knl_current_time;	/* System operation time */
IMPORT LSYSTIM	knl_real_time_ofs;	/* Difference from actual time */

/*
 * Timer event queue
 */
IMPORT QUEUE	knl_timer_queue;

/* Actual time */
#define real_time()	( ll_add(knl_current_time, knl_real_time_ofs) )

/*
 * Timer initialization and stop
 */
IMPORT ER   knl_timer_initialize( void );
IMPORT void knl_timer_shutdown( void );

/*
 * Register timer event onto timer queue
 */
IMPORT void knl_timer_insert( TMEB *evt, TMO tmout, CBACK cback, VP arg );
IMPORT void knl_timer_insert_reltim( TMEB *event, RELTIM tmout, CBACK callback, VP arg );
IMPORT void knl_timer_insert_abs( TMEB *evt, LSYSTIM time, CBACK cback, VP arg );

/*
 * Delete from timer queue
 */
Inline void knl_timer_delete( TMEB *event )
{
	QueRemove(&event->queue);
}

#endif /* _TIMER_ */
