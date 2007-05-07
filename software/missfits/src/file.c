/*
                                  file.c

*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*       Part of:        MissFITS
*
*       Author:         E.BERTIN (IAP) C. MARMO (IAP)
*
*       Contents:       Main loop
*
*       Last modify:    06/12/2006
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
#include <sys/types.h>
#include <unistd.h>
 
#include "define.h"
#include "globals.h"
#include "fits/fitscat.h"
#include "file.h"
#include "prefs.h"

/****** load_fitsfiles *******************************************************
PROTO	catstruct **load_fitsfiles(char *name, int *ncat, int * outcat,
                                   filenum *filetype)
PURPOSE	Try to read a file, a directory content or a series of FITS files by
	using or expanding the provided name.
INPUT	File, directory, or prefix name,
	pointer to an int, that will contain the number of catalogs read,
        pointer to an int, that will contain the number of catalogs in output,
        pointer to a fitsfile_type, that will contain the file type in output.
OUTPUT	Returns a pointer to pointers of catalogs read or NULL if no success.
	*ncat contains the number of catalogs that were found and loaded, or
	-1 if a non-FITS file/directory is found instead. *outcat contains
        the number of catalog that will be created. filetype is filled
	accordingly.
NOTES	Global preferences are used.
AUTHOR	E. Bertin (IAP) C. Marmo (IAP)
VERSION	6/12/2006
 ***/
