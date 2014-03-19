/*
*				prefs.c
*
* Functions related to run-time configuration.
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

#include	<ctype.h>
#include	<math.h>
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#if defined(USE_THREADS) \
&& (defined(__APPLE__) || defined(FREEBSD) || defined(NETBSD))	/* BSD, Apple */
 #include	<sys/types.h>
 #include	<sys/sysctl.h>
#elif defined(USE_THREADS) && defined(HAVE_MPCTL)		/* HP/UX */
 #include	<sys/mpctl.h>
#endif

#include	"define.h"
#include	"globals.h"
#include	"fits/fitscat.h"
#include 	"prefs.h"
#include	"preflist.h"

/********************************* dumpprefs ********************************/
/*
Print the default preference parameters.
*/
void	dumpprefs(int state)
  {
   char **dp;

  dp = default_prefs;
  while (**dp)
    if (**dp != '*')
      printf("%s\n",*(dp++));
    else if (state)
      printf("%s\n",*(dp++)+1);
    else
      dp++;
  return;
  }

/********************************* readprefs ********************************/
/*
Read a configuration file in ``standard'' format (see the SExtractor
documentation)
*/
void    readprefs(char *filename, char **argkey, char **argval, int narg)

  {
   FILE		*infile;
   char		*cp, str[MAXCHAR], *keyword, *value, **dp;
   int		i, ival, nkey, warn, argi, flagc, flagd, flage, flagz;
   double	dval;
#ifndef	NO_ENVVAR
   char		value2[MAXCHAR],envname[MAXCHAR];
   char		*dolpos;
#endif


  if ((infile = fopen(filename,"r")) == NULL)
    {
    flage = 1;
    warning(filename, " not found, using internal defaults");
    }
  else
    flage = 0;

/*Build the keyword-list from pkeystruct-array */

  for (i=0; key[i].name[0]; i++)
    strcpy(keylist[i], key[i].name);
  keylist[i][0] = '\0';

/*Scan the configuration file*/

  argi=0;
  flagc = 0;
  flagd = 1;
  dp = default_prefs;
  for (warn=0;;)
    {
    if (flagd)
      {
      if (**dp)
        {
        if (**dp=='*')
          strcpy(str, *(dp++)+1);
        else
          strcpy(str, *(dp++));
        }
      else
        flagd = 0;
      }
    if (!flagc && !flagd)
      if (flage || !fgets(str, MAXCHAR, infile))
        flagc=1;

    if (flagc)
      {
      if (argi<narg)
        {
        sprintf(str, "%s %s", argkey[argi], argval[argi]);
        argi++;
        }
      else
        break;
      }
    keyword = strtok(str, notokstr);
    if (keyword && keyword[0]!=0 && keyword[0]!=(char)'#')
      {
     if (warn>=10)
        error(EXIT_FAILURE, "*Error*: No valid keyword found in ", filename);
      nkey = findkeys(keyword, keylist, FIND_STRICT);
      if (nkey!=RETURN_ERROR)
        {
        value = strtok((char *)NULL, notokstr);
#ifndef	NO_ENVVAR
/*------ Expansion of environment variables (preceded by '$') */
        if (value && (dolpos=strchr(value, '$')))
          {
           int	nc;
           char	*valuet,*value2t, *envval;

          value2t = value2;
          valuet = value;
          while (dolpos)
            {
            while (valuet<dolpos)
              *(value2t++) = *(valuet++);	/* verbatim copy before '$' */
            if (*(++valuet) == (char)'{')
              valuet++;
            strncpy(envname, valuet, nc=strcspn(valuet,"}/:\"\'\\"));
            *(envname+nc) = (char)'\0';
            if (*(valuet+=nc) == (char)'}')
              valuet++;
            if (!(envval=getenv(envname)))
              error(EXIT_FAILURE, "Environment variable not found: ",
				envname);
            while(*envval)			/* Copy the ENV content */
              *(value2t++) = *(envval++);
            while(*valuet && *valuet!=(char)'$')/* Continue verbatim copy */
              *(value2t++) = *(valuet++);
            if (*valuet)
              dolpos = valuet;
            else
              {
              dolpos = NULL;
              *value2t = (char)'\0';
              }
	    }

          value = value2;
          }
#endif
        switch(key[nkey].type)
          {
          case P_FLOAT:
            if (!value || value[0]==(char)'#')
              error(EXIT_FAILURE, keyword," keyword has no value!");
            dval = atof(value);
            if (dval>=key[nkey].dmin && dval<=key[nkey].dmax)
              *(double *)(key[nkey].ptr) = dval;
            else
              error(EXIT_FAILURE, keyword," keyword out of range");
            break;

          case P_INT:
            if (!value || value[0]==(char)'#')
              error(EXIT_FAILURE, keyword," keyword has no value!");
            ival = atoi(value);
            if (ival>=key[nkey].imin && ival<=key[nkey].imax)
              *(int *)(key[nkey].ptr) = ival;
            else
              error(EXIT_FAILURE, keyword, " keyword out of range");
            break;

          case P_STRING:
            if (!value || value[0]==(char)'#')
              error(EXIT_FAILURE, keyword," string is empty!");
            strcpy((char *)key[nkey].ptr, value);
            break;

          case P_BOOL:
            if (!value || value[0]==(char)'#')
              error(EXIT_FAILURE, keyword," keyword has no value!");
            if ((cp = strchr("yYnN", (int)value[0])))
              *(int *)(key[nkey].ptr) = (tolower((int)*cp)=='y')?1:0;
            else
              error(EXIT_FAILURE, keyword, " value must be Y or N");
            break;

          case P_KEY:
            if (!value || value[0]==(char)'#')
              error(EXIT_FAILURE, keyword," keyword has no value!");
            if ((ival = findkeys(value, key[nkey].keylist,FIND_STRICT))
			!= RETURN_ERROR)
              *(int *)(key[nkey].ptr) = ival;
            else
              error(EXIT_FAILURE, keyword, " set to an unknown keyword");
            break;

          case P_BOOLLIST:
            for (i=0; i<MAXLIST&&value&&value[0]!=(char)'#'; i++)
              {
              if (i>=key[nkey].nlistmax)
                error(EXIT_FAILURE, keyword, " has too many members");
              if ((cp = strchr("yYnN", (int)value[0])))
                ((int *)(key[nkey].ptr))[i] = (tolower((int)*cp)=='y')?1:0;
              else
                error(EXIT_FAILURE, keyword, " value must be Y or N");
              value = strtok((char *)NULL, notokstr);
              }
            if (i<key[nkey].nlistmin)
              error(EXIT_FAILURE, keyword, " list has not enough members");
            *(key[nkey].nlistptr) = i;
            break;

          case P_INTLIST:
            for (i=0; i<MAXLIST&&value&&value[0]!=(char)'#'; i++)
              {
              if (i>=key[nkey].nlistmax)
                error(EXIT_FAILURE, keyword, " has too many members");
              ival = strtol(value, NULL, 0);
              if (ival>=key[nkey].imin && ival<=key[nkey].imax)
                ((int *)key[nkey].ptr)[i] = ival;
              else
                error(EXIT_FAILURE, keyword, " keyword out of range");
              value = strtok((char *)NULL, notokstr);
              }
            if (i<key[nkey].nlistmin)
              error(EXIT_FAILURE, keyword, " list has not enough members");
            *(key[nkey].nlistptr) = i;
            break;

          case P_FLOATLIST:
            for (i=0; i<MAXLIST&&value&&value[0]!=(char)'#'; i++)
              {
              if (i>=key[nkey].nlistmax)
                error(EXIT_FAILURE, keyword, " has too many members");
              dval = atof(value);
              if (dval>=key[nkey].dmin && dval<=key[nkey].dmax)
                ((double *)key[nkey].ptr)[i] = dval;
              else
                error(EXIT_FAILURE, keyword, " keyword out of range");
              value = strtok((char *)NULL, notokstr);
              }
            if (i<key[nkey].nlistmin)
              error(EXIT_FAILURE, keyword, " list has not enough members");
            *(key[nkey].nlistptr) = i;
            break;

          case P_KEYLIST:
            for (i=0; i<MAXLIST && value && value[0]!=(char)'#'; i++)
              {
              if (i>=key[nkey].nlistmax)
                error(EXIT_FAILURE, keyword, " has too many members");
	      if ((ival = findkeys(value, key[nkey].keylist, FIND_STRICT))
			!= RETURN_ERROR)
                ((int *)(key[nkey].ptr))[i] = ival;
              else
                error(EXIT_FAILURE, keyword, " set to an unknown keyword");
              value = strtok((char *)NULL, notokstr);
              }
            if (i<key[nkey].nlistmin)
              error(EXIT_FAILURE, keyword, " list has not enough members");
            *(key[nkey].nlistptr) = i;
            break;

          case P_STRINGLIST:
            if (!value || value[0]==(char)'#')
              {
              value = "";
              flagz = 1;
              }
            else
              flagz = 0;
            for (i=0; i<MAXLIST && value && value[0]!=(char)'#'; i++)
              {
              if (i>=key[nkey].nlistmax)
                error(EXIT_FAILURE, keyword, " has too many members");
              free(((char **)key[nkey].ptr)[i]);
              QMALLOC(((char **)key[nkey].ptr)[i], char, MAXCHAR);
              strcpy(((char **)key[nkey].ptr)[i], value);
              value = strtok((char *)NULL, notokstr);
              }
            if (i<key[nkey].nlistmin)
              error(EXIT_FAILURE, keyword, " list has not enough members");
            *(key[nkey].nlistptr) = flagz?0:i;
            break;

          default:
            error(EXIT_FAILURE, "*Internal ERROR*: Type Unknown",
				" in readprefs()");
            break;
          }
        key[nkey].flag = 1;
        }
      else
        {
        warning(keyword, " keyword unknown");
        warn++;
        }
      }
    }

  for (i=0; key[i].name[0]; i++)
    if (!key[i].flag)
      error(EXIT_FAILURE, key[i].name, " configuration keyword missing");
  if (!flage)
    fclose(infile);

  return;
  }


