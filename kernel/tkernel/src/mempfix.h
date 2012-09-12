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
 *	mempfix.h
 *	Fixed Size Memory Pool
 */

#ifndef _MEMPFIX_H_
#define _MEMPFIX_H_

/*
 * Fixed size memory pool control block
 */
typedef struct free_list {
	struct free_list *next;
} FREEL;

typedef struct fix_memorypool_control_block {
	QUEUE	wait_queue;	/* Memory pool wait queue */
	ID	mpfid;		/* Fixed size memory pool ID */
	VP	exinf;		/* Extended information */
	ATR	mpfatr;		/* Memory pool attribute */
	W	mpfcnt;		/* Number of blocks in whole memory pool */
	W	blfsz;		/* Fixed size memory block size */
	W	mpfsz;		/* Whole memory pool size */
	W	frbcnt;		/* Number of blocks in free area */
	VP	mempool;	/* Top address of memory pool */
	VP	unused;		/* Top address of unused area */
	FREEL	*freelist;	/* Free block list */
	OBJLOCK	lock;		/* Lock for object exclusive access */
#if USE_OBJECT_NAME
	UB	name[OBJECT_NAME_LENGTH];	/* name */
#endif
} MPFCB;

IMPORT MPFCB knl_mpfcb_table[];	/* Fixed size memory pool control block */
IMPORT QUEUE knl_free_mpfcb;	/* FreeQue */

#define get_mpfcb(id)	( &knl_mpfcb_table[INDEX_MPF(id)] )


#define MINSIZE		( sizeof(FREEL) )
#define MINSZ(sz)	( ((UW)(sz) + (UW)(MINSIZE-1)) & ~(UW)(MINSIZE-1) )

/*
 * Return end address in memory pool area
 */
Inline VP knl_mempool_end( MPFCB *mpfcb )
{
	return (VB*)mpfcb->mempool + mpfcb->mpfsz;
}



#endif /* _MEMPFIX_H_ */
