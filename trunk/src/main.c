/*
*				main.c
*
* Command line parsing.
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
#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>
#include	<time.h>
#include	<sys/time.h>

#include	"define.h"
#include	"globals.h"
#include	"fits/fitscat.h"
#include	"prefs.h"
#include        "xml.h"

#define		SYNTAX \
"missfits image1 [image2 ...][@image_list1 [@image_list2 ...]]\n" \
"\t\t[-c <configuration_file>][-<keyword> <value>]\n" \
"> to dump a default configuration file: missfits -d \n" \
"> to dump a default extended configuration file: missfits -dd \n"

static double	counter_seconds(void);

extern const char	notokstr[];

time_t		thetime, thetime2;

/********************************** main ************************************/
int	main(int argc, char *argv[])

  {
   FILE         *fp;
   char 	liststr[MAXCHAR];
   char		**argkey, **argval, *str, *listname, *listbuf;
   double	dtime;
   int		a, bufpos, bufsize, l, narg, nim, opt, opt2;
   struct tm	*tm;

  if (argc<2)
    {
    fprintf(OUTPUT, "\n         %s  version %s (%s)\n", BANNER,MYVERSION,DATE);
    fprintf(OUTPUT, "\nWritten by %s\n", AUTHORS);
    fprintf(OUTPUT, "Copyright %s\n", COPYRIGHT);
    fprintf(OUTPUT, "\nvisit %s\n", WEBSITE);
    fprintf(OUTPUT, "\n%s\n", DISCLAIMER);
    error(EXIT_SUCCESS, "SYNTAX: ", SYNTAX);
    }
  QMALLOC(argkey, char *, argc);
  QMALLOC(argval, char *, argc);

/* Install error logging */
  error_installfunc(write_error);

/* Processing start date and time */
  thetime = time(NULL);
  dtime = counter_seconds();
  tm = localtime(&thetime);
  sprintf(prefs.sdate_start,"%04d-%02d-%02d",
        tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday);
  sprintf(prefs.stime_start,"%02d:%02d:%02d",
        tm->tm_hour, tm->tm_min, tm->tm_sec);

/*default parameters */
  prefs.command_line = argv;
  prefs.ncommand_line = argc;
  strcpy(prefs.prefs_name, "default.missfits");
  prefs.nfile = 1;
  prefs.file_name[0] = "image";
  narg = nim = 0;
  listbuf = (char *)NULL;
  bufpos = 0;
  bufsize = MAXCHAR*1000;

  for (a=1; a<argc; a++)
    {
    if (*(argv[a]) == '-')
      {
      opt = (int)argv[a][1];
      if (strlen(argv[a])<4 || opt == '-')
        {
        opt2 = (int)tolower((int)argv[a][2]);
        if (opt == '-')
	  {
	  opt = opt2;
          opt2 = (int)tolower((int)argv[a][3]);
	  }
        switch(opt)
          {
          case 'c':
            if (a<(argc-1))
              strcpy(prefs.prefs_name, argv[++a]);
            break;
          case 'd':
            dumpprefs(opt2=='d' ? 1 : 0);
            exit(EXIT_SUCCESS);
            break;
          case 'v':
            printf("%s version %s (%s)\n", BANNER,MYVERSION,DATE);
            exit(0);
            break;
          case 'h':
          default:
            error(EXIT_SUCCESS,"SYNTAX: ", SYNTAX);
          }
        }
      else
        {
        argkey[narg] = &argv[a][1];
        argval[narg++] = argv[++a];
        }       
      }
    else if (*(argv[a]) == '@')
      {
/*---- The input image list filename */
      listname = argv[a]+1;
      if ((fp=fopen(listname,"r")))
        {
        if (!listbuf)
          {
          QMALLOC(listbuf, char, bufsize);
          }
        while (fgets(liststr,MAXCHAR,fp))
          if (nim<MAXFILE)
            {
            str = strtok(liststr, "\n\r\t ");
            if (!str)
              continue;
            l = strlen(str)+1;
            if (bufpos+l > bufsize)
              {
              bufsize += MAXCHAR*1000;
              QREALLOC(listbuf, char, bufsize);
              }
            prefs.file_name[nim] = strcpy(listbuf + bufpos, str);
            bufpos += l;
            nim++;
            }
          else
            error(EXIT_FAILURE, "*Error*: Too many input images in ",
			liststr);
        fclose(fp);
        }
      else
        error(EXIT_FAILURE, "*Error*: Cannot open image list ", listname);
      }
    else
      {
/*---- The input image filename(s) */
      for(; (a<argc) && (*argv[a]!='-'); a++)
        for (str=NULL;(str=strtok(str?NULL:argv[a], notokstr)); nim++)
          if (nim<MAXFILE)
            prefs.file_name[nim] = str;
         else
            error(EXIT_FAILURE, "*Error*: Too many input images: ", str);
      a--;
      }
    }

  prefs.nfile = nim;
  readprefs(prefs.prefs_name, argkey, argval, narg);
  useprefs();

  free(argkey);
  free(argval);

  makeit();

/* Processing end date and time */
  thetime2 = time(NULL);
  tm = localtime(&thetime2);
  sprintf(prefs.sdate_end,"%04d-%02d-%02d",
        tm->tm_year+1900, tm->tm_mon+1, tm->tm_mday);
  sprintf(prefs.stime_end,"%02d:%02d:%02d",
        tm->tm_hour, tm->tm_min, tm->tm_sec);
  prefs.time_diff = counter_seconds() - dtime;

/* Write XML */

  if (prefs.xml_flag)
    {
    NFPRINTF(OUTPUT, "Writing XML file...");
    write_xml(prefs.xml_name);
    end_xml();
    }

  endprefs();

  NFPRINTF(OUTPUT, "");
  NPRINTF(OUTPUT, "> All done (in %.1f s)\n", prefs.time_diff);

  exit(EXIT_SUCCESS);
  }


/****** counter_seconds *******************************************************
PROTO	double counter_seconds(void)
PURPOSE	Count the number of seconds (with an arbitrary offset).
INPUT	-.
OUTPUT	Returns a number of seconds.
NOTES	Results are meaningful only for tasks that take one microsec or more.
AUTHOR	E. Bertin (IAP)
VERSION	24/09/2009
 ***/
static double	counter_seconds(void)
  {
   struct timeval	tp;
   struct timezone	tzp;
   int			dummy;

  dummy = gettimeofday(&tp,&tzp);
  return (double) tp.tv_sec + (double) tp.tv_usec * 1.0e-6;
  }


