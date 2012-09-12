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
 *	@(#)machine_common.h (sys)
 *
 *	Machine type definition
 */

#ifndef __SYS_MACHINE_COMMON_H__
#define __SYS_MACHINE_COMMON_H__

#ifdef _APP_AT91_
#  include <sys/sysdepend/app_at91/machine_depend.h>
#endif

#ifdef _APP_H8S2212_
#  include <sys/sysdepend/app_h8s2212/machine_depend.h>
#endif

#ifdef _APP_RASPIB_
#  include <sys/sysdepend/app_raspib/machine_depend.h>
#endif

#endif /* __SYS_MACHINE_COMMON_H__ */
