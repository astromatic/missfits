/*
*				makeit.c
*
* Main loop.
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

#ifdef	HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <math.h>
 
#include "define.h"
#include "globals.h"
#include "custom.h"
#include "fits/fitscat.h"
#include "file.h"
#include "prefs.h"
#include "process.h"
#include "xml.h"

void	print_tabinfo(tabstruct *tab, xmlkeystruct *xmlkey, int no);

/********************************** makeit ***********************************/
void	makeit(void)
  {
   catstruct		**incat,
			*outcat;
   tabstruct		*tab, *intab;
   char	                im[MAXCHAR];
   filenum		filetype;
   int			a,c,k,n,p, t, s, check, narg, nfile, nout,
                        ntabin, ntabout, flagmulti, flagcube, headflag, nxml;
   char                 *pix;
   KINGSIZE_T           size;
   double               minval, maxval;
   xmlkeystruct         *xmlkey=NULL;

  check = flagmulti = flagcube = n = 0;
  minval = maxval = 0.0;

/* Install the signal-catching routines for temporary file cleanup */
#ifdef USE_THREADS
  install_cleanup(NULL);
#else
  install_cleanup(NULL);
#endif

  NFPRINTF(OUTPUT, "");
  QPRINTF(OUTPUT,
        "----- %s %s started on %s at %s with %d thread%s\n\n",
                BANNER,
                MYVERSION,
                prefs.sdate_start,
                prefs.stime_start,
                prefs.nthreads,
                prefs.nthreads>1? "s":"");

/* Load input images */
  narg = prefs.nfile;
  nxml = narg;

/* Go argument by argument */
  NFPRINTF(OUTPUT, "Examining input data...")

  for (a=0; a<narg; a++)
    {
    headflag = 0;
    incat = load_fitsfiles(prefs.file_name[a], &nfile, &nout,
                                &filetype, &headflag);
    if (nfile <=0)
      {
      warning(prefs.file_name[a], " cannot be loaded/expanded, skipping...");
      continue;
      }
    ntabin = incat[0]->ntab;

/*-- Create new FITS */
    for (s=0; s<nout; s++)
      {
      switch (prefs.outfile_type)
        {
        case FILE_SAME:
          outcat = new_cat(1);
          copy_tabs_blind(incat[0], outcat);
          break;            
        case FILE_MULTI:
          outcat = new_cat(1);
          if (ntabin==1)
            init_cat(outcat);
          for (c=0; c<nfile; c++)
            copy_tabs_blind(incat[c], outcat);
          break;
        case FILE_SPLIT:
          flagmulti = 1;
          outcat = new_cat(1);
          copy_tabs_blind(incat[0], outcat);
          if (incat[0]->ntab>1)
            nxml *= (incat[0]->ntab-1);
          else
            nxml *= (incat[0]->ntab); 
          break;
        case FILE_CUBE:
          outcat = new_cat(1);
          copy_tabs_blind(incat[0], outcat);
          break;
        case FILE_SLICE:
          flagcube = 1;
          outcat = new_cat(1);
          copy_tabs_blind(incat[0], outcat);
          nxml *= nout;
          break;            
        case FILE_DIR:
          if (!strcmp(prefs.slice_format,"NONE"))
            {
            flagmulti = 1;
            nxml *= (incat[0]->ntab-1);            
            }
          else if (!strcmp(prefs.split_format,"NONE"))
            {
            flagcube = 1;
            nxml *= nout;
            }
          outcat = new_cat(1);
          copy_tabs_blind(incat[0], outcat);
          break;            
        default:
          error(EXIT_FAILURE, "*Internal Error*:  unknown output file type", "");
        }

      if (prefs.xml_flag && !a && !s)
        init_xml(nxml);

      NFPRINTF(OUTPUT, "Reading input data...");
      ntabout = outcat->ntab;
 
/*---- Check of the number of tables */
      if (ntabout<prefs.next_min+1)
        {
        sprintf(gstr, " misses extensions/files (%d expected, %d found)",
                prefs.next_min, ntabout-1);
        warning(prefs.file_name[a], gstr);
        }
      tab = outcat->tab;
      NFPRINTF(OUTPUT, "");
/*-- Go extension by extension */
      for (t=0; t<ntabout; t++, tab = tab->nexttab)
        {
/*---- Check Checksum */
        if (prefs.checksum_type==CHECKSUM_VERIFY
	  || prefs.checksum_type==CHECKSUM_UPDATE)
          check = verify_checksum(tab);
/*---- Slicing cubes */
        if (flagcube)
          {
          if (tab->naxis>2)
            {
            tab->naxis = 2;
            removekeywordfrom_head(tab, "NAXIS3");
            tab->tabsize = tab->naxisn[0] * tab->naxisn[1] *
                              tab->bytepix ;
            tab->bodypos+= s * tab->tabsize; 
            }
          }
/*---- Building cubes */
        else if (prefs.outfile_type == FILE_CUBE)
          {
          if (tab->naxis>0)
            {
            tab->naxis = 3;
            tab->naxisn[2] = nfile;
            size = tab->naxisn[0] * tab->naxisn[1] * tab->bytepix;
            tab->tabsize = size * nfile;
            addkeywordto_head(tab, "NAXIS3  ", "length of data axis 3");
            tab->bodybuf = malloc(size*nfile);
            pix = tab->bodybuf ;
            for (c=0; c<nfile; c++, pix += size)
              {
              intab = incat[c]->tab->nexttab;
              for (p=t ; --p ; intab = intab->nexttab);
              QFSEEK(incat[c]->file, intab->bodypos, SEEK_SET,
                       incat[c]->filename);
              QFREAD(pix, size, incat[c]->file, incat[c]->filename);
              if (bswapflag)
                swapbytes(pix, tab->bytepix, size/tab->bytepix);
              }
            }
          }
/*---- Bitpix conversion */
/*
        if (tab->naxis>0 && prefs.process_type==PROCESS_TOBITPIX16)
          {
	  if (tab->bitpix!=BP_FLOAT || tab->bitpix!=BP_DOUBLE)
            reduce_bitpix(tab);
          else
            warning(prefs.file_name[a], " is not in floating-point format!");
          }
*/
/*---- Replace keywords */
        for (k=0; k<prefs.nreplace_key; k++)
          if ((n=fitsfind(tab->headbuf, prefs.old_key[k])))
            strncpy(tab->headbuf+n*80, prefs.new_key[k],8);
/*---- Slice keywords */
        for (k=0; k<prefs.nslice_key; k++)
          {
          if (flagcube)
            {
            sprintf(im,prefs.slicekey_format,s+prefs.slice_start);
            sprintf(prefs.oldslice_key[k], "%s%s", prefs.slice_key[k],im);
            if ((n=fitsfind(tab->headbuf, prefs.oldslice_key[k]))!=RETURN_ERROR)
              strncpy(tab->headbuf+n*80, prefs.newslice_key[k],8);
            }
          else if (prefs.outfile_type == FILE_CUBE)
            {
            for (c=0; c<nfile; c++)
              {
              sprintf(im,prefs.slicekey_format,c+prefs.slice_start);
              sprintf(prefs.oldslice_key[k], "%s%s", prefs.slice_key[k],im);
              if (tab->naxis>0)
                {
                if (((n=fitsfind(tab->headbuf, prefs.slice_key[k]))
                            !=RETURN_ERROR) &&
		       fitsfind(tab->headbuf, prefs.oldslice_key[k])
                               ==RETURN_ERROR)
                  strncpy(tab->headbuf+n*80, prefs.oldslice_key[k],strlen(prefs.oldslice_key[k]));
                }
              }
            }
          }
/*---- Remove keywords */
        for (k=0; k<prefs.nremove_key; k++)
          removekeywordfrom_head(tab, prefs.remove_key[k]);
/*---- Fix WFI Astrometric keywords (custom option) */
        if (prefs.fixwfi_flag)
          fix_wfi(tab);
/*---- Print file info */
        if (prefs.xml_flag)
          QCALLOC(xmlkey,xmlkeystruct,prefs.ndisplay_key);
        print_tabinfo(tab, xmlkey, t);
/*---- Print Checksum */
        if (prefs.checksum_type==CHECKSUM_VERIFY
	  || prefs.checksum_type==CHECKSUM_UPDATE)
          printf("Checksum: %s\n", check==RETURN_OK? "OK"
	  			: (check==RETURN_ERROR ? "BAD" : "N/A"));
/*---- Compute new Checksum */
/*---- Make sure that the HDU is up-to-date */
        update_tab(tab);
        update_head(tab);
        if (prefs.checksum_type==CHECKSUM_COMPUTE
	  || prefs.checksum_type==CHECKSUM_UPDATE)
          write_checksum(tab);
        }

/*-- Save the new file */
      save_fitsfiles(prefs.file_name[a], s, nout, outcat,
                     prefs.outfile_type? prefs.outfile_type: filetype);
/*-- Update the xml file */
      if (prefs.xml_flag)
        {
        update_xml(prefs.file_name[a], s, nout, outcat, incat[0],
                     prefs.outfile_type? prefs.outfile_type: filetype,
                     xmlkey, headflag);
        free(xmlkey);
        }
      free_cat(&outcat,1);
      }
    free_cat(incat, nfile);
    }
  cleanup_files();

  return;
  }

