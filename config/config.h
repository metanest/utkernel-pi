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
 *	config.h
 *	System Configuration Definition
 */

#ifndef _CONFIG_
#define _CONFIG_

/* Task configuration */
#define MIN_TSKID	(1)
#define MAX_TSKID	(CFN_MAX_TSKID)
#define NUM_TSKID	(MAX_TSKID)
#define CHK_TSKID(id)	((MIN_TSKID) <= (id) && (id) <= (MAX_TSKID))
#define INDEX_TSK(id)	((id)-(MIN_TSKID))
#define ID_TSK(index)	((index)+(MIN_TSKID))

/* Semaphore configuration */
#define MIN_SEMID	(1)
#define MAX_SEMID	(CFN_MAX_SEMID)
#define NUM_SEMID	(MAX_SEMID)
#define CHK_SEMID(id)	((MIN_SEMID) <= (id) && (id) <= (MAX_SEMID))
#define INDEX_SEM(id)	((id)-(MIN_SEMID))
#define ID_SEM(index)	((index)+(MIN_SEMID))

/* Event flag configuration */
#define MIN_FLGID	(1)
#define MAX_FLGID	(CFN_MAX_FLGID)
#define NUM_FLGID	(MAX_FLGID)
#define CHK_FLGID(id)	((MIN_FLGID) <= (id) && (id) <= (MAX_FLGID))
#define INDEX_FLG(id)	((id)-(MIN_FLGID))
#define ID_FLG(index)	((index)+(MIN_FLGID))

/* Mailbox configuration */
#define MIN_MBXID	(1)
#define MAX_MBXID	(CFN_MAX_MBXID)
#define NUM_MBXID	(MAX_MBXID)
#define CHK_MBXID(id)	((MIN_MBXID) <= (id) && (id) <= (MAX_MBXID))
#define INDEX_MBX(id)	((id)-(MIN_MBXID))
#define ID_MBX(index)	((index)+(MIN_MBXID))

/* Mutex configuration */
#define MIN_MTXID	(1)
#define MAX_MTXID	(CFN_MAX_MTXID)
#define NUM_MTXID	(MAX_MTXID)
#define CHK_MTXID(id)	((MIN_MTXID) <= (id) && (id) <= (MAX_MTXID))
#define INDEX_MTX(id)	((id)-(MIN_MTXID))
#define ID_MTX(index)	((index)+(MIN_MTXID))

/* Message buffer configuration */
#define MIN_MBFID	(1)
#define MAX_MBFID	(CFN_MAX_MBFID)
#define NUM_MBFID	(MAX_MBFID)
#define CHK_MBFID(id)	((MIN_MBFID) <= (id) && (id) <= (MAX_MBFID))
#define INDEX_MBF(id)	((id)-(MIN_MBFID))
#define ID_MBF(index)	((index)+(MIN_MBFID))

/* Rendezvous configuration */
#define MIN_PORID	(1)
#define MAX_PORID	(CFN_MAX_PORID)
#define NUM_PORID	(MAX_PORID)
#define CHK_PORID(id)	((MIN_PORID) <= (id) && (id) <= (MAX_PORID))
#define INDEX_POR(id)	((id)-(MIN_PORID))
#define ID_POR(index)	((index)+(MIN_PORID))

/* Memory pool configuration */
#define MIN_MPLID	(1)
#define MAX_MPLID	(CFN_MAX_MPLID)
#define NUM_MPLID	(MAX_MPLID)
#define CHK_MPLID(id)	((MIN_MPLID) <= (id) && (id) <= (MAX_MPLID))
#define INDEX_MPL(id)	((id)-(MIN_MPLID))
#define ID_MPL(index)	((index)+(MIN_MPLID))

/* Fixed size memory pool configuration */
#define MIN_MPFID	(1)
#define MAX_MPFID	(CFN_MAX_MPFID)
#define NUM_MPFID	(MAX_MPFID)
#define CHK_MPFID(id)	((MIN_MPFID) <= (id) && (id) <= (MAX_MPFID))
#define INDEX_MPF(id)	((id)-(MIN_MPFID))
#define ID_MPF(index)	((index)+(MIN_MPFID))

/* Cyclic handler configuration */
#define MIN_CYCID	(1)
#define MAX_CYCID	(CFN_MAX_CYCID)
#define NUM_CYCID	(MAX_CYCID)
#define CHK_CYCID(id)	((MIN_CYCID) <= (id) && (id) <= (MAX_CYCID))
#define INDEX_CYC(id)	((id)-(MIN_CYCID))
#define ID_CYC(index)	((index)+(MIN_CYCID))

/* Alarm handler configuration */
#define MIN_ALMID	(1)
#define MAX_ALMID	(CFN_MAX_ALMID)
#define NUM_ALMID	(MAX_ALMID)
#define CHK_ALMID(id)	((MIN_ALMID) <= (id) && (id) <= (MAX_ALMID))
#define INDEX_ALM(id)	((id)-(MIN_ALMID))
#define ID_ALM(index)	((index)+(MIN_ALMID))

/* Subsystem manager configuration */
#define MIN_SSYID	(1)
#define MAX_SSYID	(CFN_MAX_SSYID)
#define NUM_SSYID	(MAX_SSYID)
#define CHK_SSYID(id)	((MIN_SSYID) <= (id) && (id) <= (MAX_SSYID))
#define INDEX_SSY(id)	((id)-(MIN_SSYID))
#define ID_SSY(index)	((index)+(MIN_SSYID))

/* Task priority configuration */
#define MIN_PRI		(1)	/* Minimum priority number = highest priority */
#define MAX_PRI		(32)	/* Maximum priority number = lowest priority */
#define NUM_PRI		(32)	/* Number of priority levels */
#define CHK_PRI(pri)	((MIN_PRI) <= (pri) && (pri) <= (MAX_PRI))

/* Subsystem manager configuration */
#define MIN_SSYPRI	(1)
#define MAX_SSYPRI	(CFN_MAX_SSYPRI)
#define NUM_SSYPRI	(MAX_SSYPRI)
#define CHK_SSYPRI(pri)	((MIN_SSYPRI) <= (pri) && (pri) <= (MAX_SSYPRI))


/*
 * Check parameter
 *   0: Do not check parameter
 *   1: Check parameter
 */
#define CHK_NOSPT	(1)	/* Check unsupported function (E_NOSPT) */
#define CHK_RSATR	(1)	/* Check reservation attribute error (E_RSATR) */
#define CHK_PAR		(1)	/* Check parameter (E_PAR) */
#define CHK_ID		(1)	/* Check object ID range (E_ID) */
#define CHK_OACV	(1)	/* Check Object Access Violation (E_OACV) */
#define CHK_CTX		(1)	/* Check whether task-independent part is running (E_CTX) */
#define CHK_CTX1	(1)	/* Check dispatch disable part */
#define CHK_CTX2	(1)	/* Check task independent part */
#define CHK_SELF	(1)	/* Check if its own task is specified (E_OBJ) */

/*
 * Debugger support function
 *   0: Invalid
 *   1: Valid
 */
#define USE_DBGSPT		(0)

/* Use object name (Add object name to each control block) */
#define USE_OBJECT_NAME		(0)	/* 0: Do not use object name */
					/* 1: Use object name */
#define OBJECT_NAME_LENGTH	(8)	/* Object name length in each control block */

/*
 * Output (error) messages from micro T-Kernel
 *   0: Do not output message
 *   1: Output message
 */
#define USE_KERNEL_MESSAGE	(1)



#endif /* _CONFIG_ */
