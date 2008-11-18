 /*
 				process.h

*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	Part of:	MissFITS
*
*	Author:		C. MARMO (IAP)
*
*	Contents:	include for process.c.
*
*	Last modify:	03/08/2007
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#ifndef _FITSCAT_H_
#include "fits/fitscat.h"
#endif

#ifndef _PREFS_H_
#include "prefs.h"
#endif

/*------------------------------- functions ---------------------------------*/

extern void		    *reduce_bitpix(tabstruct *tab);
