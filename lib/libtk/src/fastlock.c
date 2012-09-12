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
 *	@(#)fastlock.c (libtk)
 *
 *	High-speed exclusive control lock 
 */

/** [BEGIN Common Definitions] */
#include <basic.h>
#include <tk/tkernel.h>
#include <tk/util.h>
#include <libstr.h>
#include "libtk_config.h"

/* ------------------------------------------------------------------------ */
/*
 *	Inc	Increment cnt, in result if cnt >  0, returns positive value.
 *		If cnt <= 0, returns 0 or negative value.
 *	Dec	Decrement cnt, in result if cnt >= 0, returns positive value.
 *		If cnt <  0, returns 0 or negative value. 
 *	Increment/Decrement and evaluation of the associated result must
 *	be executed exclusively.
 */

Inline INT Inc( FastLock *lock )
{
	UINT	imask;
	INT	c;
	DI(imask);
	c = ++lock->cnt;
	EI(imask);
	return c;
}
Inline INT Dec( FastLock *lock )
{
	UINT	imask;
	INT	c;
	DI(imask);
	c = lock->cnt--;
	EI(imask);
	return c;
}

/* ------------------------------------------------------------------------ */
/** [END Common Definitions] */

#ifdef USE_FUNC_LOCK
/*
 * Lock 
 */
EXPORT void Lock( FastLock *lock )
{
	if ( Inc(lock) > 0 ) {
		tk_wai_sem(lock->id, 1, TMO_FEVR);
	}
}
#endif /* USE_FUNC_LOCK */

#ifdef USE_FUNC_UNLOCK
/*
 * Lock release
 */
EXPORT void Unlock( FastLock *lock )
{
	if ( Dec(lock) > 0 ) {
		tk_sig_sem(lock->id, 1);
	}
}
#endif /* USE_FUNC_UNLOCK */

#ifdef USE_FUNC_CREATELOCK
/*
 * Create high-speed lock 
 */
EXPORT ER CreateLock( FastLock *lock, UB *name )
{
	T_CSEM	csem;
	ER	ercd;

	if ( name == NULL ) {
		csem.exinf = NULL;
	} else {
		strncpy((char*)&csem.exinf, (char*)name, sizeof(csem.exinf));
	}
	csem.sematr  = TA_TPRI;
	csem.isemcnt = 0;
	csem.maxsem  = 1;

	ercd = tk_cre_sem(&csem);
	if ( ercd < E_OK ) {
		return ercd;
	}

	lock->id = ercd;
	lock->cnt = -1;

	return E_OK;
}
#endif /* USE_FUNC_CREATELOCK */

#ifdef USE_FUNC_DELETELOCK
/*
 * Delete high-speed lock
 */
EXPORT void DeleteLock( FastLock *lock )
{
	if ( lock->id > 0 ) {
		tk_del_sem(lock->id);
	}
	lock->id = 0;
}
#endif /* USE_FUNC_DELETELOCK */
