/*
*				xml.h
*
* Include file for xml.c
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	This file part of:	MissFITS
*
*	Copyright:		(C) 2006-2010 Chiara Marmo -- IAP/CNRS
*				& Emmanuel Bertin -- IAP/CNRS/UPMC
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

#include "prefs.h"

/*----------------------------- Internal constants --------------------------*/
#ifndef XSL_URL
#define	XSL_URL	"."
#endif
/*--------------------------------- typedefs --------------------------------*/
typedef struct
  {
  char         filename[80];                /* name of the file */
  char         infiletype[8];               /* input file type */
  char         outfiletype[8];              /* output file type */
  int          in_next;                     /* input extensions */
  int          out_next;                    /* output extensions */
  int          in_nslice;                   /* input slices */
  int          out_nslice;                  /* output slices */
  int          extheadflag;                 /* external header? */
  char	       **display_value;             /* Keyword display */
  }	xmlstruct;

typedef struct
  {
  char	       display_key[MAXCHAR];  /* Keyword to display */
  char	       display_value[MAXCHAR];/* Value of displayed keyword */
  }	xmlkeystruct;
/*------------------------------- functions ---------------------------------*/

extern int	init_xml(int nfile),
                update_xml(char *name, int t, int nfile, catstruct *cat,
                                          catstruct *incat, filenum filetype,
                                          xmlkeystruct *xmlkey, int headflag),
                update_dimxml(catstruct *cat, catstruct *incat, xmlstruct *x),
                write_xml(char *filename),
                write_xml_header(FILE *file),
                write_xml_meta(FILE *file, char *error),
                write_xml_keyword(FILE *file, char *error),
                write_xmlconfigparam(FILE *file, char *name, char *unit,
                                char *ucd, char *format),
                end_xml(void);
extern void	write_xmlerror(char *filename, char *error);
