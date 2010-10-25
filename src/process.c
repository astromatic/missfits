/*
*				process.c
*
* Functions related to image processing.
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	This file part of:	MissFITS
*
*	Copyright:		(C) 2006-2010 Emmanuel Bertin -- IAP/CNRS/UPMC
*				& Chiara Marmo -- IAP/CNRS
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

#ifdef HAVE_CONFIG_H
#include	"config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
#include <math.h>
 
#include "define.h"
#include "globals.h"
#include "custom.h"
#include "fits/fitscat.h"
#include "file.h"
#include "prefs.h"
#include "process.h"

/******** reduce_bitpix ***************************************************
*/
void    *reduce_bitpix(tabstruct *tab)

  {
   catstruct        *cat;
   int              /* bk, bktot, bufsize, */ npix, i;
   /* long int         *dim; */
   PIXTYPE          *pix /*, *pixt */;
   PIXTYPE          min, max, bscale, bzero, invbscale;


  cat = tab->cat;

  npix = tab->tabsize / tab->bytepix;

/*
  if (!tab->bodybuf)
    {
    QMALLOC(tab->bodybuf, char, tab->tabsize);
    QFREAD(tab->bodybuf, tab->tabsize, tab->tabcat->file, cat->filename);
    if (bswapflag)
      swapbytes(tab->bodybuf, tab->bytepix, tab->tabsize/tab->bytepix);
    }

  npix = tab->tabsize / tab->bytepix;
  bs = (PIXTYPE)tab->bscale;
  bz = (PIXTYPE)tab->bzero;
  bufdata = tab->bodybuf;
  if (tab->bitpix==BP_FLOAT)
    {
    if (bswapflag)
      swapbytes(bufdata, 4, npix);
    for (i=npix; i--; bufdata += sizeof(float))
      *(ptr++) = ((0x7f800000&*(unsigned int *)bufdata) == 0x7f800000)?
		-BIG : *((float *)bufdata)*bs + bz;
    }
  else if (tab->bitpix==BP_DOUBLE)
    {
    swapbytes(bufdata, 8, spoonful);
    for (i=spoonful; i--; bufdata += sizeof(double))
      *(ptr++) = ((0x7ff00000 & *(unsigned int *)(bufdata+4)) == 0x7ff00000)?
		-BIG : *((double *)bufdata)*bs + bz;

  QMALLOC(shortpix, short, npix);

*/

  QMALLOC(pix, PIXTYPE, npix);

  min = BIG;
  max = -BIG;

/*   pix = (PIXTYPE *)tab->bodybuf; */

  read_body(tab, pix, npix);

  for (i=0; i<npix ; i++)
    {
    if (pix[i] > max)
      max = pix[i];
    if (pix[i] < min)
      min = pix[i];
    }
  
  printf ("%e %e\n",min, max);
 
  bscale = (max - min) / 65535.0;
  bzero = max - (bscale*32767.0);
  invbscale = fabs(bscale)>0.0 ? 1.0/bscale : 1.0;


  tab->tabsize = npix * tab->bytepix;
  tab->bitpix = BP_SHORT;
  tab->bytepix = tab->bitpix / 8;
  tab->bscale = bscale;
  tab->bzero = bzero;
  
/*
  QMALLOC(shortpix, short, bufsize);


  QFSEEK(cat->file, tab->bodypos, SEEK_SET, cat->filename);
  for (bk=0; bk < bktot ; bk++)
    {
    read_body(tab, pix, bufsize);
    for (i=0; i<bufsize; i++)
      shortpix[i] = (short)((pix[i]*invbscale+bzero);

    QFSEEK(cat->file, tab->bodypos+bk*bufsize, SEEK_SET, cat->filename);
    write_body(tab, (short int)&pixt, bufsize); 
    }
*/

  addkeywordto_head(tab, "BZERO  ", " ");
  fitswrite(tab->headbuf,"BZERO  ", &tab->bzero, H_EXPO, T_DOUBLE);
  addkeywordto_head(tab, "BSCALE ", " ");
  fitswrite(tab->headbuf,"BSCALE ", &tab->bscale, H_EXPO, T_DOUBLE);

  free(pix);

  return(0);
  }
