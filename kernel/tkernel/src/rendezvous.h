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
 *	rendezvous.h
 *	Rendezvous
 */

#ifndef _RENDEZVOUS_H_
#define _RENDEZVOUS_H_

/*
 * Rendezvous port control block
 */
typedef struct port_control_block {
	QUEUE	call_queue;	/* Port call wait queue */
	ID	porid;		/* Port ID */
	VP	exinf;		/* Extended information */
	ATR	poratr;		/* Port attribute */
	QUEUE	accept_queue;	/* Port accept wait queue */
	INT	maxcmsz;	/* Maximum length of call message */
	INT	maxrmsz;	/* Maximum length of reply message */
#if USE_OBJECT_NAME
	UB	name[OBJECT_NAME_LENGTH];	/* name */
#endif
} PORCB;
/** [END Common Definitions] */

IMPORT PORCB knl_porcb_table[];	/* Rendezvous port control block */
IMPORT QUEUE knl_free_porcb;	/* FreeQue */

#define get_porcb(id)	( &knl_porcb_table[INDEX_POR(id)] )


#if CFN_MAX_PORID > 0

#define RDVNO_SHIFT	(sizeof(RNO)*8/2)

/*
 * Create rendezvous number
 */
Inline RNO knl_gen_rdvno( TCB *tcb )
{
	RNO	rdvno;

	rdvno = tcb->wrdvno;
	tcb->wrdvno += (1U << RDVNO_SHIFT);

	return rdvno;
}

/*
 * Get task ID from rendezvous number 
 */
Inline ID knl_get_tskid_rdvno( RNO rdvno )
{
	return (ID)((UINT)rdvno & ((1U << RDVNO_SHIFT) - 1));
}

#endif /* CFN_MAX_PORID > 0 */

/*
 * Check validity of rendezvous number
 */
#define CHECK_RDVNO(rdvno) {					\
	if ( !CHK_TSKID(knl_get_tskid_rdvno(rdvno)) ) {		\
		return E_OBJ;					\
	}							\
}


/*
 * Definition of rendezvous wait specification
 */
IMPORT WSPEC knl_wspec_cal_tfifo;
IMPORT WSPEC knl_wspec_cal_tpri;
IMPORT WSPEC knl_wspec_rdv;


#endif /* _RENDEZVOUS_H_ */
