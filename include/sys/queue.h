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
 *	@(#)queue.h (sys)
 *
 *	Queuing operation
 */

#ifndef	__SYS_QUEUE_H__
#define __SYS_QUEUE_H__

#include <basic.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
 * Double-link queue (ring)
 */
typedef struct queue {
	struct queue	*next;
	struct queue	*prev;
} QUEUE;

/*
 * Queue initialization 
 */
Inline void QueInit( QUEUE *que )
{
	que->next = (struct queue *)que;
	que->prev = (struct queue *)que;
}

/*
 * TRUE if the queue is empty 
 */
Inline BOOL isQueEmpty( QUEUE *que )
{
	return ( que->next == que )? TRUE: FALSE;
}

/*
 * Insert in queue 
 *	Inserts entry directly prior to que 
 */
Inline void QueInsert( QUEUE *entry, QUEUE *que )
{
	entry->prev = (struct queue*) que->prev;
	entry->next = que;
	que->prev->next = entry;
	que->prev = entry;
}

/*
 * Delete from queue 
 *	Deletes entry from queue 
 *	No action is performed if entry is empty. 
 */
Inline void QueRemove( QUEUE *entry )
{
	if ( entry->next != entry ) {
		entry->prev->next = (struct queue*) entry->next;
		entry->next->prev = (struct queue*) entry->prev;
	}
}

/*
 * Remove top entry 
 *	Deletes the entry directly after que from the queue,
 *	and returns the deleted entry.
 *	Returns NULL if que is empty.
 */
Inline QUEUE* QueRemoveNext( QUEUE *que )
{
	QUEUE	*entry;

	if ( que->next == que ) {
		return NULL;
	}

	entry = que->next;
	que->next = (struct queue*)entry->next;
	entry->next->prev = que;

	return entry;
}

#ifdef __cplusplus
}
#endif
#endif /* __SYS_QUEUE_H__ */
