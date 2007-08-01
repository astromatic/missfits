 /*
 				file.h

*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	Part of:	MissFITS
*
*	Author:		E.BERTIN (IAP) C. MARMO (IAP)
*
*	Contents:	include for file.c.
*
*	Last modify:	24/10/2006
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
extern catstruct	**load_fitsfiles(char *name, int *ncat, int *outcat,
				filenum *filetype);

extern int		read_aschead(char *filename, int frameno,
				tabstruct *tab);

extern void		save_fitsfiles(char *name, int t, int nfile,
                                catstruct *cat, filenum filetype);
