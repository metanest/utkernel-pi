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
 * modified for Raspberry Pi Type B by Makoto Kishimoto
 */

/*
 *      @(#)str_align_common.h (sys)
 *
 *	Bit alignment definitions for structure 
 */

#ifndef __SYS_STR_ALIGN_COMMON_H__
#define __SYS_STR_ALIGN_COMMON_H__

#ifdef APP_AT91
#  include <sys/sysdepend/app_at91/str_align_depend.h>
#endif

#ifdef APP_H8S2212
#  include <sys/sysdepend/app_h8s2212/str_align_depend.h>
#endif

#ifdef APP_RASPIB
#  include <sys/sysdepend/app_raspib/str_align_depend.h>
#endif

#endif /* __SYS_STR_ALIGN_COMMON_H__ */
