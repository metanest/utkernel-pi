/*
 *	micro T-Kernel function code
 */

#ifndef _TKFNCD_H_
#define _TKFNCD_H_

#if INT_BITWIDTH == 16
#include "tkfncd16.h"
#else
#include "tkfncd32.h"
#endif

#endif /* _TKFNCD_H_ */
