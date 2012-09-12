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
 *	subsystem.c
 *	Subsystem Manager
 */

/** [BEGIN Common Definitions] */
#include "kernel.h"
#include "task.h"
#include "check.h"
#include "bitop.h"
#include "subsystem.h"
/** [END Common Definitions] */

#if CFN_MAX_SSYID > 0


#ifdef USE_FUNC_SSYCB_TABLE
Noinit(EXPORT SSYCB knl_ssycb_table[NUM_SSYID]);	/* Subsystem control block */
#endif /* USE_FUNC_SSYCB_TABLE */


#ifdef USE_FUNC_SUBSYSTEM_INITIALIZE
/*
 * Initialization of subsystem control block
 */
EXPORT ER knl_subsystem_initialize( void )
{
	INT	i;

	/* Get system information */
	if ( NUM_SSYID < 1 ) {
		return E_SYS;
	}
	if ( NUM_SSYPRI < 1 ) {
		return E_SYS;
	}

	for ( i = 0; i < NUM_SSYID; i++ ) {
		knl_ssycb_table[i].svchdr    = knl_no_support;
	}

	return E_OK;
}
#endif /* USE_FUNC_SUBSYSTEM_INITIALIZE */


#ifdef USE_FUNC_TK_DEF_SSY
/*
 * Definition of subsystem
 */
SYSCALL ER tk_def_ssy_impl P2( ID ssid, T_DSSY *pk_dssy )
{
	SSYCB	*ssycb;
	ER	ercd = E_OK;

	CHECK_SSYID(ssid);
#if CHK_RSATR
	if ( pk_dssy != NULL ) {
		CHECK_RSATR(pk_dssy->ssyatr, TA_NULL|TA_GP);
	}
#endif

	ssycb = get_ssycb(ssid);

	BEGIN_CRITICAL_SECTION;
	if ( pk_dssy != NULL ) {
		/* Register */
		if ( ssycb->svchdr != knl_no_support ) {
			ercd = E_OBJ;  /* Registered */
			goto error_exit;
		}
		ssycb->svchdr    = (SVC)pk_dssy->svchdr;
#if TA_GP
		if ( (pk_dssy->ssyatr & TA_GP) != 0 ) {
			gp = pk_dssy->gp;
		}
		ssycb->gp = gp;
#endif

	} else {
		/* Delete */
		if ( ssycb->svchdr == knl_no_support ) {
			ercd = E_NOEXS;  /* Not registered */
			goto error_exit;
		}

		ssycb->svchdr    = knl_no_support;
	}

    error_exit:
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_DEF_SSY */

#ifdef USE_FUNC_TK_REF_SSY
/*
 * Refer subsystem definition information
 */
SYSCALL ER tk_ref_ssy_impl( ID ssid, T_RSSY *pk_rssy )
{
	SSYCB	*ssycb;
	ER	ercd = E_OK;

	CHECK_SSYID(ssid);

	ssycb = get_ssycb(ssid);

	BEGIN_CRITICAL_SECTION;
	if ( ssycb->svchdr == knl_no_support ) {
		ercd = E_NOEXS;
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_REF_SSY */

#if USE_DBGSPT

#ifdef USE_FUNC_TD_LST_SSY
/*
 * Refer subsystem usage state
 */
SYSCALL INT td_lst_ssy_impl( ID list[], INT nent )
{
	SSYCB	*ssycb, *end;
	INT	n = 0;

	BEGIN_DISABLE_INTERRUPT;
	end = knl_ssycb_table + NUM_SSYID;
	for ( ssycb = knl_ssycb_table; ssycb < end; ssycb++ ) {
		if ( ssycb->svchdr == knl_no_support ) {
			continue;
		}

		if ( n++ < nent ) {
			*list++ = ID_SSY(ssycb - knl_ssycb_table);
		}
	}
	END_DISABLE_INTERRUPT;

	return n;
}
#endif /* USE_FUNC_TD_LST_SSY */

#ifdef USE_FUNC_TD_REF_SSY
/*
 * Refer subsystem definition information
 */
SYSCALL ER td_ref_ssy_impl( ID ssid, TD_RSSY *pk_rssy )
{
	SSYCB	*ssycb;
	ER	ercd = E_OK;

	CHECK_SSYID(ssid);

	ssycb = get_ssycb(ssid);

	BEGIN_DISABLE_INTERRUPT;
	if ( ssycb->svchdr == knl_no_support ) {
		ercd = E_NOEXS;
	}
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_FUNC_TD_REF_SSY */

#endif /* USE_DBGSPT */

#ifdef USE_FUNC_SVC_IENTRY
/*
 * Branch routine to extended SVC handler
 */
EXPORT ER knl_svc_ientry P2GP( VP pk_para, FN fncd )
{
	ID	ssid;
	SSYCB	*ssycb;
	ER	ercd;

	/* Lower 8 bits are subsystem ID */
	ssid = fncd & 0xff;
	if ( ssid < MIN_SSYID || ssid > MAX_SSYID ) {
		return E_RSFN;
	}

	ssycb = get_ssycb(ssid);

	if ( in_indp() ) {
		/* Execute at task-independent part */
		ercd = CallUserHandlerP2_GP(pk_para, fncd,
						ssycb->svchdr, ssycb);
	} else {
		DISABLE_INTERRUPT;
		knl_ctxtsk->sysmode++;
		ENABLE_INTERRUPT;

		/* Call extended SVC handler */
		ercd = CallUserHandlerP2_GP(pk_para, fncd,
						ssycb->svchdr, ssycb);

		DISABLE_INTERRUPT;
		knl_ctxtsk->sysmode--;
		ENABLE_INTERRUPT;
	}

	return ercd;
}
#endif /* USE_FUNC_SVC_IENTRY */

#endif /* CFN_MAX_SSYID */
