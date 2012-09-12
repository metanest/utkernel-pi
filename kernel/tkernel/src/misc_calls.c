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
 *	misc_calls.c
 *	Other System Calls
 */

/** [BEGIN Common Definitions] */
#include "kernel.h"
#include "task.h"
#include "check.h"
#include "misc_calls.h"
/** [END Common Definitions] */


#ifdef USE_FUNC_TK_REF_SYS
/*
 * Refer system state
 */
SYSCALL ER tk_ref_sys_impl( T_RSYS *pk_rsys )
{
	if ( in_indp() ) {
		pk_rsys->sysstat = TSS_INDP;
	} else {
		if ( in_qtsk() ) {
			pk_rsys->sysstat = TSS_QTSK;
		} else {
			pk_rsys->sysstat = TSS_TSK;
		}
		if ( in_loc() ) {
			pk_rsys->sysstat |= TSS_DINT;
		}
		if ( in_ddsp() ) {
			pk_rsys->sysstat |= TSS_DDSP;
		}
	}
	pk_rsys->runtskid = ( knl_ctxtsk != NULL )? knl_ctxtsk->tskid: 0;
	pk_rsys->schedtskid = ( knl_schedtsk != NULL )? knl_schedtsk->tskid: 0;

	return E_OK;
}
#endif /* USE_FUNC_TK_REF_SYS */

#ifdef USE_FUNC_TK_REF_VER
/*
 * Refer version information
 *	If there is no kernel version information,
 *	set 0 in each information. (Do NOT cause errors.)
 */
SYSCALL ER tk_ref_ver_impl( T_RVER *pk_rver )
{
	pk_rver->maker = (UH)CFN_VER_MAKER;	/* OS manufacturer */
	pk_rver->prid  = (UH)CFN_VER_PRID;	/* OS identification number */
	pk_rver->spver = (UH)CFN_VER_SPVER;	/* Specification version */
	pk_rver->prver = (UH)CFN_VER_PRVER;	/* OS product version */
	pk_rver->prno[0] = (UH)CFN_VER_PRNO1;	/* Product number */
	pk_rver->prno[1] = (UH)CFN_VER_PRNO2;	/* Product number */
	pk_rver->prno[2] = (UH)CFN_VER_PRNO3;	/* Product number */
	pk_rver->prno[3] = (UH)CFN_VER_PRNO4;	/* Product number */

	return E_OK;
}
#endif /* USE_FUNC_TK_REF_VER */

#ifdef USE_FUNC_LOWPOW_DISCNT
/*
 * Number of times for disabling power-saving mode switch
 *	If it is 0, the mode switch is enabled.
 */
EXPORT UINT	knl_lowpow_discnt = 0;
#endif /* USE_FUNC_LOWPOW_DISCNT */
/* ------------------------------------------------------------------------ */
/*
 *	Debugger support function
 */
#if USE_DBGSPT

/*
 * Hook routine address
 */
#ifdef USE_FUNC_HOOK_ENTERFN
Noinit(EXPORT FP knl_hook_enterfn);
Noinit(EXPORT FP knl_hook_leavefn);
#if TA_GP
Noinit(EXPORT VP knl_hook_svc_gp);
#endif
#endif /* USE_FUNC_HOOK_ENTERFN */

#ifdef USE_FUNC_HOOK_EXECFN
Noinit(EXPORT FP knl_hook_execfn);
Noinit(EXPORT FP knl_hook_stopfn);
#if TA_GP
Noinit(EXPORT VP knl_hook_dsp_gp);
#endif
#endif /* USE_FUNC_HOOK_EXECFN */

#ifdef USE_FUNC_HOOK_IENTERFN
Noinit(EXPORT FP knl_hook_ienterfn);
Noinit(EXPORT FP knl_hook_ileavefn);
#if TA_GP
Noinit(EXPORT VP knl_hook_int_gp);
#endif
#endif /* USE_FUNC_HOOK_IENTERFN */


#ifdef USE_FUNC_TD_HOK_SVC
/*
 * Set/Cancel system call/extended SVC hook routine
 */
SYSCALL ER td_hok_svc_impl P1( TD_HSVC *hsvc )
{
	BEGIN_DISABLE_INTERRUPT;
	if ( hsvc == NULL ) { /* Cancel system call hook routine */
		/* Cancel */
		knl_unhook_svc();
	} else {
		/* Set */
		knl_hook_enterfn = hsvc->enter;
		knl_hook_leavefn = hsvc->leave;
#if TA_GP
		knl_hook_svc_gp = gp;
#endif
		knl_hook_svc();
	}
	END_DISABLE_INTERRUPT;

	return E_OK;
}
#endif /* USE_FUNC_TD_HOK_SVC */

#ifdef USE_FUNC_TD_HOK_DSP
/*
 * Set/Cancel dispatcher hook routine
 */
SYSCALL ER td_hok_dsp_impl P1( TD_HDSP *hdsp )
{
	BEGIN_DISABLE_INTERRUPT;
	if ( hdsp == NULL ) { /* Cancel dispatcher hook routine */
		/* Cancel */
		knl_unhook_dsp();
	} else {
		/* Set */
		knl_hook_execfn = hdsp->exec;
		knl_hook_stopfn = hdsp->stop;
#if TA_GP
		knl_hook_dsp_gp = gp;
#endif
		knl_hook_dsp();
	}
	END_DISABLE_INTERRUPT;

	return E_OK;
}
#endif /* USE_FUNC_TD_HOK_DSP */

#ifdef USE_FUNC_TD_HOK_INT
/*
 * Set/Cancel EIT handler hook routine
 */
SYSCALL ER td_hok_int_impl P1( TD_HINT *hint )
{
#if USE_HLL_INTHDR
	BEGIN_DISABLE_INTERRUPT;
	if ( hint == NULL ) { /* Cancel interrupt handler hook routine */
		/* Cancel */
		knl_unhook_int();
	} else {
		/* Set */
		knl_hook_ienterfn = hint->enter;
		knl_hook_ileavefn = hint->leave;
#if TA_GP
		knl_hook_int_gp = gp;
#endif
		knl_hook_int();
	}
	END_DISABLE_INTERRUPT;

	return E_OK;
#else
	return E_NOSPT;
#endif /* USE_HLL_INTHDR */
}
#endif /* USE_FUNC_TD_HOK_INT */

#ifdef USE_FUNC_TD_REF_SYS
/*
 * Refer system state
 */
SYSCALL ER td_ref_sys_impl( TD_RSYS *pk_rsys )
{
	if ( in_indp() ) {
		pk_rsys->sysstat = TSS_INDP;
	} else {
		if ( in_qtsk() ) {
			pk_rsys->sysstat = TSS_QTSK;
		} else {
			pk_rsys->sysstat = TSS_TSK;
		}
		if ( in_loc() ) {
			pk_rsys->sysstat |= TSS_DINT;
		}
		if ( in_ddsp() ) {
			pk_rsys->sysstat |= TSS_DDSP;
		}
	}
	pk_rsys->runtskid = ( knl_ctxtsk != NULL )? knl_ctxtsk->tskid: 0;
	pk_rsys->schedtskid = ( knl_schedtsk != NULL )? knl_schedtsk->tskid: 0;

	return E_OK;
}
#endif /* USE_FUNC_TD_REF_SYS */

#endif /* USE_DBGSPT */
