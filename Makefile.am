# "foreign" means not GNU, and is common for avoiding boring messages
# about files organized differently from what GNU expects

ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = foreign

SUBDIRS = \
    tempi \
	plugins-base \
    src \
    data \
    man \
    abstractions \
    doc

EXTRA_DIST = \
    LICENSE \
    NEWS \
    README

# pkg-config file:
DISTCLEANFILES = \
    doxymain.h \
    tempi.pc
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = tempi-@TEMPI_API_VERSION@.pc

html-local: htmldoc
htmldoc:
	@if [ -z `which doxygen` ]; then echo "No Doxygen!"; else doxygen; fi

# Documentation:
if HAVE_DOXYGEN

doxyfile.stamp:
	echo "/*! \mainpage Tempi" > doxymain.h
	echo " * \verbatim" >> doxymain.h
	cat $(srcdir)/README >> doxymain.h
	echo " \endverbatim" >> doxymain.h
	echo " */" >> doxymain.h
	$(DOXYGEN) doxyfile
	echo `date -R` > doxyfile.stamp

CLEANFILES = doxyfile.stamp

all-local: doxyfile.stamp

clean-local:
	rm -rf html

endif

