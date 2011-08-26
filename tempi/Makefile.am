AM_CXXFLAGS = $(GLIB_CFLAGS) -I$(top_srcdir)  -I$(top_srcdir)/tempi

AM_LIBS = $(GLIB_LIBS)

# use lib_LTLIBRARIES to build a shared lib:
lib_LTLIBRARIES = libtempi-@TEMPI_API_VERSION@.la

## Define the source file list for the "libtempi-@TEMPI_API_VERSION@.la"
## target.  Note that @TEMPI_API_VERSION@ is not interpreted by Automake and
## will therefore be treated as if it were literally part of the target name,
## and the variable name derived from that.
## The file extension .cc is recognized by Automake, and makes it produce
## rules which invoke the C++ compiler to produce a libtool object file (.lo)
## from each source file.  Note that it is not necessary to list header files
## which are already listed elsewhere in a _HEADERS variable assignment.

## PLEASE KEEP THEM IN ALPHABETICAL ORDER
libtempi_@TEMPI_API_VERSION@_la_SOURCES = \
    rule.cpp \
    state.cpp \
    statemachine.cpp \
    tempi-version.cpp \
    tempi.cpp

## Instruct libtool to include ABI version information in the generated shared
## library file (.so).  The library ABI version is defined in configure.ac, so
## that all version information is kept in one place.
libtempi_@TEMPI_API_VERSION@_la_LDFLAGS = -export-dynamic -version-info $(TEMPI_LT_VERSION)

libtempi_@TEMPI_API_VERSION@_la_LIBS = $(AM_LIBS)
libtempi_@TEMPI_API_VERSION@_la_CXXFLAGS = $(AM_CXXFLAGS)

libtempi_@TEMPI_API_VERSION@_la_includedir = $(includedir)/tempi-$(TEMPI_API_VERSION)/tempi

## PLEASE KEEP THEM IN ALPHABETICAL ORDER
libtempi_@TEMPI_API_VERSION@_la_include_HEADERS = \
    rule.h \
    state.h \
    statemachine.h \
    tempi-version.h \
    tempi.h

EXTRA_DIST = config.h

