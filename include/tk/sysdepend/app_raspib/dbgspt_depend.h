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
 *	@(#)dbgspt_depend.h (tk/AT91)
 *
 *	micro T-Kernel Debugger Support System-Dependent Definition (AT91M55800A)
 */

#ifndef __TK_DBGSPT_DEPEND_H__
#define __TK_DBGSPT_DEPEND_H__

#ifdef __cplusplus
extern "C" {
#endif

/*
 * System call/extension SVC caller information 
 */
typedef struct td_calinf {
	VP	ssp;		/* System stack pointer */
	VP	r11;		/* Frame pointer when calling */
} TD_CALINF;

#ifdef __cplusplus
}
#endif
#endif /* __TK_DBGSPT_DEPEND_H__ */
