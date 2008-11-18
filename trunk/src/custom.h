 /*
 				custom.h

*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	Part of:	MissFITS
*
*	Author:		E.BERTIN (IAP)
*
*	Contents:	include for custom.c.
*
*	Last modify:	29/06/2001
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#ifndef _FITSCAT_H_
#include "fits/fitscat.h"
#endif

#ifndef _PREFS_H_
#include "prefs.h"
#endif

/*------------------------------- constants ---------------------------------*/
#define		WFI_PIXSCALE	(0.238*ARCSEC/DEG)

/*------------------------------- functions ---------------------------------*/
void	fix_wfi(tabstruct *tab);
