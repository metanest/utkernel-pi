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
 *	cpu_calls.c
 *	CPU-Dependent System Call
 */

/** [BEGIN Common Definitions] */
#include "kernel.h"
#include "task.h"
#include "check.h"
#include "cpu_task.h"

#include <sys/sysinfo.h>
#include <tk/sysdef.h>
#include "cpu_insn.h"
#include "cpu_calls.h"
/** [END Common Definitions] */

#ifdef USE_FUNC_TK_DIS_DSP
/*
 * Dispatch enable/disable 
 */
SYSCALL ER tk_dis_dsp_impl( void )
{
	CHECK_CTX(!in_loc());

	knl_dispatch_disabled = DDS_DISABLE;

	return E_OK;
}
#endif /* USE_FUNC_TK_DIS_DSP */

#ifdef USE_FUNC_TK_ENA_DSP
/*
 * Dispatch enable 
 */
SYSCALL ER tk_ena_dsp_impl( void )
{
	CHECK_CTX(!in_loc());

	knl_dispatch_disabled = DDS_ENABLE;
	if ( knl_ctxtsk != knl_schedtsk ) {
		knl_dispatch();
	}

	return E_OK;
}
#endif /* USE_FUNC_TK_ENA_DSP */

/* ------------------------------------------------------------------------ */

/*
 * High level programming language
 */

#ifdef USE_FUNC_HLL_INTHDR
/* High level programming language interrupt handler entry */
Noinit(EXPORT FP knl_hll_inthdr[N_INTVEC]);
#endif /* USE_FUNC_HLL_INTHDR */

#ifdef USE_FUNC_TK_DEF_INT
IMPORT FP knl_hll_inthdr[];

/* High level programming language routine (Interrupt) */
IMPORT void knl_inthdr_startup();

/* High level programming language routine (Exception) */
IMPORT void knl_exchdr_startup();

/*
 * Interrupt handler definition
 */
SYSCALL ER tk_def_int_impl( UINT dintno, T_DINT *pk_dint )
{
	FP	inthdr;

	CHECK_PAR(dintno < N_INTVEC);
#if !USE_HLL_INTHDR
	CHECK_PAR((pk_dint->intatr & TA_HLNG) == 0);
#endif

	if ( pk_dint != NULL ) {
		/* Set interrupt handler */
		CHECK_RSATR(pk_dint->intatr, TA_HLNG);
#if PSR_DI == PSR_I
		CHECK_PAR( !(dintno == EIT_FIQ
				&& (pk_dint->intatr & TA_HLNG) != 0) );
#endif

		inthdr = pk_dint->inthdr;

		BEGIN_CRITICAL_SECTION;
#if USE_HLL_INTHDR
		if ( (pk_dint->intatr & TA_HLNG) != 0 ) {
			knl_hll_inthdr[dintno] = inthdr;
			inthdr = (dintno < 96) ? knl_inthdr_startup:
									 knl_exchdr_startup; /* SWI handler */
		}
#endif
		knl_define_inthdr(dintno, inthdr);
		END_CRITICAL_SECTION;
	} else {
		/* Clear interrupt handler */
		switch ( dintno ) {
		  default:		inthdr = NULL;
		}

		BEGIN_CRITICAL_SECTION;
		knl_define_inthdr(dintno, inthdr);
#if USE_HLL_INTHDR
		knl_hll_inthdr[dintno] = NULL;
#endif
		END_CRITICAL_SECTION;
	}

	return E_OK;
}
#endif /* USE_FUNC_TK_DEF_INT */

/* ------------------------------------------------------------------------ */

#ifdef USE_FUNC_SET_REG
/*
 * Set task register contents
 */
EXPORT void knl_set_reg( TCB *tcb, T_REGS *regs, T_EIT *eit, T_CREGS *cregs )
{
	SStackFrame	*ssp;
#if USE_TRAP
	UW	cpsr;
#endif
	INT	i;

	ssp = tcb->tskctxb.ssp;
#if USE_TRAP
	cpsr = ssp->spsr_svc;
#endif

	if ( cregs != NULL ) {
		ssp = cregs->ssp;
	}

	if ( regs != NULL ) {
		for ( i = 0; i < 12; ++i ) {
			ssp->r[i] = regs->r[i];
		}
#if USE_TRAP
		ssp->ip = regs->r[12];
		if ( (cpsr & PSR_M(31)) == PSR_SVC ) {
			ssp->lr_svc = regs->lr;
		} else {
			ssp->lr_usr = regs->lr;
		}
#endif
	}

	if ( eit != NULL ) {
		ssp->pc       = eit->pc;
#if USE_TRAP
		ssp->spsr_svc = (eit->cpsr & 0xff000000) | (cpsr & 0x00ffffff);
#endif
		ssp->taskmode = eit->taskmode;
	}

	if ( cregs != NULL ) {
		tcb->tskctxb.ssp  = cregs->ssp;

		ssp->usp = cregs->usp;
	}
}
#endif /* USE_FUNC_SET_REG */

