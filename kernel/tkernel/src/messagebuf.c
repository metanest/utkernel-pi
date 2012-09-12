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
 *	messagebuf.c
 *	Message Buffer
 */

/** [BEGIN Common Definitions] */
#include "kernel.h"
#include "task.h"
#include "wait.h"
#include "check.h"
#include "messagebuf.h"
/** [END Common Definitions] */

#if CFN_MAX_MBFID > 0


#ifdef USE_FUNC_MBFCB_TABLE
Noinit(EXPORT MBFCB knl_mbfcb_table[NUM_MBFID]);	/* Message buffer control block */
Noinit(EXPORT QUEUE knl_free_mbfcb);	/* FreeQue */
#endif /* USE_FUNC_MBFCB_TABLE */


#ifdef USE_FUNC_MESSAGEBUFFER_INITIALIZE
/*
 * Initialization of message buffer control block 
 */
EXPORT ER knl_messagebuffer_initialize( void )
{
	MBFCB	*mbfcb, *end;

	/* Get system information */
	if ( NUM_MBFID < 1 ) {
		return E_SYS;
	}

	/* Register all control blocks onto FreeQue */
	QueInit(&knl_free_mbfcb);
	end = knl_mbfcb_table + NUM_MBFID;
	for ( mbfcb = knl_mbfcb_table; mbfcb < end; mbfcb++ ) {
		mbfcb->mbfid = 0;
		QueInsert(&mbfcb->send_queue, &knl_free_mbfcb);
	}

	return E_OK;
}
#endif /* USE_FUNC_MESSAGEBUFFER_INITIALIZE */

/* ------------------------------------------------------------------------ */

#ifdef USE_FUNC_MSG_TO_MBF
/*
 * Store the message to message buffer.
 */
EXPORT void knl_msg_to_mbf( MBFCB *mbfcb, VP msg, INT msgsz )
{
	W	tail = mbfcb->tail;
	VB	*buffer = mbfcb->buffer;
	W	remsz;

	mbfcb->frbufsz -= (W)(HEADERSZ + ROUNDSZ(msgsz));

	*(HEADER*)&buffer[tail] = msgsz;
	tail += HEADERSZ;
	if ( tail >= mbfcb->bufsz ) {
		tail = 0;
	}

	if ( (remsz = mbfcb->bufsz - tail) < (W)msgsz ) {
		memcpy(&buffer[tail], msg, (size_t)remsz);
		msg = (VB*)msg + remsz;
		msgsz -= (INT)remsz;
		tail = 0;
	}
	memcpy(&buffer[tail], msg, (size_t)msgsz);
	tail += (W)ROUNDSZ(msgsz);
	if ( tail >= mbfcb->bufsz ) {
		tail = 0;
	}

	mbfcb->tail = tail;
}
#endif /* USE_FUNC_MSG_TO_MBF */

/* ------------------------------------------------------------------------ */

#ifdef USE_FUNC_MBF_WAKEUP
/*
 * Accept message and release wait task,
 * as long as there are free message area.
 */
EXPORT void knl_mbf_wakeup( MBFCB *mbfcb )
{
	TCB	*top;
	INT	msgsz;

	while ( !isQueEmpty(&mbfcb->send_queue) ) {
		top = (TCB*)mbfcb->send_queue.next;
		msgsz = top->winfo.smbf.msgsz;
		if ( !knl_mbf_free(mbfcb, msgsz) ) {
			break;
		}

		/* Store a message from waiting task and release it */
		knl_msg_to_mbf(mbfcb, top->winfo.smbf.msg, msgsz);
		knl_wait_release_ok(top);
	}
}
#endif /* USE_FUNC_MBF_WAKEUP */


#ifdef USE_FUNC_TK_CRE_MBF
/*
 * Create message buffer
 */
