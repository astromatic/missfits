/*
                                  custom.c

*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*       Part of:        MissFITS
*
*       Author:         E.BERTIN (IAP)
*
*       Contents:       Custom functions
*
*       Last modify:    29/06/2002
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#ifdef	HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 
#include "define.h"
#include "globals.h"
#include "custom.h"
#include "fits/fitscat.h"
#include "fitswcs.h"
#include "prefs.h"

/****** fix_wfi *******************************************************
PROTO	void fix_wfi(tabstruct *tab)
PURPOSE	Fix WFI image headers (astrometric information especially)
INPUT	Pointer to the image tabstruct.
OUTPUT	-.
NOTES	-.
AUTHOR	E. Bertin (IAP)
VERSION	29/06/2002
 ***/
void	fix_wfi(tabstruct *tab)
  {
   char		str[82];
   double	dval;

/* Update CRVAL1 and CRVAL2 using the sexagedecimal strings RA and DEC */
  if (fitsread(tab->headbuf, "RA      ", str, H_STRING, T_STRING)==RETURN_OK)
    {
    dval = sextodegal(str);
    addkeywordto_head(tab, "CRVAL1  ",
			"Projection center in world coordinates");
    fitswrite(tab->headbuf, "CRVAL1  ", &dval, H_EXPO, T_DOUBLE);
    }
  if (fitsread(tab->headbuf, "DEC     ", str, H_STRING, T_STRING)==RETURN_OK)
    {
    dval = sextodegde(str);
    addkeywordto_head(tab, "CRVAL2  ",
			"Projection center in world coordinates");
    fitswrite(tab->headbuf, "CRVAL2  ", &dval, H_EXPO, T_DOUBLE);
    }

/* Take care of CTYPEx */
  addkeywordto_head(tab, "CTYPE1  ", "World coordinate type");
  addkeywordto_head(tab, "CTYPE2  ", "World coordinate type");
  fitswrite(tab->headbuf, "CTYPE1  ", "RA---TAN", H_STRING, T_STRING);
  fitswrite(tab->headbuf, "CTYPE2  ", "DEC--TAN", H_STRING, T_STRING);
/* Take care of CDELTx */
  addkeywordto_head(tab, "CDELT1  ", "Pixel step in world coordinates");
  addkeywordto_head(tab, "CDELT2  ", "Pixel step in world coordinates");
  dval = -WFI_PIXSCALE;
  fitswrite(tab->headbuf, "CDELT1  ", &dval, H_EXPO, T_DOUBLE);
  dval = WFI_PIXSCALE;
  fitswrite(tab->headbuf, "CDELT2  ", &dval, H_EXPO, T_DOUBLE);

  return;
  }

