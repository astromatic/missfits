/*
                                  makeit.c

*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*       Part of:        MissFITS
*
*       Author:         E.BERTIN (IAP) C. MARMO (IAP)
*
*       Contents:       Main loop
*
*       Last modify:    07/05/2007
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

#ifdef	HAVE_CONFIG_H
#include "config.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <time.h>
 
#include "define.h"
#include "globals.h"
#include "custom.h"
#include "fits/fitscat.h"
#include "file.h"
#include "prefs.h"

void	print_tabinfo(tabstruct *tab, int no);

time_t		thetime, thetime2;

/********************************** makeit ***********************************/
void	makeit(void)
  {
   catstruct		**incat,
			*outcat;
   tabstruct		*tab, *intab;
   char	                im[MAXCHAR];
   filenum		filetype;
   int			a,c,k,n,p, t, s, check, narg, nfile, nout,
                        ntabin, ntabout, flagmulti, flagcube;
   char                 *pix;
   char			keyword[10], str[100], str2[100];
   KINGSIZE_T           size;
   h_type               htype;
   t_type               ttype;
   struct tm		*tm;

  check = flagmulti = flagcube = 0;

  install_cleanup(NULL);

/* Processing start date and time */
  thetime = time(NULL);
  tm = localtime(&thetime);
  sprintf(prefs.sdate_start,"%04d-%02d-%02d",
        tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday);
  sprintf(prefs.stime_start,"%02d:%02d:%02d",
        tm->tm_hour, tm->tm_min, tm->tm_sec);

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
/* Go argument by argument */
  NFPRINTF(OUTPUT, "Examining input data...")

  for (a=0; a<narg; a++)
    {
    incat = load_fitsfiles(prefs.file_name[a], &nfile, &nout, &filetype);
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
          break;
        case FILE_CUBE:
          outcat = new_cat(1);
          copy_tabs_blind(incat[0], outcat);
          break;
        case FILE_SLICE:
          flagcube = 1;
          outcat = new_cat(1);
          copy_tabs_blind(incat[0], outcat);
          break;            
        case FILE_DIR:
          if (!strcmp(prefs.slice_format,"NONE"))
            flagmulti = 1;
          else if (!strcmp(prefs.split_format,"NONE"))
            flagcube = 1;
          outcat = new_cat(1);
          copy_tabs_blind(incat[0], outcat);
          break;            
        default:
          error(EXIT_FAILURE, "*Internal Error*:  unknown output file type", "");
        }

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
/*---- Slicing cubes */
        if (flagcube)
          {
          if (tab->naxis>2)
            {
            tab->naxis = 2;
            fitsremove(tab->headbuf, "NAXIS3");
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
            fitswrite(tab->headbuf, "NAXIS3  ",&nfile, H_INT, T_LONG);
            tab->bodybuf = malloc(size*nfile);
            pix = tab->bodybuf ;
            for (c=0; c<nfile; c++, pix += size)
              {
              intab = incat[c]->tab->nexttab;
              for (p=t ; --p ; intab = intab->nexttab);
              QFSEEK(incat[c]->file, intab->bodypos, SEEK_SET,
                       incat[c]->filename);
              QFREAD(pix, size, incat[c]->file, incat[c]->filename);
              swapbytes(pix, tab->bytepix, size/tab->bytepix);
              }
            }
          }
/*---- Check Checksum */
        if (prefs.checksum_type==CHECKSUM_VERIFY
	  || prefs.checksum_type==CHECKSUM_UPDATE)
          check = verify_checksum(tab);
/*---- Replace keywords */
        for (k=0; k<prefs.nreplace_key; k++)
          if ((n=fitsfind(tab->headbuf, prefs.old_key[k])))
            strncpy(tab->headbuf+n*80, prefs.new_key[k],8);
/*---- Slice keywords */
        for (k=0; k<prefs.nslice_key; k++)
          {
          if (flagcube)
            {
            sprintf(im,prefs.slicekey_format,s+1);
            sprintf(prefs.oldslice_key[k], "%s%s", prefs.slice_key[k],im);
            if ((n=fitsfind(tab->headbuf, prefs.oldslice_key[k])))
              strncpy(tab->headbuf+n*80, prefs.newslice_key[k],8);
            }
          else if (prefs.outfile_type == FILE_CUBE)
            {
            for (c=0; c<nfile; c++)
              {
              intab = incat[c]->tab->nexttab;
              for (p=t ; --p ; intab = intab->nexttab);
              sprintf(im,prefs.slicekey_format,c+1);
              sprintf(prefs.oldslice_key[k], "%s%s", prefs.slice_key[k],im);
              if ((n=fitsfind(intab->headbuf, prefs.newslice_key[k])))
                {
                fitspick(intab->headbuf+n*80, keyword, str2, &htype,
                         &ttype, str);
                fitsadd(tab->headbuf, prefs.oldslice_key[k], str);
                fitswrite(tab->headbuf, prefs.oldslice_key[k],
                          str2, htype, ttype);          
                }
              }
            }
          }
/*---- Remove keywords */
        for (k=0; k<prefs.nremove_key; k++)
          fitsremove(tab->headbuf, prefs.remove_key[k]);
/*---- Fix WFI Astrometric keywords (custom option) */
        if (prefs.fixwfi_flag)
          fix_wfi(tab);
/*---- Print file info */
        print_tabinfo(tab, t);
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
      free_cat(&outcat,1);
      }
    free_cat(incat, nfile);
    }
  cleanup_files();

/* Processing end date and time */
  thetime2 = time(NULL);
  tm = localtime(&thetime2);
  sprintf(prefs.sdate_end,"%04d-%02d-%02d",
        tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday);
  sprintf(prefs.stime_end,"%02d:%02d:%02d",
        tm->tm_hour, tm->tm_min, tm->tm_sec);
  prefs.time_diff = difftime(thetime2, thetime);

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
VERSION	11/06/2001
 ***/
void	print_tabinfo(tabstruct *tab, int no)

  {
   h_type		htype;
   t_type		ttype;
   char			keyword[10], str[100],
			*filename,*rfilename;
   int			i,n;

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
    if ((n=fitsfind(tab->headbuf, prefs.display_key[i]))>0)
      {
      fitspick(tab->headbuf+n*80, keyword, gstr, &htype, &ttype, str);
/*---- Display formatting is a modified version of fitswrite() */
      switch(htype)
        {
        case H_INT:
          printf(" %12d", (ttype==T_SHORT)?
					*(short *)gstr: *(int *)gstr);
          break;
        case H_FLOAT:
        case H_EXPO:
          printf(" %12g", (ttype==T_DOUBLE)?
					*(double *)gstr: *(float *)gstr);
          break;
        case H_BOOL:
          printf(" %c", *(int *)gstr? '0':'1');
          break;
        case H_STRING:
        case H_STRINGS:
          printf(" \"%.70s\"", gstr);
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
      printf(" xxx");
    }
  printf("\n");

  return;
  }
