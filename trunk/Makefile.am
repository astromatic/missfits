# Main Makefile for Missfits
# Copyright (C) 2006-2009 Emmanuel Bertin & Chiara Marmo IAP/CNRS/UPMC
AUTOMAKE_OPTIONS	= foreign no-dependencies
SUBDIRS			= man src 
dist_pkgdata_DATA	= xsl/missfits.xsl
EXTRA_DIST		= AUTHORS BUGS COPYRIGHT HISTORY INSTALL README THANKS \
			  TODO ChangeLog acx_prog_cc_optim.m4 doc \
			  acx_urbi_resolve_dir.m4
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

