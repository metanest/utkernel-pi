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
 *	@(#)errno.h
 *
 *	micro T-Kernel Error Code
 */

#ifndef __TK_ERRNO_H__
#define __TK_ERRNO_H__

#if 0
/*
 * To port a program using macros concerning error codes from T-Kernel
 * into micro T-Kernel, you may modify and use following macros (ERCD, 
 * MERCD, SERCD).
 */
#ifndef _in_asm_source_
#include <tk/typedef.h>

#define MERCD(er)	( (ER)(er) >> 16 )	/* Main error code */
#define SERCD(er)	( (H)(er) )		/* Sub-error code */
#define ERCD(mer, ser)	( (ER)(((UW)(mer) << 16) | ((UW)(ser) & 0x0000FFFF)) )
#else
#define ERCD(mer, ser)	( ((mer) << 16) | ((ser) & 0xffff) )
#endif /* _in_asm_source_ */
#endif

#define E_OK		(0)	/* Completed successfully */

#define E_SYS		(-5)	/* System error */
#define E_NOCOP		(-6)	/* Coprocessor disable */
#define E_NOSPT		(-9)	/* Unsupported function */
#define E_RSFN		(-10)	/* Reserved function code number */
#define E_RSATR		(-11)	/* Reserved attribute */
#define E_PAR		(-17)	/* Parameter error */
#define E_ID		(-18)	/* Incorrect ID number */
#define E_CTX		(-25)	/* Context error */
#define E_MACV		(-26)	/* Inaccessible memory/access violation */
#define E_OACV		(-27)	/* Object access violation */
#define E_ILUSE		(-28)	/* Incorrect system call use */
#define E_NOMEM		(-33)	/* Insufficient memory */
#define E_LIMIT		(-34)	/* Exceed system limits */
#define E_OBJ		(-41)	/* Incorrect object state */
#define E_NOEXS		(-42)	/* Object does not exist */
#define E_QOVR		(-43)	/* Queuing overflow */
#define E_RLWAI		(-49)	/* Forcibly release wait state */
#define E_TMOUT		(-50)	/* Polling fail/time out */
#define E_DLT		(-51)	/* Waited object was deleted */
#define E_DISWAI	(-52)	/* Release wait caused by wait disable */

#define E_IO		(-57)	/* Output/input error */
#define E_NOMDA		(-58)	/* No media */
#define E_BUSY		(-65)	/* Busy state */
#define E_ABORT		(-66)	/* Aborted */
#define E_RONLY		(-67)	/* Write protected */

#endif /* __TK_ERRNO_H__ */
