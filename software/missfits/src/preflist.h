 /*
 				preflist.h

*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*
*	Part of:	MissFITS
*
*	Author:		E.BERTIN (IAP) C. MARMO (IAP)
*
*	Contents:	Keywords for the configuration file.
*
*	Last modify:	07/05/2007
*
*%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
*/

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
  {"OUTFILE_TYPE", P_KEY, &prefs.outfile_type, 0,0, 0.0,0.0,
    {"SAME", "MULTI", "SPLIT", "SLICE", "CUBE", "DIR", ""}},
  {"PROCESS_TYPE", P_KEY, &prefs.process_type, 0,0, 0.0,0.0,
    {"NONE","COMPRESS", "UNCOMPRESS", ""}},
  {"REMOVE_KEYWORD",  P_STRINGLIST, prefs.remove_key, 0,0, 0.0,0.0,
   {""}, 0, MAXKEYWORD, &prefs.nremove_key},
  {"REPLACE_KEYWORD",  P_STRINGLIST, prefs.replace_key, 0,0, 0.0,0.0,
   {""}, 0, MAXKEYWORD, &prefs.nreplace_key},
  {"SLICE_SUFFIX", P_STRING, prefs.slice_format},
  {"SLICE_KEYWORD", P_STRINGLIST, prefs.slice_key, 0,0, 0.0,0.0,
   {""}, 0, MAXKEYWORD, &prefs.nslice_key},
  {"SLICEKEY_FORMAT", P_STRING, prefs.slicekey_format},
  {"SAVE_TYPE", P_KEY, &prefs.save_type, 0,0, 0.0,0.0,
   {"NONE","NEW","BACKUP","REPLACE",""}},
  {"SPLIT_SUFFIX", P_STRING, prefs.split_format},
  {"VERBOSE_TYPE", P_KEY, &prefs.verbose_type, 0,0, 0.0,0.0,
   {"QUIET","NORMAL","FULL",""}},
  {"NTHREADS", P_INT, &prefs.nthreads, 0, THREADS_PREFMAX},
  {""}
 };

char			keylist[sizeof(key)/sizeof(pkeystruct)][16];
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
"SLICEKEY_FORMAT        %02d            # format of slice referring keywords",
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
"SPLIT_SUFFIX           .%02d.fits      # Suffix expected for split FITS files",
"SLICE_SUFFIX           .%02d.fits      # Suffix expected for sliced FITS files",
" ",
"#-------------------------------- FITS data  ----------------------------------",
" ",
"PROCESS_TYPE           NONE            # Operations on FITS data:",
"                                       # \"NONE\", \"COMPRESS\" or \"UNCOMPRESS\"",
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
#ifdef USE_THREADS
"NTHREADS        0               # Number of simultaneous threads for",
"                                # the SMP version of " BANNER,
"                                # 0 = automatic",
#else
"NTHREADS        1               # 1 single thread",
#endif
" ",
""
};