/********************************* findkey **********************************/
/*
find an item within a list of keywords.
*/
int	findkeys(char *str, char keyw[][32], int mode)

  {
  int i;

  for (i=0; keyw[i][0]; i++)
    if (!cistrcmp(str, keyw[i], mode))
      return i;

  return RETURN_ERROR;
  }


/******************************* cistrcmp ***********************************/
/*
case-insensitive strcmp.
*/
int	cistrcmp(char *cs, char *ct, int mode)

  {
   int  i, diff;

  if (mode)
    {
    for (i=0; cs[i]&&ct[i]; i++)
      if ((diff=tolower((int)cs[i])-tolower((int)ct[i])))
        return diff;
    }
  else
    {
    for (i=0; cs[i]||ct[i]; i++)
      if ((diff=tolower((int)cs[i])-tolower((int)ct[i])))
        return diff;
    }

  return 0;
  }

/********************************* fitsroot *********************************/
/* Get the output directory and directory-independent names */

char    *fitsroot(char *str)

  {
  char dirname[MAXCHAR], str2[MAXCHAR], *pstr;

  strcpy(dirname, str);
  if (!(pstr = strrchr(dirname, '/')))
    {
    *dirname = '.';
    pstr = dirname+1;
    strcpy(str2,str);
    }
  else
    strcpy(str2,pstr+1);
  *pstr = '\0';
  if ((pstr=strrchr(str2, '.')) && !cistrcmp(pstr, ".fit",1))
    *pstr = '\0';

  strcpy(str,str2);
  return(str);
  }

