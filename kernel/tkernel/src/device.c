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
 *	device.c
 *	Device Management Function
 */

/** [BEGIN Common Definitions] */
#include "kernel.h"
#include "sysmgr.h"
#include "device.h"
/** [END Common Definitions] */

#if CFN_MAX_REGDEV > 0

#ifdef USE_FUNC_DEVMGRLOCK
/* Lock for device management exclusive control */
Noinit(EXPORT	FastMLock	knl_DevMgrLock);
#endif /* USE_FUNC_DEVMGRLOCK */

#ifdef USE_FUNC_DEFAULTIDEV
/* Device initial setting information */
Noinit(EXPORT	T_IDEV		knl_DefaultIDev);
#endif /* USE_FUNC_DEFAULTIDEV */

/* ------------------------------------------------------------------------ */
/*
 *	Device registration management
 */

#ifdef USE_FUNC_DEVCBTBL
Noinit(EXPORT	DevCB		knl_DevCBtbl[CFN_MAX_REGDEV]);	/* Device registration information table */
Noinit(EXPORT	QUEUE		knl_UsedDevCB);	/* In-use queue */
Noinit(EXPORT	QUEUE		knl_FreeDevCB);	/* Unused queue */
#endif /* USE_FUNC_DEVCBTBL */


#ifdef USE_FUNC_SEARCHDEVCB
/*
 * Search registration device
 */
EXPORT DevCB* knl_searchDevCB( UB *devnm )
{
	QUEUE	*q;
	DevCB	*devcb;

	for ( q = knl_UsedDevCB.next; q != &knl_UsedDevCB; q = q->next ) {
		devcb = (DevCB*)q;

		if ( devcb->devnm[0] == devnm[0] && strcmp((char*)devcb->devnm, (char*)devnm) == 0 ) {
			return devcb; /* Found */
		}
	}

	return NULL;
}
#endif /* USE_FUNC_SEARCHDEVCB */

#ifdef USE_FUNC_TK_DEF_DEV
/*
 * Get DevCB for new registration
 */
LOCAL DevCB* newDevCB( UB *devnm )
{
	DevCB	*devcb;

	devcb = (DevCB*)QueRemoveNext(&knl_FreeDevCB);
	if ( devcb == NULL ) {
		return NULL; /* No space */
	}

	strncpy((char*)devcb->devnm, (char*)devnm, L_DEVNM+1);
	QueInit(&devcb->openq);

	QueInsert(&devcb->q, &knl_UsedDevCB);

	return devcb;
}

/*
 * Free DevCB
 */
LOCAL void delDevCB( DevCB *devcb )
{
	QueRemove(&devcb->q);
	QueInsert(&devcb->q, &knl_FreeDevCB);
	devcb->devnm[0] = '\0';
}

/*
 * Device registration
 */
SYSCALL ID tk_def_dev_impl P3( UB *devnm, T_DDEV *pk_ddev, T_IDEV *pk_idev )
{
	DevCB	*devcb;
	INT	len;
	ER	ercd;

	LockREG();

	len = strlen((char*)devnm);
	if ( len == 0 || len > L_DEVNM ) {
		ercd = E_PAR;
		goto err_ret1;
	}

	if ( pk_ddev != NULL ) {
		if ( pk_ddev->nsub < 0 || pk_ddev->nsub > MAX_UNIT ) {
			ercd = E_PAR;
			goto err_ret1;
		}

		/* Make sure that the length of the logical device name
		   does not exceed the character limit */
		if ( pk_ddev->nsub > 0   ) {
			++len;
		}
		if ( pk_ddev->nsub > 10  ) {
			++len;
		}
		if ( pk_ddev->nsub > 100 ) {
			++len;
		}
		if ( len > L_DEVNM ) {
			ercd = E_PAR;
			goto err_ret1;
		}
	}

	LockDM();

	/* Search whether 'devnm' device is registered */
	devcb = knl_searchDevCB(devnm);
	if ( devcb == NULL ) {
		if ( pk_ddev == NULL ) {
			ercd = E_NOEXS;
			goto err_ret2;
		}

		/* Get 'devcb' for new registration because it is not
		   registered */
		devcb = newDevCB(devnm);
		if ( devcb == NULL ) {
			ercd = E_LIMIT;
			goto err_ret2;
		}
	}

	if ( pk_ddev != NULL ) {
		/* Set/update device registration information */
		devcb->ddev = *pk_ddev;
#if TA_GP
		if ( (pk_ddev->drvatr & TA_GP) == 0 ) {
			/* Apply caller 'gp' if TA_GP is not specified */
			devcb->ddev.gp = gp;
		}
#endif

		if ( pk_idev != NULL ) {
			/* Device initial setting information */
			*pk_idev = knl_DefaultIDev;
		}
	} else {
		if ( !isQueEmpty(&devcb->openq) ) {
			/* In use (open) */
			ercd = E_BUSY;
			goto err_ret2;
		}

		/* Device unregistration */
		delDevCB(devcb);
	}

	UnlockDM();
	UnlockREG();

	return DID(devcb);

err_ret2:
	UnlockDM();
err_ret1:
	UnlockREG();
	DEBUG_PRINT(("tk_def_dev_impl ercd = %d\n", ercd));
	return ercd;
}
#endif /* USE_FUNC_TK_DEF_DEV */

