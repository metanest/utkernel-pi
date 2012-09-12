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
 *	objname.c
 *	Object name support
 */

/** [BEGIN Common Definitions] */
#include "kernel.h"
#include <libstr.h>
/** [END Common Definitions] */

#if USE_DBGSPT

#if USE_OBJECT_NAME
#ifdef USE_FUNC_OBJECT_GETNAME
EXPORT ER knl_object_getname( UINT objtype, ID objid, UB **name)
{
	ER	ercd;

	switch (objtype) {
#if CFN_MAX_TSKID > 0
	  case TN_TSK:
		{
			IMPORT ER knl_task_getname(ID id, UB **name);
			ercd = knl_task_getname(objid, name);
			break;
		}
#endif

#if CFN_MAX_SEMID > 0
	  case TN_SEM:
		{
			IMPORT ER knl_semaphore_getname(ID id, UB **name);
			ercd = knl_semaphore_getname(objid, name);
			break;
		}
#endif

#if CFN_MAX_FLGID > 0
	  case TN_FLG:
		{
			IMPORT ER knl_eventflag_getname(ID id, UB **name);
			ercd = knl_eventflag_getname(objid, name);
			break;
		}
#endif

#if CFN_MAX_MBXID > 0
	  case TN_MBX:
		{
			IMPORT ER knl_mailbox_getname(ID id, UB **name);
			ercd = knl_mailbox_getname(objid, name);
			break;
		}
#endif

#if CFN_MAX_MBFID > 0
	  case TN_MBF:
		{
			IMPORT ER knl_messagebuffer_getname(ID id, UB **name);
			ercd = knl_messagebuffer_getname(objid, name);
			break;
		}
#endif

#if CFN_MAX_PORID > 0
	  case TN_POR:
		{
			IMPORT ER knl_rendezvous_getname(ID id, UB **name);
			ercd = knl_rendezvous_getname(objid, name);
			break;
		}
#endif

#if CFN_MAX_MTXID > 0
	  case TN_MTX:
		{
			IMPORT ER knl_mutex_getname(ID id, UB **name);
			ercd = knl_mutex_getname(objid, name);
			break;
		}
#endif

#if CFN_MAX_MPLID > 0
	  case TN_MPL:
		{
			IMPORT ER knl_memorypool_getname(ID id, UB **name);
			ercd = knl_memorypool_getname(objid, name);
			break;
		}
#endif

#if CFN_MAX_MPFID > 0
	  case TN_MPF:
		{
			IMPORT ER knl_fix_memorypool_getname(ID id, UB **name);
			ercd = knl_fix_memorypool_getname(objid, name);
			break;
		}
#endif

#if CFN_MAX_CYCID > 0
	  case TN_CYC:
		{
			IMPORT ER knl_cyclichandler_getname(ID id, UB **name);
			ercd = knl_cyclichandler_getname(objid, name);
			break;
		}
#endif

#if CFN_MAX_ALMID > 0
	  case TN_ALM:
		{
			IMPORT ER knl_alarmhandler_getname(ID id, UB **name);
			ercd = knl_alarmhandler_getname(objid, name);
			break;
		}
#endif

	  default:
		ercd = E_PAR;

	}

	return ercd;
}
#endif /* USE_FUNC_OBJECT_GETNAME */
#endif /* USE_OBJECT_NAME */

#ifdef USE_FUNC_TD_REF_DSNAME
#if USE_OBJECT_NAME
IMPORT ER knl_object_getname( UINT objtype, ID objid, UB **name);
#endif /* USE_OBJECT_NAME */

SYSCALL ER td_ref_dsname_impl( UINT type, ID id, UB *dsname )
{
#if USE_OBJECT_NAME
	UB	*name_cb;
	ER	ercd;

	ercd = knl_object_getname(type, id, &name_cb);
	if (ercd == E_OK) {
		strncpy((char*)dsname, (char*)name_cb, OBJECT_NAME_LENGTH);
	}

	return ercd;
#else
	return E_NOSPT;
#endif /* USE_OBJECT_NAME */
}
#endif /* USE_FUNC_TD_REF_DSNAME */

#ifdef USE_FUNC_TD_SET_DSNAME
#if USE_OBJECT_NAME
IMPORT ER knl_object_getname( UINT objtype, ID objid, UB **name);
#endif /* USE_OBJECT_NAME */

SYSCALL ER td_set_dsname_impl( UINT type, ID id, UB *dsname )
{
#if USE_OBJECT_NAME
	UB	*name_cb;
	ER	ercd;

	ercd = knl_object_getname(type, id, &name_cb);
	if (ercd == E_OK) {
		strncpy((char*)name_cb, (char*)dsname, OBJECT_NAME_LENGTH);
	}

	return ercd;
#else
	return E_NOSPT;
#endif /* USE_OBJECT_NAME */
}
#endif /* USE_FUNC_TD_SET_DSNAME */

#endif /* USE_DBGSPT */