/********************************* useprefs *********************************/
/*
Update various structures according to the prefs.
*/
void	useprefs(void)

  {
   unsigned short	ashort=1;
   char			*pstr, *tstr;
   int			i, p;
#ifdef USE_THREADS
   int			nproc;
#endif

/* Test if byteswapping will be needed */
  bswapflag = *((char *)&ashort);

/* Multithreading */
#ifdef USE_THREADS
  if (!prefs.nthreads)
    {
/*-- Get the number of processors for parallel builds */
/*-- See, e.g. http://ndevilla.free.fr/threads */
    nproc = -1;
#if defined(_SC_NPROCESSORS_ONLN)		/* AIX, Solaris, Linux */
    nproc = (int)sysconf(_SC_NPROCESSORS_ONLN);
#elif defined(_SC_NPROCESSORS_CONF)
    nproc = (int)sysconf(_SC_NPROCESSORS_CONF);
#elif defined(__APPLE__) || defined(FREEBSD) || defined(NETBSD)	/* BSD, Apple */
    {
     int	mib[2];
     size_t	len;

     mib[0] = CTL_HW;
     mib[1] = HW_NCPU;
     len = sizeof(nproc);
     sysctl(mib, 2, &nproc, &len, NULL, 0);
     }
#elif defined (_SC_NPROC_ONLN)			/* SGI IRIX */
    nproc = sysconf(_SC_NPROC_ONLN);
#elif defined(HAVE_MPCTL)			/* HP/UX */
    nproc =  mpctl(MPC_GETNUMSPUS_SYS, 0, 0);
#endif

    if (nproc>0)
      prefs.nthreads = nproc;
    else
      {
      prefs.nthreads = 2;
      warning("Cannot find the number of CPUs on this system:",
                "NTHREADS defaulted to 2");
      }
    }
#else
  if (prefs.nthreads != 1)
    {
    prefs.nthreads = 1;
    warning("NTHREADS != 1 ignored: ",
        "this build of " BANNER " is single-threaded");
    }
#endif

/* Handle FITS keyword translation */
  for (i=0; i<prefs.nreplace_key; i++)
    {
    if (!(strpbrk(prefs.replace_key[i],":")))
      error(EXIT_FAILURE,"Wrong syntax in REPLACE_KEYWORD: ", prefs.replace_key[i]);

    memset(prefs.old_key[i], ' ', 8);
    memset(prefs.new_key[i], ' ', 8);
    strncpy(prefs.old_key[i], strtok(prefs.replace_key[i], ":"), 8);
    tstr =  strtok(NULL, ":");
    strncpy(prefs.new_key[i], tstr, strlen(tstr));
    }

/* Handle FITS keyword slicing */
  for (i=0; i<prefs.nslice_key; i++)
    {
    memset(prefs.oldslice_key[i], ' ', 8);
    memset(prefs.newslice_key[i], ' ', 8);
    QCALLOC(tstr, char, 9);
    strncpy(tstr,prefs.slice_key[i],8);
    for (pstr = tstr, p=0; p<8; p++, pstr++)
      if (!*pstr)
        *pstr = ' ';
    sprintf(prefs.newslice_key[i],"%8s",tstr);
    free(tstr);
    }


/* Remove possible trailing "/" in filenames */
  for (i=0; i<prefs.nfile; i++)
    if (*(pstr=prefs.file_name[i]+strlen(prefs.file_name[i])-1) == '/')
      *pstr = '\0';

  return;
  }

/********************************* endprefs *********************************/
/*
Mostly free memory allocate for static arrays.
*/
void	endprefs(void)

  {
    int	i;

  for (i=0; i<prefs.ndisplay_key; i++)
    free(prefs.display_key[i]);
  for (i=0; i<prefs.nremove_key; i++)
    free(prefs.remove_key[i]);
  for (i=0; i<prefs.nreplace_key; i++)
    free(prefs.replace_key[i]);
  for (i=0; i<prefs.nslice_key; i++)
    free(prefs.slice_key[i]);

  return;
  }
