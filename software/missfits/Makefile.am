# Main Makefile for Missfits
# Copyright (C) 2006-2008 Emmanuel Bertin, Chiara Marmo.
AUTOMAKE_OPTIONS	= foreign no-dependencies
SUBDIRS			= man src 
dist_pkgdata_DATA	= xsl/missfits.xsl
EXTRA_DIST		= AUTHORS BUGS COPYING HISTORY INSTALL README THANKS \
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

rpm-opteron:	dist
	cp -f $(PACKAGE_NAME)-$(PACKAGE_VERSION).tar.gz $(RPM_SRCDIR)
	TPXCC="gcc" TPXFLAGS="-O3 -funroll-loops -fomit-frame-pointer -Wall \
		-march=opteron" TPXLDFLAGS="-static -shared-libgcc" \
		TPXLIBS="-lm" rpmbuild -ba --target=x86_64 --clean \
		$(PACKAGE_NAME).spec
	                                                            

rpm-athlon:	dist
	cp -f $(PACKAGE_NAME)-$(PACKAGE_VERSION).tar.gz $(RPM_SRCDIR)
	TPXCC="gcc" TPXFLAGS="-O3 -funroll-loops -fomit-frame-pointer -Wall \
		-m32 -march=i686 -msse -mfpmath=sse -mtune=athlon" \
		TPXLDFLAGS="-static -shared-libgcc" TPXLIBS="-lm" \
		rpmbuild -ba --target=i686 --clean $(PACKAGE_NAME).spec


rpm-opteron-icc:	dist
	cp -f $(PACKAGE_NAME)-$(PACKAGE_VERSION).tar.gz $(RPM_SRCDIR)
	TPXCC="icc" TPXFLAGS="-O3 -g -axWPT -ip -unroll" \
		TPXLDFLAGS="-static -shared-libgcc -static-intel" \
		TPXLIBS="-lm" rpmbuild -ba \
		--target=x86_64 --clean $(PACKAGE_NAME).spec

rpm-athlon-icc:	dist
	cp -f $(PACKAGE_NAME)-$(PACKAGE_VERSION).tar.gz $(RPM_SRCDIR)
	TPXCC="icc32" TPXFLAGS="-O3 -g -axKWNPTO -ip -unroll" \
		TPXLDFLAGS="-static -shared-libgcc -static-intel" \
		TPXLIBS="-lm" rpmbuild \
		-ba --target=i686 --clean $(PACKAGE_NAME).spec

icc:
	$(MAKE) CC="icc" CFLAGS="-O3 -g -axWP -ip -unroll" LIBS="-lm"
