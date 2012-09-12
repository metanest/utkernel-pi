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
 *	inittask_def.c
 *	Initial task definition
 */

#include "inittask_def.h"

IMPORT void knl_init_task(void);

/*
 * Initial task creation parameter
 */
EXPORT const T_CTSK knl_c_init_task = {
	(VP)INITTASK_EXINF,	/* exinf */
	INITTASK_TSKATR,	/* tskatr */
	(FP)&knl_init_task,	/* task */
	INITTASK_ITSKPRI,	/* itskpri */
	INITTASK_STKSZ,		/* stksz */
	INITTASK_DSNAME,	/* dsname */
	INITTASK_STACK,		/* bufptr */
};