SYSCALL ID tk_cre_mbf_impl( T_CMBF *pk_cmbf )
{
#if CHK_RSATR
	const ATR VALID_MBFATR = {
		 TA_TPRI
		|TA_USERBUF
#if USE_OBJECT_NAME
		|TA_DSNAME
#endif
	};
#endif
	MBFCB	*mbfcb;
	ID	mbfid;
	W	bufsz;
	VB	*msgbuf;
	ER	ercd;

	CHECK_RSATR(pk_cmbf->mbfatr, VALID_MBFATR);
	CHECK_PAR(pk_cmbf->bufsz >= 0);
	CHECK_PAR(pk_cmbf->maxmsz > 0);
#if !USE_IMALLOC
	/* TA_USERBUF must be specified if configured in no Imalloc */
	CHECK_PAR((pk_cmbf->mbfatr & TA_USERBUF) != 0);
#endif
	bufsz = (W)ROUNDSZ(pk_cmbf->bufsz);

	if ( bufsz > 0 ) {
#if USE_IMALLOC
		if ( (pk_cmbf->mbfatr & TA_USERBUF) != 0 ) {
			/* Size of user buffer must be multiples of sizeof(HEADER) */
			if ( bufsz != pk_cmbf->bufsz ) {
				return E_PAR;
			}
			/* Use user buffer */
			msgbuf = (VB*) pk_cmbf->bufptr;
		} else {
			/* Allocate by kernel */
			msgbuf = knl_Imalloc((UW)bufsz);
			if ( msgbuf == NULL ) {
				return E_NOMEM;
			}
		}
#else
		/* Size of user buffer must be multiples of sizeof(HEADER) */
		if ( bufsz != pk_cmbf->bufsz ) {
			return E_PAR;
		}
		/* Use user buffer */
		msgbuf = (VB*) pk_cmbf->bufptr;
#endif
	} else {
		msgbuf = NULL;
	}

	BEGIN_CRITICAL_SECTION;
	/* Get control block from FreeQue */
	mbfcb = (MBFCB*)QueRemoveNext(&knl_free_mbfcb);
	if ( mbfcb == NULL ) {
		ercd = E_LIMIT;
	} else {
		mbfid = ID_MBF(mbfcb - knl_mbfcb_table);

		/* Initialize control block */
		QueInit(&mbfcb->send_queue);
		mbfcb->mbfid = mbfid;
		mbfcb->exinf = pk_cmbf->exinf;
		mbfcb->mbfatr = pk_cmbf->mbfatr;
		QueInit(&mbfcb->recv_queue);
		mbfcb->buffer = msgbuf;
		mbfcb->bufsz = mbfcb->frbufsz = bufsz;
		mbfcb->maxmsz = pk_cmbf->maxmsz;
		mbfcb->head = mbfcb->tail = 0;
#if USE_OBJECT_NAME
		if ( (pk_cmbf->mbfatr & TA_DSNAME) != 0 ) {
			strncpy((char*)mbfcb->name, (char*)pk_cmbf->dsname,
				OBJECT_NAME_LENGTH);
		}
#endif
		ercd = mbfid;
	}
	END_CRITICAL_SECTION;

#if USE_IMALLOC
	if ( (ercd < E_OK) && (msgbuf != NULL) && ((pk_cmbf->mbfatr & TA_USERBUF) == 0 ) ) {
		knl_Ifree(msgbuf);
	}
#endif

	return ercd;
}
#endif /* USE_FUNC_TK_CRE_MBF */

#ifdef USE_FUNC_TK_DEL_MBF
/*
 * Delete message buffer
 */
SYSCALL ER tk_del_mbf_impl( ID mbfid )
{
	MBFCB	*mbfcb;
	VB	*msgbuf = NULL;
	ER	ercd = E_OK;

	CHECK_MBFID(mbfid);

	mbfcb = get_mbfcb(mbfid);

	BEGIN_CRITICAL_SECTION;
	if ( mbfcb->mbfid == 0 ) {
		ercd = E_NOEXS;
	} else {
		msgbuf = mbfcb->buffer;

		/* Release wait state of task (E_DLT) */
		knl_wait_delete(&mbfcb->recv_queue);
		knl_wait_delete(&mbfcb->send_queue);

		/* Return to FreeQue */
		QueInsert(&mbfcb->send_queue, &knl_free_mbfcb);
		mbfcb->mbfid = 0;
	}
	END_CRITICAL_SECTION;

#if USE_IMALLOC
	if ( msgbuf != NULL && ((mbfcb->mbfatr & TA_USERBUF) == 0 ) ) {
		knl_Ifree(msgbuf);
	}
#endif

	return ercd;
}
#endif /* USE_FUNC_TK_DEL_MBF */

#ifdef USE_FUNC_TK_SND_MBF
/*
 * Processing if the priority of wait task changes
 */
