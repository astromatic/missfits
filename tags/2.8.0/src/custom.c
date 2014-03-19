/*
*				custom.c
*
* Custom functions.
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	This file part of:	MissFITS
*
*	Copyright:		(C) 2002-2010 Emmanuel Bertin -- IAP/CNRS/UPMC
*
*	License:		GNU General Public License
*
*	MissFITS is free software: you can redistribute it and/or modify
*	it under the terms of the GNU General Public License as published by
*	the Free Software Foundation, either version 3 of the License, or
* 	(at your option) any later version.
*	MissFITS is distributed in the hope that it will be useful,
*	but WITHOUT ANY WARRANTY; without even the implied warranty of
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*	GNU General Public License for more details.
*	You should have received a copy of the GNU General Public License
*	along with MissFITS. If not, see <http://www.gnu.org/licenses/>.
*
*	Last modified:		10/10/2010
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

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

