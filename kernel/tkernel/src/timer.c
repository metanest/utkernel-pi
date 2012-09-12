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
 *	timer.c
 *	Timer Control
 */

#include "kernel.h"
#include "timer.h"
#include "task.h"
#include "tkdev_timer.h"

/*
 * Current time (Software clock)
 *	'current_time' shows the total operation time since
 *	operating system Starts. 'real_time_ofs' shows difference
 *	between the current time and the operating system clock
 *	(current_time). Do not change 'current_time' when setting
 *	time by 'set_tim()'. Set 'real_time_ofs' with the time  	 
 *   	difference between 'current_time' and setup time.
 *	Therefore 'current_time' does not affect with time change
 *	and it increases simply.
 */
Noinit(EXPORT LSYSTIM	knl_current_time);	/* System operation time */
Noinit(EXPORT LSYSTIM	knl_real_time_ofs);	/* Actual time - System operation time */

/* 
 * Timer event queue
 */
Noinit(EXPORT QUEUE	knl_timer_queue);

/*
 * Initialization of timer module
 */
EXPORT ER knl_timer_initialize( void )
{
	if ( CFN_TIMER_PERIOD < MIN_TIMER_PERIOD
	  || CFN_TIMER_PERIOD > MAX_TIMER_PERIOD ) {
		return E_SYS;
	}

	knl_current_time = knl_real_time_ofs = ltoll(0);
	QueInit(&knl_timer_queue);

	/* Start timer interrupt */
	knl_start_hw_timer();

	return E_OK;
}

#if USE_CLEANUP
/*
 * Stop timer
 */
EXPORT void knl_timer_shutdown( void )
{
	knl_terminate_hw_timer();
}
#endif /* USE_CLEANUP */


/*
 * Insert timer event to timer event queue
 */
LOCAL void knl_enqueue_tmeb( TMEB *event )
{
	QUEUE	*q;

	for ( q = knl_timer_queue.next; q != &knl_timer_queue; q = q->next ) {
		if ( ll_cmp(event->time, ((TMEB*)q)->time) < 0) {
			break;
		}
	}
	QueInsert(&event->queue, q);
}

/*
 * Set timeout event
 *	Register the timer event 'event' onto the timer queue to
 *	start after the timeout 'tmout'. At timeout, start with the
 *	argument 'arg' on the callback function 'callback'.
 *	When 'tmout' is TMO_FEVR, do not register onto the timer
 *	queue, but initialize queue area in case 'timer_delete' 
 *	is called later.
 *
 *	"include/tk/typedef.h"
 *	typedef	W		TMO;
 *	typedef UW		RELTIM;
 *	#define TMO_FEVR	(-1)
 */
EXPORT void knl_timer_insert( TMEB *event, TMO tmout, CBACK callback, VP arg )
{
	event->callback = callback;
	event->arg = arg;

	if ( tmout == TMO_FEVR ) {
		QueInit(&event->queue);
	} else {
		/* To guarantee longer wait time specified by 'tmout',
		   add TIMER_PERIOD on wait time */
		event->time = ll_add( ll_add(knl_current_time, ltoll(tmout)),
					uitoll(CFN_TIMER_PERIOD) );
		knl_enqueue_tmeb(event);
	}
}

EXPORT void knl_timer_insert_reltim( TMEB *event, RELTIM tmout, CBACK callback, VP arg )
{
	event->callback = callback;
	event->arg = arg;

	/* To guarantee longer wait time specified by 'tmout',
	   add TIMER_PERIOD on wait time */
	event->time = ll_add( ll_add(knl_current_time, ultoll(tmout)),
				uitoll(CFN_TIMER_PERIOD) );
	knl_enqueue_tmeb(event);
}

/*
 * Set time specified event
 *	Register the timer event 'evt' onto the timer queue to start at the 
 *	(absolute) time 'time'.
 *	'time' is not an actual time. It is system operation time.
 */
EXPORT void knl_timer_insert_abs( TMEB *evt, LSYSTIM time, CBACK cback, VP arg )
{
	evt->callback = cback;
	evt->arg = arg;
	evt->time = time;
	knl_enqueue_tmeb(evt);
}

/* ------------------------------------------------------------------------ */

/*
 * Timer interrupt handler
 *	Timer interrupt handler starts every TIMER_PERIOD millisecond 
 *	interval by hardware timer. Update the software clock and start the 
 *	timer event upon arriving at start time.
 */

EXPORT void knl_timer_handler( void )
{
	TMEB	*event;

	knl_clear_hw_timer_interrupt();		/* Clear timer interrupt */

	BEGIN_CRITICAL_SECTION;
	knl_current_time = ll_add(knl_current_time, uitoll(CFN_TIMER_PERIOD));

#if USE_DBGSPT && defined(USE_FUNC_TD_INF_TSK)
	if ( knl_ctxtsk != NULL ) {
		/* Task at execution */
		if ( knl_ctxtsk->sysmode > 0 ) {
			knl_ctxtsk->stime += CFN_TIMER_PERIOD;
		} else {
			knl_ctxtsk->utime += CFN_TIMER_PERIOD;
		}
	}
#endif

	/* Execute event that passed occurring time. */
	while ( !isQueEmpty(&knl_timer_queue) ) {
		event = (TMEB*)knl_timer_queue.next;

		if ( ll_cmp(event->time, knl_current_time) > 0 ) {
			break;
		}

		QueRemove(&event->queue);
		if ( event->callback != NULL ) {
			(*event->callback)(event->arg);
		}
	}

	END_CRITICAL_SECTION;

	knl_end_of_hw_timer_interrupt();		/* Clear timer interrupt */
}
