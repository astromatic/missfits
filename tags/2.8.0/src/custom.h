/*
*				custom.h
*
* Include file for custom.c.
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
