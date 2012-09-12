/*
 *	micro T-Kernel SVC interface library (at91)
 *
 *	   created from /usr/local/te/include/tk/dbgspt.h
 */

#include <machine.h>
#include <tk/sysdef.h>
#include <sys/svc/tdfncd.h>
#include "utk_config.h"

#if USE_TRAP || (USE_DBGSPT && USE_HOOK_TRACE)
	.text
	.balign	4
	.globl	Csym(td_get_tim)
	.type	Csym(td_get_tim), %function
Csym(td_get_tim):
	stmfd	sp!, {lr}
	ldr	ip, =TDFN_GET_TIM
#if USE_TRAP
	swi	SWI_DEBUG
#else
	bl	Csym(knl_call_dbgspt)
#endif
	ldmfd	sp!, {lr}
	bx	lr
#endif

