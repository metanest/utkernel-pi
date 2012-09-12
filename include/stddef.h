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
 *	@(#)stddef.h
 *
 *	C language: common definitions 
 */

#ifndef __STDDEF_H__
#define __STDDEF_H__

#include <stdtype.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int		ptrdiff_t;

#ifdef	__size_t
typedef __size_t	size_t;
#undef	__size_t
#endif

#ifdef	__wchar_t
typedef __wchar_t	wchar_t;
#undef	__wchar_t
#endif

#define offsetof(type, member)	( (size_t)(&((type *)0)->member) )
#define NULL	0

#ifdef __cplusplus
}
#endif
#endif /* __STDDEF_H__ */
