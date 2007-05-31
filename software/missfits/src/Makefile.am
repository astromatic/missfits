# Program Makefile for MissFITS
# Copyright (C) 2002 Emmanuel Bertin.
SUBDIRS			= fits wcs
bin_PROGRAMS		= missfits
missfits_SOURCES	= custom.c file.c fitswcs.c main.c makeit.c prefs.c \
			  custom.h define.h file.h fitswcs.h globals.h key.h \
			  preflist.h prefs.h types.h wcscelsys.h xml.h xml.c

missfits_LDADD		= $(top_builddir)/src/fits/libfits.a \
			$(top_builddir)/src/wcs/libwcs_c.a

DATE=`date +"%Y-%m-%d"`
