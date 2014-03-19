/*
*				preflist.h
*
* Configuration keyword definitions.
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	This file part of:	MissFITS
*
*	Copyright:		(C) 2006-2013 Emmanuel Bertin -- IAP/CNRS/UPMC
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
*	Last modified:		09/12/2013
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%*/

#include "key.h"

#ifndef _PREFS_H_
#include "prefs.h"
#endif
#ifdef  USE_THREADS
#define	THREADS_PREFMAX	THREADS_NMAX
#else
#define	THREADS_PREFMAX	65535
#endif

int idummy;

pkeystruct key[] =
 {
  {"CHECKSUM_TYPE", P_KEY, &prefs.checksum_type, 0,0, 0.0,0.0,
    {"NONE", "COMPUTE", "VERIFY", "UPDATE", ""}},
  {"DISPLAY_KEYWORD",  P_STRINGLIST, prefs.display_key, 0,0, 0.0,0.0,
   {""}, 0, MAXKEYWORD, &prefs.ndisplay_key},
  {"FIX_WFI", P_BOOL, &prefs.fixwfi_flag},
  {"HEADER_SUFFIX", P_STRING, prefs.head_suffix},
  {"NEW_SUFFIX", P_STRING, prefs.new_suffix},
  {"NEXTENSIONS_MIN", P_INT, &prefs.next_min, 0,1000000000},
  {"NTHREADS", P_INT, &prefs.nthreads, 0, THREADS_PREFMAX},
  {"OUTFILE_TYPE", P_KEY, &prefs.outfile_type, 0,0, 0.0,0.0,
    {"SAME", "MULTI", "SPLIT", "SLICE", "CUBE", "DIR", ""}},
  {"PROCESS_TYPE", P_KEY, &prefs.process_type, 0,0, 0.0,0.0,
    {"NONE", "TOBITPIX16", "COMPRESS", "UNCOMPRESS", ""}},
  {"REMOVE_KEYWORD",  P_STRINGLIST, prefs.remove_key, 0,0, 0.0,0.0,
   {""}, 0, MAXKEYWORD, &prefs.nremove_key},
  {"REPLACE_KEYWORD",  P_STRINGLIST, prefs.replace_key, 0,0, 0.0,0.0,
   {""}, 0, MAXKEYWORD, &prefs.nreplace_key},
  {"SAVE_TYPE", P_KEY, &prefs.save_type, 0,0, 0.0,0.0,
   {"NONE","NEW","BACKUP","REPLACE",""}},
  {"SLICEKEY_FORMAT", P_STRING, prefs.slicekey_format},
  {"SLICE_KEYWORD", P_STRINGLIST, prefs.slice_key, 0,0, 0.0,0.0,
   {""}, 0, MAXKEYWORD, &prefs.nslice_key},
  {"SLICE_START", P_INT, &prefs.slice_start, 0, 1000000},
  {"SLICE_SUFFIX", P_STRING, prefs.slice_format},
  {"SPLIT_START", P_INT, &prefs.split_start, 0, 1000000},
  {"SPLIT_SUFFIX", P_STRING, prefs.split_format},
  {"VERBOSE_TYPE", P_KEY, &prefs.verbose_type, 0,0, 0.0,0.0,
   {"QUIET","NORMAL","FULL",""}},
  {"WRITE_XML", P_BOOL, &prefs.xml_flag},
  {"XML_NAME", P_STRING, prefs.xml_name},
  {"XSL_URL", P_STRING, prefs.xsl_name},
  {""}
 };

char			keylist[sizeof(key)/sizeof(pkeystruct)][32];
const char		notokstr[] = {" \t=,;\n\r\""};

char *default_prefs[] =
{
"# Default configuration file for " BANNER " " MYVERSION,
"# EB CM " DATE,
"#",
"#------------------------------- FITS keywords --------------------------------",
"REMOVE_KEYWORD                         # Remove a FITS keyword from the headers",
"REPLACE_KEYWORD                        # Replace a FITS keyword with another",
"                                       # Syntax: OLD_KEY1:NEW_KEY1,",
"                                       #         OLD_KEY2:NEW_KEY2,...",
"SLICE_KEYWORD                          # Replace the keyword ",
"                                       # SLICE_KEYWORD+SLICEKEY_FORMAT",
"                                       # with SLICE_KEYWORD for every slice",
"                                       # or viceversa building cubes",
"SLICEKEY_FORMAT        %03d            # format of slice referring keywords",
"DISPLAY_KEYWORD        OBJECT          # Display the following keywords while",
"                                       # processing the files",
" ",
"HEADER_SUFFIX          .head           # Filename extension for add. headers",
" ",
"*FIX_WFI                N               # Fix WFI camera image-headers",
"*",
"#------------------------------ FITS properties -------------------------------",
" ",
"NEXTENSIONS_MIN        0               # Minimum number of extensions (warns",
"                                       # if less are found)",
"OUTFILE_TYPE           SAME            # Basic or Multi-FITS output:",
"                                       # \"SAME\", \"MULTI\", \"SPLIT\",",
"                                       # \"SLICE\", \"CUBE\" or \"DIR\"",
"SPLIT_SUFFIX           .%03d.fits      # Suffix expected for split FITS files",
"*SPLIT_START            1               # Suffix number of the first extension",
"SLICE_SUFFIX           .%03d.fits      # Suffix expected for sliced FITS files",
"*SLICE_START            1               # Suffix number of the first slice",
" ",
"#-------------------------------- FITS data  ----------------------------------",
" ",
"PROCESS_TYPE           NONE            # Operations on FITS data:",
"                                       # \"NONE\", \"TOBITPIX16\", \"COMPRESS\" or \"UNCOMPRESS\"",
" ",
"CHECKSUM_TYPE          NONE            # Checksum operations:",
"                                       # \"NONE\", \"COMPUTE\", \"VERIFY\" or",
"                                       # \"UPDATE\"",
"#------------------------------- Output filename  -----------------------------",
" ",
"SAVE_TYPE              BACKUP          # Behaviour towards output filename:",
"                                       # \"NONE\", \"BACKUP\", \"NEW\" or \"REPLACE\"",
" ",
"NEW_SUFFIX             .miss           # suffix to add in SAVE_TYPE NEW mode",
" ",
"#------------------------------ Miscellaneous ---------------------------------",
" ",
"VERBOSE_TYPE           NORMAL          # \"QUIET\",\"NORMAL\" or \"FULL\"",
"WRITE_XML              Y               # Write XML file (Y/N)?",
"XML_NAME               missfits.xml    # Filename for XML output",
"*XSL_URL                " XSL_URL,
"*                                       # Filename for XSL style-sheet",
#ifdef USE_THREADS
"NTHREADS               0               # Number of simultaneous threads for",
"                                       # the SMP version of " BANNER,
"                                       # 0 = automatic",
#else
"NTHREADS               1               # 1 single thread",
#endif
" ",
""
};

