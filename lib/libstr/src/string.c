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
 *	@(#)string.c (libstr)
 *	Standard string library (for kernel link) 
 */

/*
 *	This file is linked to the kernel only when the
 *	C standard library is not used.
 */
/** [BEGIN Common Definitions] */
#include <basic.h>
#include <tk/tkernel.h>

#include "libstr_config.h"

/*** macros ***/
/* text evaluation and conversion macro */
#define _TO_UPPER_CASE(c) \
        ( (((c) >= 'a') && ((c) <= 'z')) ? ((c) + ('A' - 'a')) : (c) )
#define _IS_SPACE_CODE(c) \
        ( ((c) == ' ') || ((c) == '\t') )
/** [END Common Definitions] */

/*** binary operation ***/
#ifdef USE_FUNC_MEMSET
/* memset : fill memory area */
void*
memset( void *s, int c, size_t n )
{
	register unsigned char *cp, cval;
	register unsigned long *lp, lval;

	cp = (unsigned char *)s;
	cval = (unsigned char)c;
	
	if (n < 8) {
		while (n-- > 0) {
			*cp++ = cval;
		}
		return s;
	}

	while ((long)cp % 4) {
		--n;
		*cp++ = cval;
	}

	lp = (unsigned long *)cp;
	lval = (unsigned long)cval |
		(unsigned long)cval << 8 |
		(unsigned long)cval << 16 |
		(unsigned long)cval << 24;

	while (n >= 4) {
		*lp++ = lval;
		n -= 4;
	}

	cp = (unsigned char *)lp;
	while (n) {
		*cp++ = cval;
		--n;
	}

	return s;
}
#endif /* USE_FUNC_MEMSET */

#ifdef USE_FUNC_MEMCMP
/* memcmp : perform memory comparison */
int
memcmp( const void *s1, const void *s2, size_t n )
{
	register int result;
	register unsigned char *cp1, *cp2;

	cp1 = (unsigned char *)s1;
	cp2 = (unsigned char *)s2;
	while (n-- > 0) {
		result = *cp1++ - *cp2++;
		if (result) {
			return result;
		}
	}
	return 0;
}
#endif /* USE_FUNC_MEMCMP */

#ifdef USE_FUNC_MEMCPY
/* memcpy : copy memory */
void*
memcpy( void *dst, const void *src, size_t n )
{
	register unsigned char *cdst, *csrc;

	cdst = (unsigned char *)dst;
	csrc = (unsigned char *)src;
	while (n-- > 0) {
		*cdst++ = *csrc++;
	}

	return dst;
}
#endif /* USE_FUNC_MEMCPY */

#ifdef USE_FUNC_MEMMOVE
/* memmove : move memory */
void*
memmove( void *dst, const void *src, size_t n )
{
	register unsigned char *cdst, *csrc;

	cdst = (unsigned char *)dst;
	csrc = (unsigned char *)src;
	if (csrc < cdst) {
		cdst += n;
		csrc += n;
		while (n-- > 0) {
			*--cdst = *--csrc;
		}
	} else {
		while (n-- > 0) {
			*cdst++ = *csrc++;
		}
	}

	return dst;
}
#endif /* USE_FUNC_MEMMOVE */

#ifdef USE_FUNC_BZERO
extern void* memset( void *s, int c, size_t n );

/* bzero : zero clear memory area */
void
bzero( void *s, size_t n )
{
	memset(s, 0, n);
}
#endif /* USE_FUNC_BZERO */

/*** text string operation ***/
#ifdef USE_FUNC_STRLEN
/* strlen : get text string length */
size_t
strlen( const char *s )
{
	register char *cp;

	cp = (char *)s;
	while (*cp) {
		++cp;
	}
	return (size_t)(cp - s);
}
#endif /* USE_FUNC_STRLEN */

#ifdef USE_FUNC_STRCMP
/* strcmp : perform text string comparison */
int
strcmp( const char *s1, const char *s2 )
{
	register int result;

	while (*s1) {
		result = (unsigned char)*s1++ - (unsigned char)*s2++;
		if (result) {
			return result;
		}
	}

	return (unsigned char)*s1 - (unsigned char)*s2;
}
#endif /* USE_FUNC_STRCMP */

#ifdef USE_FUNC_STRNCMP
/* strncmp : perform text string comparison of specified length */
int
strncmp( const char *s1, const char *s2, size_t n )
{
	register int result = 0;

	while (*s1) {
		if (n-- <= 0) {
			return result;
		}
		result = (unsigned char)*s1++ - (unsigned char)*s2++;
		if (result) {
			return result;
		}
	}

	if (n != 0) {
		return (unsigned char)*s1 - (unsigned char)*s2;
	} else {
		return 0;
	}
}
#endif /* USE_FUNC_STRNCMP */

#ifdef USE_FUNC_STRCPY
/* strcpy : copy text string */
char*
strcpy( char *dst, const char *src )
{
	register char *cp;

	cp = dst;
	do {
		*cp++ = *src;
	} while (*src++);

	return dst;
}
#endif /* USE_FUNC_STRCPY */

#ifdef USE_FUNC_STRNCPY
/* strncpy : copy text string of specified length */
char*
strncpy( char *dst, const char *src, size_t n )
{
	register char *cp;

	cp = dst;
	do {
		if (n-- <= 0) {
			return dst;
		}
		*cp++ = *src;
	} while (*src++);

	while (n-- > 0) {
		*cp++ = 0;
	}

	return dst;
}
#endif /* USE_FUNC_STRNCPY */

#ifdef USE_FUNC_STRCAT
/* strcat : perform text string concatenation */
char*
strcat( char *dst, const char *src )
{
	register char *cp;

	cp = dst;
	while (*cp) {
		++cp;
	}

	while (*src) {
		*cp++ = *src++;
	}
	*cp = '\0';

	return dst;
}
#endif /* USE_FUNC_STRCAT */

#ifdef USE_FUNC_STRNCAT
/* strncat : perform concatenation on text string of specified length */
char*
strncat( char *dst, const char *src, size_t n )
{
	register char *cp;

	cp = dst;
	while (*cp) {
		++cp;
	}

	while (*src) {
		if (n-- <= 0) {
			break;
		}
		*cp++ = *src++;
	}
	*cp = '\0';

	return dst;
}
#endif /* USE_FUNC_STRNCAT */

#ifdef USE_FUNC_STRTOL
/* strtol : convert text string to integer value (long int) */
long int
strtol( const char *nptr, char **endptr, int base )
{
	const char *num_table = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	long int value = 0;
	int sign = 1, i;
	char *cp;

	while (_IS_SPACE_CODE(*nptr)) {
		++nptr;
	}

	switch (*nptr) {
	  case '-':
		sign = -1;
		/* no break */
	  case '+':
		++nptr;
		/* no break */
	  default:
		break;
	}

	if (base == 16) {
		if (*nptr == '0') {
			++nptr;
			if (_TO_UPPER_CASE(*nptr) != 'X') {
				goto PARSE_START;
			}
			++nptr;
		}
	}

	if (base == 0) {
		if (*nptr == '0') {
			++nptr;
			if (_TO_UPPER_CASE(*nptr) == 'X') {
				++nptr;
				base = 16;
			} else {
				base = 8;
			}
		} else {
			base = 10;
		}
	}

	if ((base < 2) || (base > 36)) {
		base = 10;
	}

PARSE_START:
	while (*nptr != '\0') {
		cp = (char *)num_table;
		for (i = 0; i < base; ++i) {
			if (_TO_UPPER_CASE(*nptr) == *cp) {
				break;
			}
			++cp;
		}
		if (i >= base) {
			goto PARSE_END;
		}

		value = value * base + i;
		++nptr;
	}

PARSE_END:
	if (endptr != NULL) {
		*endptr = (char *)nptr;
	}
	return value * sign;
}
#endif /* USE_FUNC_STRTOL */
