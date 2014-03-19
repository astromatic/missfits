#
#				Makefile.am
#
# src Makefile.am. Process this file with automake to generate a Makefile
#
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#
#	This file part of:	MissFITS
#
#	Copyright:		(C) 2006-2010 Chiara Marmo -- IAP/CNRS
#				& Emmanuel Bertin -- IAP/CNRS/UPMC
#
#	License:		GNU General Public License
#
#	MissFITS is free software: you can redistribute it and/or modify
#	it under the terms of the GNU General Public License as published by
#	the Free Software Foundation, either version 3 of the License, or
# 	(at your option) any later version.
#	MissFITS is distributed in the hope that it will be useful,
#	but WITHOUT ANY WARRANTY; without even the implied warranty of
#	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#	GNU General Public License for more details.
#	You should have received a copy of the GNU General Public License
#	along with MissFITS.  If not, see <http://www.gnu.org/licenses/>.
#
#	Last modified:		10/10/2010
#
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%


SUBDIRS			= fits wcs
bin_PROGRAMS		= missfits
missfits_SOURCES	= custom.c file.c fitswcs.c main.c makeit.c prefs.c \
			  custom.h define.h file.h fitswcs.h globals.h key.h \
			  preflist.h prefs.h process.c process.h types.h \
			  wcscelsys.h xml.h xml.c

missfits_LDADD		= $(top_builddir)/src/fits/libfits.a \
			$(top_builddir)/src/wcs/libwcs_c.a

DATE=`date +"%Y-%m-%d"`
