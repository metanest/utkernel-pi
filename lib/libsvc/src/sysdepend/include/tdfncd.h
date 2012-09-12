/*
 *	micro T-Kernel function code (Debugger Support)
 */

#ifndef _TDFNCD_H_
#define _TDFNCD_H_

#if INT_BITWIDTH == 16
#include "tdfncd16.h"
#else
#include "tdfncd32.h"
#endif

#endif /* _TDFNCD_H_ */
