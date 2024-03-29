/*
*				prefs.h
*
* Include file for prefs.c.
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

#ifndef _PREFS_H_
#define _PREFS_H_

/*--------------------------------- typedefs --------------------------------*/
typedef  enum {FILE_SAME,FILE_MULTI,FILE_SPLIT,FILE_SLICE,FILE_CUBE,FILE_DIR} 
               filenum;

/*------------------------------- preferences -------------------------------*/
typedef struct
  {
  char		**command_line;		    /* Command line */
  int		ncommand_line;	            /* nb of params */
  char		prefs_name[MAXCHAR];   	    /* prefs filename*/
  char		*(file_name[MAXFILE]);	    /* Filename(s) of input images */
  int		nfile;			    /* Number of input images */
  enum {CHECKSUM_NONE,CHECKSUM_COMPUTE,CHECKSUM_VERIFY,CHECKSUM_UPDATE}
		checksum_type;		    /* Checksum option */
  char		*(display_key[MAXKEYWORD]); /* Keyword display */
  int		ndisplay_key;		    /* nb of params */
  char		new_suffix[MAXCHAR];	    /* Generic suffix for new files */
  int		next_min;		    /* Minimum number of FITS extens. */
  enum {PROCESS_NONE,PROCESS_TOBITPIX16,PROCESS_COMPRESS,PROCESS_UNCOMPRESS}
		process_type;		    /* Data processing option */
  char		*(remove_key[MAXKEYWORD]);  /* Keyword removal */
  int		nremove_key;		    /* nb of params */
  char		*(replace_key[MAXKEYWORD]); /* Keyword translations */
  int		nreplace_key;		    /* nb of params */
  char		old_key[8][MAXKEYWORD];	    /* Keywords to be replaced */
  filenum	outfile_type;		    /* Grouping/splitting options */
  char		new_key[8][MAXKEYWORD];	    /* Replacing keywords */
  char		*(slice_key[MAXKEYWORD]);   /* Keyword translations */
  int		nslice_key;                 /* nb of params */
  char		oldslice_key[8][MAXKEYWORD];  /* Keywords to be sliced */
  char		newslice_key[8][MAXKEYWORD];  /* Sliced keyword */
  char		slicekey_format[MAXCHAR];   /* Suffix format for splitted key */
  enum {SAVE_NONE,SAVE_NEW,SAVE_BACKUP,SAVE_REPLACE}
		save_type;		    /* Save option */
  enum {QUIET, NORM, FULL} verbose_type;	/* display type */
  char		split_format[MAXCHAR];	    /* Suffix format for splitted files */
  int		split_start;		    /* First extension suffix number */
  char		slice_format[MAXCHAR];	    /* Suffix format for splitted files */
  int		slice_start;		    /* First slice suffix number */
  int		fixwfi_flag;		    /* Fix WFI headers? */
  char		head_suffix[MAXCHAR];	    /* Generic suffix for FITS headers */
/* Multithreading */
  int		nthreads;		    /* Number of active threads */
/* Misc */
  int		xml_flag;	            /* Write XML file? */
  char		xml_name[MAXCHAR];          /* XML file name */
  char		xsl_name[MAXCHAR];          /* XSL file name (or URL) */
  char		sdate_start[12];            /* missfits start date */
  char		stime_start[12];            /* missfits start time */
  char		sdate_end[12];		    /* missfits end date */
  char		stime_end[12];		    /* missfits end time */
  double	time_diff;		    /* Execution time */
  }	prefstruct;

prefstruct	prefs;

/*----------------------------- Internal constants --------------------------*/

#define		MAXLIST		32	/* max. nb of list members */

/*-------------------------------- protos -----------------------------------*/
extern int	cistrcmp(char *cs, char *ct, int mode);

extern void	dumpprefs(int state),
		readprefs(char *filename,char **argkey,char **argval,int narg),
		useprefs(void),
                endprefs(void);
extern char     *fitsroot(char *str);

#endif

