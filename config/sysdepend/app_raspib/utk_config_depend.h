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
 *	utk_config_depend.h (Raspberry Pi Type B)
 *	System Configuration Definition
 */


/* ROMINFO */
#define SYSTEMAREA_TOP	0x00000000	/* RAM system area top */
#define SYSTEMAREA_END	0x02000000	/* RAM system area end */

/* User definition */
#define RI_USERAREA_TOP	0x04000000	/* RAM user area top */
#define RI_USERINIT	NULL	/* User initialization program */


/* SYSCONF */
#define CFN_TIMER_PERIOD	10  /* milliseconds */
#define CFN_MAX_TSKID		32
#define CFN_MAX_SEMID		16
#define CFN_MAX_FLGID		16
#define CFN_MAX_MBXID		8
#define CFN_MAX_MTXID		2
#define CFN_MAX_MBFID		8
#define CFN_MAX_PORID		4
#define CFN_MAX_MPLID		2
#define CFN_MAX_MPFID		8
#define CFN_MAX_CYCID		4
#define CFN_MAX_ALMID		8
#define CFN_MAX_SSYID		4
#define CFN_MAX_SSYPRI		16

#define CFN_MAX_REGDEV		(8)
#define CFN_MAX_OPNDEV		(16)
#define CFN_MAX_REQDEV		(16)
#define CFN_DEVT_MBFSZ0		(-1)
#define CFN_DEVT_MBFSZ1		(-1)

#define CFN_VER_MAKER		0
#define CFN_VER_PRID		0
#define CFN_VER_SPVER		0x6101
#define CFN_VER_PRVER		0x0101
#define CFN_VER_PRNO1		0
#define CFN_VER_PRNO2		0
#define CFN_VER_PRNO3		0
#define CFN_VER_PRNO4		0

#define CFN_REALMEMEND		((void *)0x02000000)

/*
 * Initial task priority
 */
#define INIT_TASK_PRI	(MAX_PRI-2)

/*
 * Use zero-clear bss section
 */
#define USE_NOINIT	(1)

/*
 * Stack size for each mode
 */
#define IRQ_STACK_SIZE  2048
#define FIQ_STACK_SIZE    16 /* 4 words */
#define ABT_STACK_SIZE     4 /* 1 word */
#define UND_STACK_SIZE     4 /* 1 word */
#define USR_STACK_SIZE     0 /* not used */

#define EXCEPTION_STACK_TOP 0x04000000

/*
 * Use dynamic memory allocation
 */
#define USE_IMALLOC	(1)

/*
 * Use program trace function (in debugger support)
 */
#define USE_HOOK_TRACE	(0)

/*
 * Use clean-up sequence
 */
#define USE_CLEANUP	(1)


/*
 * Use high level programming language support routine
 */
#define USE_HLL_INTHDR	(1)