#ifdef USE_FUNC_TK_SET_REG
/*
 * Set task register contents
 */
SYSCALL ER tk_set_reg_impl( ID tskid,
		T_REGS *pk_regs, T_EIT *pk_eit, T_CREGS *pk_cregs )
{
	TCB		*tcb;
	ER		ercd = E_OK;

	CHECK_INTSK();
	CHECK_TSKID(tskid);
	CHECK_NONSELF(tskid);

	tcb = get_tcb(tskid);

	BEGIN_CRITICAL_SECTION;
	if ( tcb->state == TS_NONEXIST ) {
		ercd = E_NOEXS;
	} else {
		knl_set_reg(tcb, pk_regs, pk_eit, pk_cregs);
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_SET_REG */

#ifdef USE_FUNC_GET_REG
/*
 * Get task register contents
 */
EXPORT void knl_get_reg( TCB *tcb, T_REGS *regs, T_EIT *eit, T_CREGS *cregs )
{
	SStackFrame	*ssp;
#if USE_TRAP
	UW		cpsr;
#endif
	INT		i;

	ssp = tcb->tskctxb.ssp;
#if USE_TRAP
	cpsr = ssp->spsr_svc;
#endif

	if ( regs != NULL ) {
		for ( i = 0; i < 12; ++i ) {
			regs->r[i] = ssp->r[i];
		}
#if USE_TRAP
		regs->r[12] = ssp->ip;
		if ( (cpsr & PSR_M(31)) == PSR_SVC ) {
			regs->lr = ssp->lr_svc;
		} else {
			regs->lr = ssp->lr_usr;
		}
#endif
	}

	if ( eit != NULL ) {
		eit->pc       = ssp->pc;
#if USE_TRAP
		eit->cpsr     = ssp->spsr_svc;
#endif
		eit->taskmode = ssp->taskmode;
	}

	if ( cregs != NULL ) {
		cregs->ssp   = tcb->tskctxb.ssp;

		cregs->usp = ssp->usp;
	}
}
#endif /* USE_FUNC_GET_REG */

#ifdef USE_FUNC_TK_GET_REG
/*
 * Get task register contents
 */
SYSCALL ER tk_get_reg_impl( ID tskid, T_REGS *pk_regs, T_EIT *pk_eit, T_CREGS *pk_cregs )
{
	TCB		*tcb;
	ER		ercd = E_OK;

	CHECK_INTSK();
	CHECK_TSKID(tskid);
	CHECK_NONSELF(tskid);

	tcb = get_tcb(tskid);

	BEGIN_CRITICAL_SECTION;
	if ( tcb->state == TS_NONEXIST ) {
		ercd = E_NOEXS;
	} else {
		knl_get_reg(tcb, pk_regs, pk_eit, pk_cregs);
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_GET_REG */

/* ------------------------------------------------------------------------ */
/*
 *	Debugger support function
 */
#if USE_DBGSPT

#ifdef USE_FUNC_TD_SET_REG
/*
 * Set task register
 */
SYSCALL ER td_set_reg_impl( ID tskid, T_REGS *regs, T_EIT *eit, T_CREGS *cregs )
{
	TCB	*tcb;
	ER	ercd = E_OK;

	CHECK_TSKID(tskid);

	tcb = get_tcb(tskid);
	if ( tcb == knl_ctxtsk ) {
		return E_OBJ;
	}

	BEGIN_DISABLE_INTERRUPT;
	if ( tcb->state == TS_NONEXIST ) {
		ercd = E_NOEXS;
	} else {
		knl_set_reg(tcb, regs, eit, cregs);
	}
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_FUNC_TD_SET_REG */

#ifdef USE_FUNC_TD_GET_REG
/*
 * Get task register
 */
SYSCALL ER td_get_reg_impl( ID tskid, T_REGS *regs, T_EIT *eit, T_CREGS *cregs )
{
	TCB	*tcb;
	ER	ercd = E_OK;

	CHECK_TSKID(tskid);

	tcb = get_tcb(tskid);
	if ( tcb == knl_ctxtsk ) {
		return E_OBJ;
	}

	BEGIN_DISABLE_INTERRUPT;
	if ( tcb->state == TS_NONEXIST ) {
		ercd = E_NOEXS;
	} else {
		knl_get_reg(tcb, regs, eit, cregs);
	}
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_FUNC_TD_GET_REG */

#endif /* USE_DBGSPT */