#ifdef USE_FUNC_TK_REF_IDV
/*
 * Check device initial information
 */
SYSCALL ER tk_ref_idv_impl( T_IDEV *pk_idev )
{
	LockDM();
	*pk_idev = knl_DefaultIDev;
	UnlockDM();

	return E_OK;
}
#endif /* USE_FUNC_TK_REF_IDV */

/* ------------------------------------------------------------------------ */

#ifdef USE_FUNC_PHYDEVNM
/*
 * Get physical device name
 *	Get the subunit number (return value) 
 *	from the logical device name (ldevnm) and the physical
 *	device name (pdevnm).
 */
EXPORT INT knl_phydevnm( UB *pdevnm, UB *ldevnm )
{
	UB	c;
	INT	unitno;

	while ( (c = *ldevnm) != '\0' ) {
		if ( c >= '0' && c <= '9' ) {
			break;
		}
		*pdevnm++ = c;
		ldevnm++;
	}
	*pdevnm = '\0';

	unitno = 0;
	if (c != '\0') {
		while ( (c = *ldevnm) != '\0' ) {
			unitno = unitno * 10 + (c - '0');
			ldevnm++;
		}
		++unitno;
	}

	return unitno;
}
#endif /* USE_FUNC_PHYDEVNM */

#ifdef USE_FUNC_TK_GET_DEV
/*
 * Get logical device name
 *	Get the logical device name from
 *	the physical device name (pdevnm) and the subunit number (unitno).
 */
LOCAL void logdevnm( UB *ldevnm, UB *pdevnm, INT unitno )
{
	UB	unostr[12], *cp;

	strcpy((char*)ldevnm, (char*)pdevnm);
	if ( unitno > 0 ) {
		cp = &unostr[11];
		*cp = '\0';
		while (*ldevnm != '\0') {
			++ldevnm;
		}
		--unitno;
		do {
			*(--cp) = (UB)('0' + (unitno % 10));
			unitno /= 10;
		} while (unitno);
		strcat((char*)ldevnm, (char*)cp);
	}
}

/*
 * Get device name
 */
SYSCALL ID tk_get_dev_impl( ID devid, UB *devnm )
{
	DevCB	*devcb;
	ER	ercd;

	ercd = knl_check_devid(devid);
	if ( ercd < E_OK ) {
		goto err_ret1;
	}

	LockDM();

	devcb = DEVCB(devid);
	if ( (devcb->devnm[0] == '\0')||(UNITNO(devid) > devcb->ddev.nsub) ) {
		ercd = E_NOEXS;
		goto err_ret2;
	}

	logdevnm(devnm, devcb->devnm, UNITNO(devid));

	UnlockDM();

	return DID(devcb);

err_ret2:
	UnlockDM();
err_ret1:
	DEBUG_PRINT(("tk_get_dev_impl ercd = %d\n", ercd));
	return ercd;
}
#endif /* USE_FUNC_TK_GET_DEV */

#ifdef USE_FUNC_TK_REF_DEV
/*
 * Get device information
 */
SYSCALL ID tk_ref_dev_impl( UB *devnm, T_RDEV *pk_rdev )
{
	UB	pdevnm[L_DEVNM + 1];
	DevCB	*devcb;
	INT	unitno;
	ER	ercd;

	unitno = knl_phydevnm(pdevnm, devnm);

	LockDM();

	devcb = knl_searchDevCB(pdevnm);
	if ( devcb == NULL || unitno > devcb->ddev.nsub ) {
		ercd = E_NOEXS;
		goto err_ret2;
	}

	if ( pk_rdev != NULL ) {
		pk_rdev->devatr = devcb->ddev.devatr;
		pk_rdev->blksz  = devcb->ddev.blksz;
		pk_rdev->nsub   = devcb->ddev.nsub;
		pk_rdev->subno  = unitno;
	}

	UnlockDM();

	return DEVID(devcb, unitno);

err_ret2:
	UnlockDM();
	DEBUG_PRINT(("tk_ref_dev_impl ercd = %d\n", ercd));
	return ercd;
}
#endif /* USE_FUNC_TK_REF_DEV */

