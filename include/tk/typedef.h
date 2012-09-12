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
 *	@(#)typedef.h (T-Kernel)
 *
 *	T-Kernel Standard Data Type Definition
 */

#ifndef	__TK_TYPEDEF_H__
#define __TK_TYPEDEF_H__

#include <basic.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Data type in which meaning is defined in T-Kernel/OS specification 
 */
typedef INT		FN;		/* Function code */
typedef INT		RNO;		/* Rendezvous number */
typedef UW		ATR;		/* Object/handler attribute */
typedef INT		ER;		/* Error code */
typedef INT		PRI;		/* Priority */
typedef W		TMO;		/* Time out setting */
typedef UW		RELTIM;		/* Relative time */

typedef struct systim {			/* System time */
	W	hi;			/* Upper 32 bits */
	UW	lo;			/* Lower 32 bits */
} SYSTIM;

/*
 * Common constant
 */
#define NULL		0		/* Invalid address */
#define TA_NULL		0U		/* No special attributes indicated */
#define TMO_POL		0		/* Polling */
#define TMO_FEVR	(-1)		/* Permanent wait */

/* ------------------------------------------------------------------------ */

/*
 * 64 bits value
 */
typedef struct dw {
#if BIGENDIAN
	W	hi;	/* Upper 32 bits */
	UW	lo;	/* Lower 32 bits */
#else
	UW	lo;	/* Lower 32 bits */
	W	hi;	/* Upper 32 bits */
#endif
} DW;

#ifdef __cplusplus
}
#endif
#endif /* __TK_TYPEDEF_H__ */
