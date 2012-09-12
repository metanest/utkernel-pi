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
 *	@(#)imalloc.h (sys)
 *
 *	Kernel memory allocation 
 *
 *	A function for allocating memory used in parts of micro T-Kernel.
 *	Not for general use.
 */

#ifndef __SYS_IMALLOC_H__
#define __SYS_IMALLOC_H__

#include <basic.h>
#include <tk/typedef.h>

#ifdef __cplusplus
extern "C" {
#endif

IMPORT void* knl_Imalloc( size_t size );
IMPORT void* knl_Icalloc( size_t nmemb, size_t size );
IMPORT void  knl_Ifree( void *ptr );

#ifdef __cplusplus
}
#endif
#endif /* __SYS_IMALLOC_H__ */