LOCAL void knl_mbf_chg_pri( TCB *tcb, INT oldpri )
{
	MBFCB	*mbfcb;

	mbfcb = get_mbfcb(tcb->wid);
	if ( oldpri >= 0 ) {
		/* Reorder wait queue */
		knl_gcb_change_priority((GCB*)mbfcb, tcb);
	}

	/* If the new head task in a send wait queue is able to sent, 
	   send its message */
	knl_mbf_wakeup(mbfcb);
}

/*
 * Processing if the wait task is released
 */
LOCAL void knl_mbf_rel_wai( TCB *tcb )
{
	knl_mbf_chg_pri(tcb, -1);
}

/*
 * Definition of message buffer wait specification
 */
LOCAL WSPEC knl_wspec_smbf_tfifo = { TTW_SMBF, NULL,	knl_mbf_rel_wai };
LOCAL WSPEC knl_wspec_smbf_tpri  = { TTW_SMBF, knl_mbf_chg_pri,	knl_mbf_rel_wai };

/*
 * Send to message buffer
 */
SYSCALL ER tk_snd_mbf_impl( ID mbfid, VP msg, INT msgsz, TMO tmout )
{
	MBFCB	*mbfcb;
	TCB	*tcb;
	ER	ercd = E_OK;

	CHECK_MBFID(mbfid);
	CHECK_PAR(msgsz > 0);
	CHECK_TMOUT(tmout);
	CHECK_DISPATCH_POL(tmout);

	mbfcb = get_mbfcb(mbfid);

	BEGIN_CRITICAL_SECTION;
	if ( mbfcb->mbfid == 0 ) {
		ercd = E_NOEXS;
		goto error_exit;
	}
#if CHK_PAR
	if ( msgsz > mbfcb->maxmsz ) {
		ercd = E_PAR;
		goto error_exit;
	}
#endif

	if ( !isQueEmpty(&mbfcb->recv_queue) ) {
		/* Send directly to the receive wait task */
		tcb = (TCB*)mbfcb->recv_queue.next;
		memcpy(tcb->winfo.rmbf.msg, msg, (size_t)msgsz);
		*tcb->winfo.rmbf.p_msgsz = msgsz;
		knl_wait_release_ok(tcb);

	} else if ( (in_indp() || knl_gcb_top_of_wait_queue((GCB*)mbfcb, knl_ctxtsk) == knl_ctxtsk)
		  &&(knl_mbf_free(mbfcb, msgsz)) ) {
		/* Store the message to message buffer */
		knl_msg_to_mbf(mbfcb, msg, msgsz);

	} else {
		ercd = E_TMOUT;
		if ( tmout != TMO_POL ) {
			/* Ready for send wait */
			knl_ctxtsk->wspec = ( (mbfcb->mbfatr & TA_TPRI) != 0 )?
					&knl_wspec_smbf_tpri: &knl_wspec_smbf_tfifo;
			knl_ctxtsk->wercd = &ercd;
			knl_ctxtsk->winfo.smbf.msg = msg;
			knl_ctxtsk->winfo.smbf.msgsz = msgsz;
			knl_gcb_make_wait((GCB*)mbfcb, tmout);
		}
	}

    error_exit:
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_SND_MBF */

#ifdef USE_FUNC_TK_RCV_MBF

LOCAL WSPEC knl_wspec_rmbf       = { TTW_RMBF, NULL,	NULL	    };

/*
 * Get a message from message buffer.
 * Return the message size.
 */
LOCAL INT knl_mbf_to_msg( MBFCB *mbfcb, VP msg )
{
	W	head = mbfcb->head;
	VB	*buffer = mbfcb->buffer;
	INT	msgsz, actsz;
	W	remsz;

	actsz = msgsz = *(HEADER*)&buffer[head];
	mbfcb->frbufsz += (W)(HEADERSZ + ROUNDSZ(msgsz));

	head += (W)HEADERSZ;
	if ( head >= mbfcb->bufsz ) {
		head = 0;
	}

	if ( (remsz = mbfcb->bufsz - head) < (W)msgsz ) {
		memcpy(msg, &buffer[head], (size_t)remsz);
		msg = (VB*)msg + remsz;
		msgsz -= (INT)remsz;
		head = 0;
	}
	memcpy(msg, &buffer[head], (size_t)msgsz);
	head += (INT)ROUNDSZ(msgsz);
	if ( head >= mbfcb->bufsz ) {
		head = 0;
	}

	mbfcb->head = head;

	return actsz;
}

/*
 * Receive from message buffer
 */
SYSCALL INT tk_rcv_mbf_impl( ID mbfid, VP msg, TMO tmout )
{
	MBFCB	*mbfcb;
	TCB	*tcb;
	INT	rcvsz;
	ER	ercd = E_OK;

	CHECK_MBFID(mbfid);
	CHECK_TMOUT(tmout);
	CHECK_DISPATCH();

	mbfcb = get_mbfcb(mbfid);

	BEGIN_CRITICAL_SECTION;
	if (mbfcb->mbfid == 0) {
		ercd = E_NOEXS;
		goto error_exit;
	}

	if ( !knl_mbf_empty(mbfcb) ) {
		/* Read from message buffer */
		rcvsz = knl_mbf_to_msg(mbfcb, msg);

		/* Accept message from sending task(s) */
		knl_mbf_wakeup(mbfcb);

	} else if ( !isQueEmpty(&mbfcb->send_queue) ) {
		/* Receive directly from send wait task */
		tcb = (TCB*)mbfcb->send_queue.next;
		rcvsz = tcb->winfo.smbf.msgsz;
		memcpy(msg, tcb->winfo.smbf.msg, (size_t)rcvsz);
		knl_wait_release_ok(tcb);
		knl_mbf_wakeup(mbfcb);
	} else {
		ercd = E_TMOUT;
		if ( tmout != TMO_POL ) {
			/* Ready for receive wait */
			knl_ctxtsk->wspec = &knl_wspec_rmbf;
			knl_ctxtsk->wid = mbfid;
			knl_ctxtsk->wercd = &ercd;
			knl_ctxtsk->winfo.rmbf.msg = msg;
			knl_ctxtsk->winfo.rmbf.p_msgsz = &rcvsz;
			knl_make_wait(tmout, mbfcb->mbfatr);
			QueInsert(&knl_ctxtsk->tskque, &mbfcb->recv_queue);
		}
	}

    error_exit:
	END_CRITICAL_SECTION;

	return ( ercd < E_OK )? ercd: rcvsz;
}
#endif /* USE_FUNC_TK_RCV_MBF */

#ifdef USE_FUNC_TK_REF_MBF
/*
 * Refer message buffer state
 */
SYSCALL ER tk_ref_mbf_impl( ID mbfid, T_RMBF *pk_rmbf )
{
	MBFCB	*mbfcb;
	TCB	*tcb;
	ER	ercd = E_OK;

	CHECK_MBFID(mbfid);

	mbfcb = get_mbfcb(mbfid);

	BEGIN_CRITICAL_SECTION;
	if ( mbfcb->mbfid == 0 ) {
		ercd = E_NOEXS;
	} else {
		pk_rmbf->exinf = mbfcb->exinf;
		pk_rmbf->wtsk = knl_wait_tskid(&mbfcb->recv_queue);
		pk_rmbf->stsk = knl_wait_tskid(&mbfcb->send_queue);
		if ( !knl_mbf_empty(mbfcb) ) {
			pk_rmbf->msgsz = *(HEADER*)&mbfcb->buffer[mbfcb->head];
		} else {
			if ( !isQueEmpty(&mbfcb->send_queue) ) {
				tcb = (TCB*)mbfcb->send_queue.next;
				pk_rmbf->msgsz = tcb->winfo.smbf.msgsz;
			} else {
				pk_rmbf->msgsz = 0;
			}
		}
		pk_rmbf->frbufsz = mbfcb->frbufsz;
		pk_rmbf->maxmsz = mbfcb->maxmsz;
	}
	END_CRITICAL_SECTION;

	return ercd;
}
#endif /* USE_FUNC_TK_REF_MBF */

/* ------------------------------------------------------------------------ */
/*
 *	Debugger support function
 */
#if USE_DBGSPT

#ifdef USE_FUNC_MESSAGEBUFFER_GETNAME
#if USE_OBJECT_NAME
/*
 * Get object name from control block
 */
EXPORT ER knl_messagebuffer_getname(ID id, UB **name)
{
	MBFCB	*mbfcb;
	ER	ercd = E_OK;

	CHECK_MBFID(id);

	BEGIN_DISABLE_INTERRUPT;
	mbfcb = get_mbfcb(id);
	if ( mbfcb->mbfid == 0 ) {
		ercd = E_NOEXS;
		goto error_exit;
	}
	if ( (mbfcb->mbfatr & TA_DSNAME) == 0 ) {
		ercd = E_OBJ;
		goto error_exit;
	}
	*name = mbfcb->name;

    error_exit:
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_OBJECT_NAME */
#endif /* USE_FUNC_MESSAGEBUFFER_GETNAME */

#ifdef USE_FUNC_TD_LST_MBF
/*
 * Refer message buffer usage state
 */
SYSCALL INT td_lst_mbf_impl( ID list[], INT nent )
{
	MBFCB	*mbfcb, *end;
	INT	n = 0;

	BEGIN_DISABLE_INTERRUPT;
	end = knl_mbfcb_table + NUM_MBFID;
	for ( mbfcb = knl_mbfcb_table; mbfcb < end; mbfcb++ ) {
		if ( mbfcb->mbfid == 0 ) {
			continue;
		}

		if ( n++ < nent ) {
			*list++ = mbfcb->mbfid;
		}
	}
	END_DISABLE_INTERRUPT;

	return n;
}
#endif /* USE_FUNC_TD_LST_MBF */

#ifdef USE_FUNC_TD_REF_MBF
/*
 * Refer message buffer state
 */
SYSCALL ER td_ref_mbf_impl( ID mbfid, TD_RMBF *pk_rmbf )
{
	MBFCB	*mbfcb;
	TCB	*tcb;
	ER	ercd = E_OK;

	CHECK_MBFID(mbfid);

	mbfcb = get_mbfcb(mbfid);

	BEGIN_DISABLE_INTERRUPT;
	if ( mbfcb->mbfid == 0 ) {
		ercd = E_NOEXS;
	} else {
		pk_rmbf->exinf = mbfcb->exinf;
		pk_rmbf->wtsk = knl_wait_tskid(&mbfcb->recv_queue);
		pk_rmbf->stsk = knl_wait_tskid(&mbfcb->send_queue);
		if ( !knl_mbf_empty(mbfcb) ) {
			pk_rmbf->msgsz = *(HEADER*)&mbfcb->buffer[mbfcb->head];
		} else {
			if ( !isQueEmpty(&mbfcb->send_queue) ) {
				tcb = (TCB*)mbfcb->send_queue.next;
				pk_rmbf->msgsz = tcb->winfo.smbf.msgsz;
			} else {
				pk_rmbf->msgsz = 0;
			}
		}
		pk_rmbf->frbufsz = mbfcb->frbufsz;
		pk_rmbf->maxmsz = mbfcb->maxmsz;
	}
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_FUNC_TD_REF_MBF */

#ifdef USE_FUNC_TD_SMBF_QUE
/*
 * Refer message buffer send wait queue
 */
SYSCALL INT td_smbf_que_impl( ID mbfid, ID list[], INT nent )
{
	MBFCB	*mbfcb;
	QUEUE	*q;
	ER	ercd = E_OK;

	CHECK_MBFID(mbfid);

	mbfcb = get_mbfcb(mbfid);

	BEGIN_DISABLE_INTERRUPT;
	if ( mbfcb->mbfid == 0 ) {
		ercd = E_NOEXS;
	} else {
		INT n = 0;
		for ( q = mbfcb->send_queue.next; q != &mbfcb->send_queue; q = q->next ) {
			if ( n++ < nent ) {
				*list++ = ((TCB*)q)->tskid;
			}
		}
		ercd = n;
	}
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_FUNC_TD_SMBF_QUE */

#ifdef USE_FUNC_TD_RMBF_QUE
/*
 * Refer message buffer receive wait queue
 */
SYSCALL INT td_rmbf_que_impl( ID mbfid, ID list[], INT nent )
{
	MBFCB	*mbfcb;
	QUEUE	*q;
	ER	ercd = E_OK;

	CHECK_MBFID(mbfid);

	mbfcb = get_mbfcb(mbfid);

	BEGIN_DISABLE_INTERRUPT;
	if ( mbfcb->mbfid == 0 ) {
		ercd = E_NOEXS;
	} else {
		INT n = 0;
		for ( q = mbfcb->recv_queue.next; q != &mbfcb->recv_queue; q = q->next ) {
			if ( n++ < nent ) {
				*list++ = ((TCB*)q)->tskid;
			}
		}
		ercd = n;
	}
	END_DISABLE_INTERRUPT;

	return ercd;
}
#endif /* USE_FUNC_TD_RMBF_QUE */

#endif /* USE_DBGSPT */
#endif /* CFN_MAX_MBFID */
