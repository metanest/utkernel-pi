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
 *	@(#)sysdef_common.h (tk)
 *
 *	System dependencies definition.
 *	Included also from assembler program.
 */

#ifndef __TK_SYSDEF_COMMON_H__
#define __TK_SYSDEF_COMMON_H__

#if APP_AT91
#  include <tk/sysdepend/app_at91/sysdef_depend.h>
#endif

#if APP_H8S2212
#  include <tk/sysdepend/app_h8s2212/sysdef_depend.h>
#endif

#if APP_RASPIB
#  include <tk/sysdepend/app_raspib/sysdef_depend.h>
#endif

#endif /* __TK_SYSDEF_COMMON_H__ */
