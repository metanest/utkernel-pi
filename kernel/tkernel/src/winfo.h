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
 *	winfo.h
 *	Definition of Wait Information for Synchronization/Communication Object
 */

#ifndef _WINFO_
#define _WINFO_

/*
 * Semaphore wait (TTW_SEM)
 */
typedef struct {
	INT	cnt;		/* Request resource number */
} WINFO_SEM;

/*
 * Event flag wait (TTW_FLG)
 */
typedef struct {
	UINT	waiptn;		/* Wait bit pattern */
	UINT	wfmode;		/* Wait mode */
	UINT	*p_flgptn;	/* Address that has a bit pattern
				   at wait released */
} WINFO_FLG;

/*
 * Mailbox wait (TTW_MBX)
 */
typedef struct {
	T_MSG	**ppk_msg;	/* Address that has the head of a
				   message packet */
} WINFO_MBX;

/*
 * Message buffer receive/send wait (TTW_RMBF, TTW_SMBF)
 */
typedef struct {
	VP	msg;		/* Address that has a received message */
	INT	*p_msgsz;	/* Address that has a received message size */
} WINFO_RMBF;

typedef struct {
	VP	msg;		/* Send message head address */
	INT	msgsz;		/* Send message size */
} WINFO_SMBF;

/*
 * Rendezvous call/accept/end wait (TTW_CAL, TTW_ACP, TTW_RDV)
 */
typedef struct {
	UINT	calptn;		/* Bit pattern that indicates caller
				   select condition */
	VP	msg;		/* Address that has a message */
	INT	cmsgsz;		/* Call message size */
	INT	*p_rmsgsz;	/* Address that has a reply message size */
} WINFO_CAL;

typedef struct {
	UINT	acpptn;		/* Bit pattern that indicates receiver
				   select condition */
	VP	msg;		/* Address that has a call message */
	RNO	*p_rdvno;	/* Address that has the rendezvous number */
	INT	*p_cmsgsz;	/* Address that has the call message size */
} WINFO_ACP;

typedef struct {
	RNO	rdvno;		/* Rendezvous number */
	VP	msg;		/* Address that has a message */
	INT	maxrmsz;	/* Maximum length of reply message */
	INT	*p_rmsgsz;	/* Address that has a reply message size */
} WINFO_RDV;

/*
 * Variable size memory pool wait (TTW_MPL)
 */
typedef struct {
	W	blksz;		/* Memory block size */
	VP	*p_blk;		/* Address that has the head of a
				   memory block */
} WINFO_MPL;

/*
 * Fixed size memory pool wait (TTW_MPF)
 */
typedef struct {
	VP	*p_blf;		/* Address that has the head of a
				   memory block */
} WINFO_MPF;

/*
 * Definition of wait information in task control block
 */
typedef union {
#if CFN_MAX_SEMID > 0
	WINFO_SEM	sem;
#endif
#if CFN_MAX_FLGID > 0
	WINFO_FLG	flg;
#endif
#if CFN_MAX_MBXID > 0
	WINFO_MBX	mbx;
#endif
#if CFN_MAX_MBFID > 0
	WINFO_RMBF	rmbf;
	WINFO_SMBF	smbf;
#endif
#if CFN_MAX_PORID > 0
	WINFO_CAL	cal;
	WINFO_ACP	acp;
	WINFO_RDV	rdv;
#endif
#if CFN_MAX_MPLID > 0
	WINFO_MPL	mpl;
#endif
#if CFN_MAX_MPFID > 0
	WINFO_MPF	mpf;
#endif
} WINFO;

/*
 * Definition of wait specification structure
 */
typedef struct {
	UW	tskwait;			/* Wait factor */
	void	(*chg_pri_hook)(TCB *, INT);	/* Process at task priority
						   change */
	void	(*rel_wai_hook)(TCB *);		/* Process at task wait
						   release */
} WSPEC;

#endif /* _WINFO_ */