catstruct	**load_fitsfiles(char *name, int *ncat, int *outcat,
                                 filenum *filetype)
  {
   struct stat		statbuf;
   catstruct		**incat, *cat;
   tabstruct		*tab;
   char 		str[MAXCHAR];
   char			hname[MAXCHAR],
			*pstr, *prefix;
   int			i, j, nfile, nout, nout1;

  nout = nout1 = 0;

  switch(prefs.outfile_type)
    {
    case FILE_SAME:
      *filetype = FILE_SAME;
      nout = 1;
      nfile = 1;
      break;

    case FILE_SLICE:
      *filetype = FILE_CUBE;
      nfile = 1;
      break;

    case FILE_SPLIT:
      *filetype = FILE_MULTI;
      nfile = 1;
      break;

    case FILE_CUBE:
      nout = 1;
      break;

    case FILE_MULTI:
      nout = 1;
      break;

    case FILE_DIR:
      if (!strcmp(prefs.slice_format,"NONE"))
        *filetype = FILE_MULTI;
      else if (!strcmp(prefs.split_format,"NONE"))
        *filetype = FILE_CUBE;
      break;

    default:
      error(EXIT_FAILURE, "*Internal Error*:  unknown file type", "");
    }

/* First is it a single file (most frequent case)? */
  if ((cat=read_cat(name)))
    {
    QMALLOC(incat, catstruct *, 1);
    incat[0] = cat;
/*-- Reading NAXIS */
    tab = cat->tab;
    if (cat->ntab>1)
      {
      tab = tab->nexttab;
      j=1;
      }
    else
      j = 0;
    if (*filetype == FILE_CUBE)
      {
      for (; j<cat->ntab; j++, tab=tab->nexttab)
        {
        if (tab->naxis<3)
          nout = 1;
        else
          nout = tab->naxisn[2];
        if (nout1>0 && nout != nout1)
          {
          warning(name, " extensions have different NAXIS3, skipping...");
          continue;
          }       
        nout1 = nout;
        }
      }
    if (*filetype == FILE_MULTI)
      nout = 1;
/*-- Search and read external ASCII header */
    strcpy(hname, name);
    if (!(pstr = strrchr(hname, '.')))
    pstr = hname+strlen(hname);
    sprintf(pstr, "%s", prefs.head_suffix);
    sprintf(gstr, "Looking for %s", hname);
    NFPRINTF(OUTPUT, gstr);
    tab = cat->tab;
    if (cat->ntab>1)
      {
      tab = tab->nexttab;
      j=1;
      }
    else
      j = 0;
    for (; j<cat->ntab; j++, tab=tab->nexttab)
      read_aschead(hname, j, tab);
    }
/* Or try at least to get some info on the "file" */
  else if (!stat(name, &statbuf))
    {
/*- Is it a directory? */
    if (S_ISDIR(statbuf.st_mode))
      {
/*---- Add a file prefix to the directory */
      if (!(pstr = strrchr(name, '/')))
        pstr = name;
      prefix = pstr;
      QCALLOC(incat, catstruct *, 16);
      nfile = 0;
      for (i=1; i<100; i++)
        {
        if (prefs.outfile_type == FILE_CUBE)
          sprintf(str, prefs.slice_format, i);
        if (prefs.outfile_type == FILE_MULTI)
          sprintf(str, prefs.split_format, i);
        sprintf(gstr, "%s/%s%s", name, prefix, str);
        if ((cat=read_cat(gstr)))
	  {
          if (nfile>=16 && !(nfile%16))
            QREALLOC(incat, catstruct *, nfile+16);
          incat[nfile++] = cat;

/*-------- Search and read external ASCII header */
           strcpy(hname, gstr);
          if (!(pstr = strrchr(hname, '.')))
            pstr = hname+strlen(hname);
          sprintf(pstr, "%s", prefs.head_suffix);
          sprintf(gstr, "Looking for %s", hname);
          NFPRINTF(OUTPUT, gstr);
          tab = cat->tab;
          if (cat->ntab>1)
            {
            tab = tab->nexttab;
            j=1;
            }
          else
            j = 0;
          for (; j<cat->ntab; j++, tab=tab->nexttab)
            read_aschead(hname, j, tab);
          }
	}
      *filetype = FILE_DIR;
      if (nfile && (nfile%16))
        QREALLOC(incat, catstruct *, nfile);
      nout = 1;
      }
    else
/*---- OK there is something but it is not FITS nor a directory: skip */
      {
      *ncat = -1;
      return NULL;
      }
    }
  else
/*- Nothing with that name on disk: try to expand it */
    {
    QCALLOC(incat, catstruct *, 16);
    nfile = 0;
    for (i=1; i<100; i++)
      {
      if (prefs.outfile_type==FILE_CUBE)
        {
        sprintf(str, prefs.slice_format, i);
        *filetype = FILE_SLICE;
        }
      if (prefs.outfile_type==FILE_MULTI)
        {
        sprintf(str, prefs.split_format, i);
        *filetype = FILE_SPLIT;
        }
      sprintf(gstr, "%s%s", name, str);
      if ((cat=read_cat(gstr)))
        {
        if (nfile>=16 && !(nfile%16))
          QREALLOC(incat, catstruct *, nfile+16);
        incat[nfile++] = cat;
/*------ Search and read external ASCII header */
        strcpy(hname, gstr);
        if (!(pstr = strrchr(hname, '.')))
          pstr = hname+strlen(hname);
        sprintf(pstr, "%s", prefs.head_suffix);
        sprintf(gstr, "Looking for %s", hname);
        NFPRINTF(OUTPUT, gstr);
        tab = cat->tab;
        if (cat->ntab>1)
          {
          tab = tab->nexttab;
          j=1;
          }
        else
          j = 0;
        for (; j<cat->ntab; j++, tab=tab->nexttab)
          read_aschead(hname, j, tab);
        }
      }      
    if (nfile && (nfile%16))
      QREALLOC(incat, catstruct *, nfile);
    }

  *ncat = nfile;
  *outcat = nout;
  return incat;
  }

/****** save_fitsfiles *******************************************************
PROTO	void save_fitsfiles(char *name, int t, catstruct *cat, filenum filetype)
PURPOSE	Write FITS data as a multi-extension FITS file (or a data-cube),
        a directory content or a series of FITS files by using or expanding
        the provided name.
INPUT	File, directory, or prefix name,
	Pointer to a cat structure containing the FITS data,
	fitsfile_type, containg the file type.
OUTPUT	-.
AUTHOR	E. Bertin (IAP) C. Marmo
VERSION	06/12/2006
 ***/