#ifdef USE_FUNC_TK_OREF_DEV
/*
 * Get device information
 */
SYSCALL ID tk_oref_dev_impl( ID dd, T_RDEV *pk_rdev )
{
	OpnCB	*opncb;
	DevCB	*devcb;
	INT	unitno;
	ER	ercd;

	LockDM();

	ercd = knl_check_devdesc(dd, 0, &opncb);
	if ( ercd < E_OK ) {
		goto err_ret2;
	}

	devcb  = opncb->devcb;
	unitno = opncb->unitno;

	if ( pk_rdev != NULL ) {
		pk_rdev->devatr = devcb->ddev.devatr;
		pk_rdev->blksz  = devcb->ddev.blksz;
		pk_rdev->nsub   = devcb->ddev.nsub;
		pk_rdev->subno  = unitno;
	}

	UnlockDM();

	return DEVID(devcb, unitno);

err_ret2:
	UnlockDM();
	DEBUG_PRINT(("tk_oref_dev_impl ercd = %d\n", ercd));
	return ercd;
}
#endif /* USE_FUNC_TK_OREF_DEV */

#ifdef USE_FUNC_TK_LST_DEV
/*
 * Get registration device list
 */
SYSCALL INT tk_lst_dev_impl( T_LDEV *pk_ldev, INT start, INT ndev )
{
	DevCB	*devcb;
	QUEUE	*q;
	INT	n, end;
	ER	ercd;

	if ( start < 0 || ndev < 0 ) {
		ercd = E_PAR;
		goto err_ret;
	}
	LockDM();

	end = start + ndev;
	n = 0;
	for ( q = knl_UsedDevCB.next; q != &knl_UsedDevCB; q = q->next ) {
		if ( n >= start && n < end ) {
			devcb = (DevCB*)q;
			pk_ldev->devatr = devcb->ddev.devatr;
			pk_ldev->blksz  = devcb->ddev.blksz;
			pk_ldev->nsub   = devcb->ddev.nsub;
			strncpy((char*)pk_ldev->devnm, (char*)devcb->devnm, L_DEVNM);
			pk_ldev++;
		}
		n++;
	}

	UnlockDM();

	if ( start >= n ) {
		ercd = E_NOEXS;
		goto err_ret;
	}

	return n - start;

err_ret:
	DEBUG_PRINT(("tk_lst_dev_impl ercd = %d\n", ercd));
	return ercd;
}
#endif /* USE_FUNC_TK_LST_DEV */

/* ------------------------------------------------------------------------ */

#ifdef USE_FUNC_TK_EVT_DEV
/*
 * Send driver request event
 */
SYSCALL INT tk_evt_dev_impl( ID devid, INT evttyp, VP evtinf )
{
	DevCB	*devcb;
	EVTFN	eventfn;
	VP	exinf;
#if TA_GP
	VP	gp;
#endif
	ER	ercd;

	ercd = knl_check_devid(devid);
	if ( ercd < E_OK ) {
		goto err_ret1;
	}
	if ( evttyp < 0 ) {
		ercd = E_PAR;
		goto err_ret1;
	}

	LockDM();

	devcb = DEVCB(devid);
	if ( (devcb->devnm[0] == '\0')||(UNITNO(devid) > devcb->ddev.nsub) ) {
		ercd = E_NOEXS;
		goto err_ret2;
	}

	eventfn = (EVTFN)devcb->ddev.eventfn;
	exinf = devcb->ddev.exinf;
#if TA_GP
	gp = devcb->ddev.gp;
#endif

	UnlockDM();

	/* Device driver call */
	DISABLE_INTERRUPT;
	knl_ctxtsk->sysmode++;
	ENABLE_INTERRUPT;
#if TA_GP
	ercd = CallDeviceDriver(evttyp, evtinf, exinf, 0, (FP)eventfn, gp);
#else
	ercd = (*eventfn)(evttyp, evtinf, exinf);
#endif
	DISABLE_INTERRUPT;
	knl_ctxtsk->sysmode--;
	ENABLE_INTERRUPT;

	return ercd;

err_ret2:
	UnlockDM();
err_ret1:
	DEBUG_PRINT(("tk_evt_dev_impl ercd = %d\n", ercd));
	return ercd;
}
#endif /* USE_FUNC_TK_EVT_DEV */

