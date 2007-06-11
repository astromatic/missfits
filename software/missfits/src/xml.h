/*
                                 xml.h

*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	Part of:	MissFITS
*
*	Author:		E.BERTIN (IAP) C.MARMO (IAP)
*
*	Contents:	XML logging.
*
*	Last modify:	31/05/2007
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

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
  filenum      infiletype;                  /* input file type */
  filenum      outfiletype;                 /* output file type */
  int          in_next;                     /* input extensions */
  int          out_next;                    /* output extensions */
  int          in_nslice;                   /* input slices */
  int          out_nslice;                  /* output slices */
  int          exthead;                     /* external header? */
  char	       *(display_key[MAXKEYWORD]);  /* Keyword display */
  char	       *(display_value[MAXKEYWORD]);/* Keyword display */
  }	xmlstruct;
/*------------------------------- functions ---------------------------------*/

extern int	init_xml(int nfile),
                update_xml(char *filename, filenum filetype, int ntab),
                write_xml(),
                write_xml_header(FILE *file),
                write_xml_meta(FILE *file, char *error),
                write_xml_keyword(FILE *file, char *error),
                write_xmlconfigparam(FILE *file, char *name, char *unit,
                                char *ucd, char *format),
                end_xml(void);
extern void	write_xmlerror(char *filename, char *error);