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
 *	@(#)dbgspt_common.h (tk)
 *
 *	micro T-Kernel Debugger Support
 */

#ifndef __TK_DBGSPT_COMMON_H__
#define __TK_DBGSPT_COMMON_H__

/*
 * System-dependent definition
 */
#if APP_AT91
#  include <tk/sysdepend/app_at91/dbgspt_depend.h>
#endif

#if APP_H8S2212
#  include <tk/sysdepend/app_h8s2212/dbgspt_depend.h>
#endif

#if APP_RASPIB
#  include <tk/sysdepend/app_raspib/dbgspt_depend.h>
#endif

#endif /* __TK_DBGSPT_COMMON_H__ */