void	save_fitsfiles(char *name, int t, int nfile, catstruct *outcat, filenum filetype)
  {
   catstruct	*cat;
   struct stat	dirstat;
   char	        dirname[MAXCHAR], tailname[MAXCHAR], tmpname[MAXCHAR],
		filename[MAXCHAR], str[MAXCHAR], str2[MAXCHAR], str3[MAXCHAR];
   char		*pstr;
   tabstruct	*tab;
   int		i, flagmulti, flagcube, ntab;

  i = flagmulti = flagcube = 0;

/* Leave now if SAVE_TYPE is set to NONE */
  if (prefs.save_type == SAVE_NONE)
    return;

/* Get the output directory and directory-independent names */
  strcpy(dirname, name);
  if (!(pstr = strrchr(dirname, '/')))
    {
    *dirname = '.';
    pstr = dirname+1;
    strcpy(tailname,name);
    }
  else
    strcpy(tailname,pstr+1);
  *pstr = '\0';
  if ((pstr=strrchr(tailname, '.')) && !cistrcmp(pstr, ".fit",1))
    *pstr = '\0';

/* Get the output directory characteristics */
  stat(dirname, &dirstat);
/* Make a temporary output file name */
  sprintf(tmpname, "%s/mf%05ldtmp", dirname, (long)getpid()) ;

  ntab = outcat->ntab;

  switch(filetype)
    {
    case FILE_SAME:
      tab = outcat->tab;
/*---- save the file */
      add_cleanupfilename(tmpname);
      save_cat(outcat, tmpname);
      remove_cleanupfilename(tmpname);
      break;

    case FILE_MULTI:
      tab = outcat->tab;
/*---- save the MEF file */
      strcat(tmpname, FITS_SUFFIX);
      add_cleanupfilename(tmpname);
      save_cat(outcat, tmpname);
      remove_cleanupfilename(tmpname);
      break;

    case FILE_DIR:
      if (!(strcmp(prefs.slice_format,"NONE")))
        flagmulti = 1;
      if (!(strcmp(prefs.split_format,"NONE")))
        flagcube = 1;
      tab = outcat->tab;
/*-- save the FITS files */
      if (flagmulti)
        for (i=0; i<ntab; i++)
          {
          cat = new_cat(1);
          sprintf(str, prefs.split_format, i);
          sprintf(filename, "%s%s", tmpname, str);
          copy_tab_fromptr(tab, cat, 0);
          add_cleanupfilename(filename); 
          save_cat(cat, filename);
          remove_cleanupfilename(filename);
          free_cat(&cat, 1);
          tab = tab->nexttab;
          }
      if (flagcube)
        {
        sprintf(str, prefs.slice_format,t+1);
        sprintf(filename, "%s%s", tmpname, str);
        save_cat(outcat, filename);
        }
      break;

    case FILE_SPLIT:
      flagmulti = 1;
      tab = outcat->tab;
/*-- save the FITS files */
      for (i=0; i<ntab; i++)
        {
        cat = new_cat(1);
        sprintf(str, prefs.split_format, i);
        sprintf(filename, "%s%s", tmpname, str);
	copy_tab_fromptr(tab, cat, 0);
        add_cleanupfilename(filename);
        save_cat(cat, filename);
        remove_cleanupfilename(filename);
        free_cat(&cat, 1);
        tab = tab->nexttab;
        }
      break;

    case FILE_SLICE:
      flagcube = 1;
      sprintf(str, prefs.slice_format,t+1);
      sprintf(filename, "%s%s", tmpname,str);
      save_cat(outcat, filename);
      break;

    case FILE_CUBE:
      tab = outcat->tab;
/*---- save the data-cube file */
      strcat(tmpname, FITS_SUFFIX);
      add_cleanupfilename(tmpname);
      save_cat(outcat, tmpname);
      remove_cleanupfilename(tmpname);
      break;

    default:
      error(EXIT_FAILURE, "*Internal Error*:  unknown output file type", "");
    }

/* Now give the files their final filenames */
  switch(prefs.save_type)
    {
    case SAVE_REPLACE:
      if (filetype == FILE_DIR)
        {
        strcpy(dirname, tailname);
        mkdir(dirname, dirstat.st_mode & (S_IRUSR|S_IWUSR|S_IXUSR
        |S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IWGRP|S_IXOTH));
        }
/*--- Simply replace the file(s) */
/*--- Erase input file(s) */
      tab = outcat->tab;
      if ((!(flagcube) || t==(nfile-1)) && (filetype!=FILE_CUBE))
        {
        for (i=0; i<outcat->ntab; i++)
          {
          if (tab->cat)
            {
            close_cat(tab->cat);
            remove(tab->cat->filename); /* A file can be erased several times:*/
                                    /* that's alright, we ignore error msgs */
            }
          tab = tab->nexttab;
          }
        }
      else if (filetype==FILE_CUBE)
        {
        tab = tab->nexttab;
        for (i=1; i<tab->naxisn[2]+1; i++)
          {
          sprintf(str, prefs.slice_format, i);
          strtok(tab->cat->filename,"/");
          sprintf(str3, "%s/%s%s", tab->cat->filename, tailname, str);
          remove(str3);
          }
        }
/*---- Rename temporary file(s) */
      if (flagmulti)
        {
        sprintf(str, prefs.split_format, i);
        for (i=0; i<ntab; i++)
          {
          sprintf(str, prefs.split_format, i);
          sprintf(str2, "%s%s", tmpname, str);
          sprintf(str3, "%s/%s%s", dirname, tailname, str);
          rename(str2, str3);
	  }
        }
      else if (flagcube)
        {
        sprintf(str, prefs.slice_format, t+1);
        sprintf(str2, "%s%s", tmpname, str);
        sprintf(str3, "%s/%s%s", dirname, tailname, str);
        rename(str2, str3);
	}
      else
        {
        if (filetype==FILE_MULTI || filetype==FILE_CUBE)
          sprintf(str2,"%s%s",name, FITS_SUFFIX);
        rename(tmpname, str2);
        }
      break;
    case SAVE_BACKUP:
      if (filetype == FILE_DIR)
        {
        strcpy(dirname, tailname);
        mkdir(dirname, dirstat.st_mode & (S_IRUSR|S_IWUSR|S_IXUSR
        |S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IWGRP|S_IXOTH));
        }
/*---- Rename temporary file(s) */
      if (flagmulti)
        {
        for (i=0; i<ntab; i++)
          {
          sprintf(str, prefs.split_format, i);
          sprintf(str2, "%s%s", tmpname, str);
          sprintf(str3, "%s/%s%s", dirname, tailname, str);
/*-------- Check if a backup of a previous file is needed */
          if (!stat(str3, &dirstat))
	    {
            sprintf(str, "%s.back", str3);
            rename(str3, str);
            }          
          rename(str2, str3);
	  }
        }
      else if (flagcube)
        {
        sprintf(str, prefs.slice_format, t+1);
        sprintf(str2, "%s%s", tmpname, str);
        sprintf(str3, "%s/%s%s", dirname, tailname, str);
/*-------- Check if a backup of a previous file is needed */
        if (!stat(str3, &dirstat))
	  {
          sprintf(str, "%s.back", str3);
          rename(str3, str);
          }          
        rename(str2, str3);
	}
      else
        {
        if (filetype==FILE_MULTI || filetype==FILE_CUBE)
          sprintf(str2,"%s%s",name, FITS_SUFFIX);
        if (!stat(str2, &dirstat))
	  {
          sprintf(str, "%s.back", str2);
          rename(str2, str);
          sprintf(str, "%s/%s%s", dirname, tailname, FITS_SUFFIX);
          rename(tmpname, str);
          }
        rename(tmpname, str2);
        }
      break;
    case SAVE_NEW:
/*-- Simply give output file(s) new name(s) */
      if (filetype == FILE_DIR)
        {
        strcpy(dirname, tailname);
        mkdir(dirname, dirstat.st_mode & (S_IRUSR|S_IWUSR|S_IXUSR
        |S_IRGRP|S_IWGRP|S_IXGRP|S_IROTH|S_IWGRP|S_IXOTH));
        }
      if (flagmulti)
        {
        for (i=0; i<ntab; i++)
          {
          sprintf(str, prefs.split_format, i);
          sprintf(str2, "%s%s", tmpname, str);
          if ((pstr=strrchr(str, '.')) && !cistrcmp(pstr, ".fit",1))
            *pstr = '\0';
          sprintf(str3, "%s/%s%s%s%s" , dirname, tailname,
		str, prefs.new_suffix, FITS_SUFFIX);
          rename(str2, str3);
	  }
        }
      else
        {
        if (flagcube)
          {
          sprintf(str, prefs.slice_format, t+1);
          sprintf(str2, "%s%s", tmpname, str);
          if ((pstr=strrchr(str, '.')) && !cistrcmp(pstr, ".fit",1))
            *pstr = '\0';
          sprintf(str3, "%s/%s%s%s%s" , dirname, tailname,
		str, prefs.new_suffix, FITS_SUFFIX);
          rename(str2, str3);
	  }
        else
          {
          sprintf(str, "%s/%s%s", dirname, tailname, prefs.new_suffix);
          strcat(str, FITS_SUFFIX);
          rename(tmpname, str);
          }
        }
      break;
    default:
      error(EXIT_FAILURE, "*Internal Error*:  unknown file save type", "");
    }


  return;
  }


