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
 * modified for Raspberry Pi Type B by Makoto Kishimoto
 */

/*
 *	tkdev_timer.h (AT91)
 *	Hardware-Dependent Timer Processing
 */

#ifndef _TKDEV_TIMER_
#define _TKDEV_TIMER_

#include <stdint.h>

#include <tk/syslib.h>
#include <sys/sysinfo.h>
#include "tkdev_conf.h"

#if USE_TRAP
#define ENAINT	Asm("msr cpsr_c, %0":: "i"(PSR_SVC))
#define DISINT	Asm("msr cpsr_c, %0":: "i"(PSR_SVC|PSR_DI))
#else
#define ENAINT	Asm("msr cpsr_c, %0":: "i"(PSR_IRQ))
#define DISINT	Asm("msr cpsr_c, %0":: "i"(PSR_IRQ|PSR_DI))
#endif

/*
 * Settable interval range (millisecond)
 */
#define MIN_TIMER_PERIOD	1
#define MAX_TIMER_PERIOD	50

static uint32_t knl_timer_interval;
static uint32_t knl_timer_next;

/*
 * Set timer
 */
Inline void knl_init_hw_timer( void )
{
	knl_timer_interval = CFN_TIMER_PERIOD * TMCLK / 1000;

	/* set first wakeup */
	knl_timer_next = in_w(0x20003004) + knl_timer_interval;
	out_w(0x20003010, knl_timer_next);
}

/*
 * Timer start processing
 *	Initialize the timer and start the periodical timer interrupt.
 */
Inline void knl_start_hw_timer( void )
{
IMPORT	void	knl_timer_handler_startup( void );

	/* Set timer */
	knl_init_hw_timer();

	/* Interrupt handler definition */
	knl_define_inthdr(1, knl_timer_handler_startup);

	/* Timer interrupt enable */
	EnableInt(1);
}

/*
 * Clear timer interrupt
 *	Clear the timer interrupt request. Depending on the type of
 *	hardware, there are two timings for clearing: at the beginning
 *	and the end of the interrupt handler.
 *	'clear_hw_timer_interrupt()' is called at the beginning of the
 *	timer interrupt handler.
 *	'end_of_hw_timer_interrupt()' is called at the end of the timer
 *	interrupt handler.
 *	Use either or both according to hardware.
 */
Inline void knl_clear_hw_timer_interrupt( void )
{
	/* set next wakeup */
	knl_timer_next += knl_timer_interval;
	out_w(0x20003010, knl_timer_next);

	/* Timer interrupt disable */
	DisableInt(1);

	/* Clear timer interrupt */
	out_w(0x20003000, 0x00000002);

	/* Multiplexed interrupt enable */
	ENAINT;
}

Inline void knl_end_of_hw_timer_interrupt( void )
{
	/* Interrupt disable */
	DISINT;

	/* Timer interrupt enable */
	EnableInt(1);
}

/*
 * Timer stop processing
 *	Stop the timer operation.
 *	Called when system stops.
 */
Inline void knl_terminate_hw_timer( void )
{
	/* Timer interrupt disable */
	DisableInt(1);
}

/*
 * Get processing time from the previous timer interrupt to the
 * current (nanosecond)
 *	Consider the possibility that the timer interrupt occurred
 *	during the interrupt disable and calculate the processing time
 *	within the following
 *	range: 0 <= Processing time < CFN_TIMER_PERIOD * 2
 */
Inline UW knl_get_hw_timer_nsec( void )
{
	return (in_w(0x20003004) - knl_timer_next + knl_timer_interval) * 1000;
}

#endif /* _TKDEV_TIMER_ */
