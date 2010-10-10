#
#				Makefile.am
#
# Main Makefile.am. Process this file with automake to generate a Makefile
#
#%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
#
#	This file part of:	MissFITS
#
#	Copyright:		(C) 2006-2010 IAP/CNRS/UPMC
#
#	Authors:		Chiara Marmo (IAP)
#				Emmanuel Bertin (IAP)
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

AUTOMAKE_OPTIONS	= foreign no-dependencies
SUBDIRS			= man src 
dist_pkgdata_DATA	= xsl/missfits.xsl
EXTRA_DIST		= AUTHORS BUGS COPYRIGHT HISTORY INSTALL LICENSE \
			  README THANKS TODO ChangeLog \
			  acx_prog_cc_optim.m4 doc acx_urbi_resolve_dir.m4
RPM_ROOTDIR 		= `rpmbuild --nobuild -E %_topdir`
RPM_SRCDIR		= $(RPM_ROOTDIR)/SOURCES
dist-hook:
	rm -rf `find $(distdir) -name .svn`

rpm:	dist
	cp -f $(PACKAGE_NAME)-$(PACKAGE_VERSION).tar.gz $(RPM_SRCDIR)
	rpmbuild -ba --clean --nodeps $(PACKAGE_NAME).spec

rpm-icc:	dist
	cp -f $(PACKAGE_NAME)-$(PACKAGE_VERSION).tar.gz $(RPM_SRCDIR)
	USE_ICC="1" rpmbuild -ba --clean --nodeps $(PACKAGE_NAME).spec

rpm-best:	dist
	cp -f $(PACKAGE_NAME)-$(PACKAGE_VERSION).tar.gz $(RPM_SRCDIR)
	USE_BEST="1" rpmbuild -ba --clean --nodeps $(PACKAGE_NAME).spec