/****** read_aschead ********************************************************
PROTO	int	read_aschead(char *filename, int frameno, tabstruct *tab)
PURPOSE	Read a ASCII header file and update the current field's tab
INPUT	Name of the ASCII file,
        Frame number (if extensions),
        Tab structure.
OUTPUT	RETURN_OK if the file was found, RETURN_ERROR otherwise.
NOTES	-.
AUTHOR	E. Bertin (IAP)
VERSION	18/11/2004
 ***/
int	read_aschead(char *filename, int frameno, tabstruct *tab)
  {
   char	        keyword[MAXCHAR],data[MAXCHAR],comment[MAXCHAR];
   FILE		*file;
   h_type	htype;
   t_type	ttype;
   char		*gstrt;
   int		i, pos;

  if ((file=fopen(filename, "r")))
    {
/*- Skip previous ENDs in multi-FITS extension headers */
    for (i=(frameno?(frameno-1):0); i--;)
      while (fgets(gstr, MAXCHAR, file)
                && strncmp(gstr,"END ",4)
                && strncmp(gstr,"END\n",4));
    pos = fitsfind(tab->headbuf, "END     ");
    memset(gstr, ' ', 80);
    while (fgets(gstr, 81, file) && strncmp(gstr,"END ",4)
                                && strncmp(gstr,"END\n",4))
      {
/*---- Remove possible junk within the 80 first chars (linefeeds, etc.) */
      gstrt = gstr;
      for (i=80; i--; gstrt++)
        if (*gstrt < ' ' || *gstrt >'~')
          *gstrt = ' ';
/*---- Skip non-FITS parts */
      if (gstr[0]<=' ' || (gstr[8]!=' ' && gstr[8]!='='))
        continue;
      fitspick(gstr, keyword, data, &htype, &ttype, comment);
/*---- Block critical keywords */
      if (!wstrncmp(keyword, "SIMPLE  ", 8)
        ||!wstrncmp(keyword, "BITPIX  ", 8)
        ||!wstrncmp(keyword, "NAXIS   ", 8)
        ||!wstrncmp(keyword, "BSCALE  ", 8)
        ||!wstrncmp(keyword, "BZERO   ", 8))
        continue;
/*---- Always keep a one-line margin */
      if ((pos+1)*80>=tab->headnblock*FBSIZE)
        {
        tab->headnblock++;
        QREALLOC(tab->headbuf, char, tab->headnblock*FBSIZE);
        memset(tab->headbuf+(tab->headnblock-1)*FBSIZE, ' ', FBSIZE);
        }
      fitsadd(tab->headbuf, keyword, comment);
      if (fitsfind(tab->headbuf, "END     ")>=pos)
        pos++;
      fitswrite(tab->headbuf, keyword, data, htype, ttype);
      memset(gstr, ' ', 80);
      }
    fclose(file);
/*-- Update the tab data */
    readbasic_head(tab);
    return RETURN_OK;
    }
  else
    return RETURN_ERROR;
  }
