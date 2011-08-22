# "foreign" means not GNU, and is common for avoiding boring messages
# about files organized differently from what GNU expects

ACLOCAL_AMFLAGS = -I m4
AUTOMAKE_OPTIONS = foreign

SUBDIRS = \
    tempi \
    src \
    tests \
    data \
    doc

EXTRA_DIST = \
    LICENSE \
    NEWS \
    README

# pkg-config file:
DISTCLEANFILES = tempi.pc
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = tempi-@TEMPI_API_VERSION@.pc