/* ------------------------------------------------------------------------ */

#ifdef USE_FUNC_INITIALIZE_DEVMGR
/*
 * Initialization of device registration information table
 */
LOCAL ER initDevCB( void )
{
	DevCB	*devcb;
	INT	num = CFN_MAX_REGDEV;

	QueInit(&knl_UsedDevCB);
	QueInit(&knl_FreeDevCB);

	devcb = knl_DevCBtbl;
	while ( num-- > 0 ) {
		QueInsert(&devcb->q, &knl_FreeDevCB);
		devcb->devnm[0] = '\0';
		devcb++;
	}

	return E_OK;
}

/*
 * Initialization of device initial setting information
 */
LOCAL ER initIDev( void )
{
#if CFN_DEVT_MBFSZ0 >= 0
	T_CMBF	cmbf;
	ER	ercd;

	/* Generate message buffer for event notification */
	strncpy((char*)&cmbf.exinf, (char*)OBJNAME_DMMBF, sizeof(cmbf.exinf));
	cmbf.mbfatr = TA_TFIFO;
	cmbf.bufsz  = CFN_DEVT_MBFSZ0;
	cmbf.maxmsz = CFN_DEVT_MBFSZ1;
	ercd = tk_cre_mbf_impl(&cmbf);
	if ( ercd < E_OK ) {
		knl_DefaultIDev.evtmbfid = 0;
		goto err_ret;
	}
	knl_DefaultIDev.evtmbfid = ercd;

	return E_OK;

err_ret:
	DEBUG_PRINT(("initIDev ercd = %d\n", ercd));
	return ercd;
#else
	/* Do not use message buffer for event notification */
	knl_DefaultIDev.evtmbfid = 0;
	return E_OK;
#endif
}

/*
 * Initialization of system management
 */
EXPORT ER knl_initialize_devmgr( void )
{
	ER	ercd;

	/* Generate lock for device management exclusive control */
	ercd = CreateMLock(&knl_DevMgrLock, (UB*)OBJNAME_DMLOCK);
	if ( ercd < E_OK ) {
		goto err_ret;
	}

	/* Generate device registration information table */
	ercd = initDevCB();
	if ( ercd < E_OK ) {
		goto err_ret;
	}

	/* Initialization of device input/output-related */
	ercd = knl_initDevIO();
	if ( ercd < E_OK ) {
		goto err_ret;
	}

	/* Initialization of device initial setting information */
	ercd = initIDev();
	if ( ercd < E_OK ) {
		goto err_ret;
	}

	knl_devmgr_startup();

	return E_OK;

err_ret:
	DEBUG_PRINT(("initialize_devmgr ercd = %d\n", ercd));
	knl_finish_devmgr();
	return ercd;
}
#endif /* USE_FUNC_INITIALIZE_DEVMGR */

#ifdef USE_FUNC_FINISH_DEVMGR
/*
 * Unregister device initial setting information
 */
LOCAL ER delIDev( void )
{
	ER	ercd = E_OK;

#if CFN_DEVT_MBFSZ0 >= 0
	/* Delete message buffer for event notification */
	if ( knl_DefaultIDev.evtmbfid > 0 ) {
		ercd = tk_del_mbf_impl(knl_DefaultIDev.evtmbfid);
		knl_DefaultIDev.evtmbfid = 0;
	}

#ifdef DEBUG
	if ( ercd < E_OK ) {
		DEBUG_PRINT(("delIDev ercd = %d\n", ercd));
	}
#endif

#endif /* CFN_DEVT_MBFSZ0 >= 0 */

	return ercd;
}

/*
 * Finalization sequence of system management 
 */
EXPORT ER knl_finish_devmgr( void )
{
	ER	ercd;

	knl_devmgr_cleanup();

	/* Unregister device initial setting information */
	ercd = delIDev();
#ifdef DEBUG
	if ( ercd < E_OK ) {
		DEBUG_PRINT(("2. finish_devmgr -> delIDev ercd = %d\n", ercd));
	}
#endif

	/* Finalization sequence of device input/output-related */
	ercd = knl_finishDevIO();
#ifdef DEBUG
	if ( ercd < E_OK ) {
		DEBUG_PRINT(("3. finish_devmgr -> finishDevIO ercd = %d\n", ercd));
	}
#endif

	/* Delete lock for device management exclusive control */
	DeleteMLock(&knl_DevMgrLock);

	return ercd;
}
#endif /* USE_FUNC_FINISH_DEVMGR */

#endif /* CFN_MAX_REGDEV */
