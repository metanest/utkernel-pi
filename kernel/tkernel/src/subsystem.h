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
 *	subsystem.h
 *	Subsystem Manager
 */

#ifndef _SUBSYSTEM_H_
#define _SUBSYSTEM_H_

typedef INT  (*SVC)( VP pk_para, FN fncd );	/* Extended SVC handler */

/*
 * Definition of subsystem control block
 */
typedef struct subsystem_control_block	SSYCB;
struct subsystem_control_block {
	SVC	svchdr;		/* Extended SVC handler */
#if TA_GP
	VP	gp;		/* Global pointer */
#endif
};

IMPORT SSYCB knl_ssycb_table[];	/* Subsystem control block */

#define get_ssycb(id)	( &knl_ssycb_table[INDEX_SSY(id)] )

/*
 * Undefined extended SVC function 
 */
IMPORT INT knl_no_support();

#endif /* _SUBSYSTEM_H_ */