/****** print_tabinfo ********************************************************
PROTO	void print_fileinfo(tabstruct *tab, int no)
PURPOSE	Print info about a FITS file
INPUT	Pointer to the tab,
	Extension number.
OUTPUT	-.
NOTES	-.
AUTHOR	E. Bertin (IAP)
VERSION	29/04/2010
 ***/
void	print_tabinfo(tabstruct *tab, xmlkeystruct *xmlkey, int no)

  {
   h_type		htype;
   t_type		ttype;
   char			keyword[12], str[100],tmpkey[12],
			*filename,*rfilename,*tstr,*pstr;
   int			i,n,p;

/* A short, "relative" version of the filename */
  if (tab->cat)
    filename = tab->cat->filename;
  else
    return;
  if (!(rfilename = strrchr(filename, '/')))
    rfilename = filename;
  else
    rfilename++;
  if (no)
    printf("%s [%2d]", rfilename, no);
  else
    printf("%s     ", rfilename);
  if (!tab)
    {
    QPRINTF(OUTPUT, "\n");
    return;
    }

  for (i=0; i<prefs.ndisplay_key; i++)
    {
    QCALLOC(tstr, char, 9);
    strncpy(tstr,prefs.display_key[i],8);
    for (pstr = tstr, p=0; p<8; p++, pstr++)
      if (!*pstr)
        *pstr = ' ';
    sprintf(tmpkey,"%8s",tstr);
    free(tstr);
    
    if ((n=fitsfind(tab->headbuf, tmpkey))>0)
      {
      if (prefs.xml_flag)
        strcpy(xmlkey[i].display_key,prefs.display_key[i]);
      fitspick(tab->headbuf+n*80, keyword, gstr, &htype, &ttype, str);
/*---- Display formatting is a modified version of fitswrite() */
      switch(htype)
        {
        case H_INT:
          printf(" %12d", (ttype==T_SHORT)?
					*(short *)gstr: *(int *)gstr);
          if (prefs.xml_flag)
            sprintf(xmlkey[i].display_value,"%12d", (ttype==T_SHORT)?
					*(short *)gstr: *(int *)gstr);
          break;
        case H_FLOAT:
        case H_EXPO:
          printf(" %12g", (ttype==T_DOUBLE)?
					*(double *)gstr: *(float *)gstr);
          if (prefs.xml_flag)
            sprintf(xmlkey[i].display_value,"%30lf", (ttype==T_DOUBLE)?
					*(double *)gstr: *(float *)gstr);
          break;
        case H_BOOL:
          printf(" %c", *(int *)gstr? '0':'1');
          if (prefs.xml_flag)
            sprintf(xmlkey[i].display_value,"%c", *(int *)gstr? '0':'1');
          break;
        case H_STRING:
        case H_STRINGS:
          printf(" \"%.70s\"", gstr);
          if (prefs.xml_flag)
            sprintf(xmlkey[i].display_value,"%.70s", gstr);
         break;
        case H_COMMENT:
        case H_HCOMMENT:
          break;
        default:
          error(EXIT_FAILURE, "*Internal Error*: Unknown FITS type in ",
				"print_fileinfo()");
        }
      }
    else
      {
      printf(" xxx");
      if (prefs.xml_flag)
        sprintf(xmlkey[i].display_value,"xxx");
      }
    }
  printf("\n");

  return;
  }

/****** write_error ********************************************************
PROTO	void    write_error(char *msg1, char *msg2)
PURPOSE	Manage files in case of a catched error
INPUT	a character string,
        another character string
OUTPUT	RETURN_OK if everything went fine, RETURN_ERROR otherwise.
NOTES	-.
AUTHOR	E. Bertin (IAP)
VERSION	23/02/2007
 ***/

void	write_error(char *msg1, char *msg2)
  {
   char	error[MAXCHAR];

  sprintf(error, "%s%s", msg1,msg2);
  if (prefs.xml_flag)
    write_xmlerror(prefs.xml_name, error);

  end_xml();

  return;
  }
