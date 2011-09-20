AM_CXXFLAGS = $(LIBXML_CFLAGS) -I$(top_srcdir)  -I$(top_srcdir)/tempi

AM_LIBS = $(LIBXML_LIBS) $(OSCPACK_LIBS)

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
    filter.cpp \
    loopingplayback.cpp \
    marker.cpp \
    message.cpp \
    node.cpp \
    oscsender.cpp \
    pingpongplayback.cpp \
    player.cpp \
    recorder.cpp \
    rule.cpp \
    sink.cpp \
    sinknode.cpp \
    slide.cpp \
    slidefilter.cpp \
    source.cpp \
    sourcenode.cpp \
    state.cpp \
    statemachine.cpp \
    table.cpp \
    table_player.cpp \
    table_writer.cpp \
    tempi.cpp \
    track.cpp \
    timer.cpp

# TODO averagefilter.cpp

## Instruct libtool to include ABI version information in the generated shared
## library file (.so).  The library ABI version is defined in configure.ac, so
## that all version information is kept in one place.
libtempi_@TEMPI_API_VERSION@_la_LDFLAGS = -export-dynamic -version-info $(TEMPI_LT_VERSION)

libtempi_@TEMPI_API_VERSION@_la_LIBS = $(AM_LIBS)
libtempi_@TEMPI_API_VERSION@_la_CXXFLAGS = $(AM_CXXFLAGS)

libtempi_@TEMPI_API_VERSION@_la_includedir = $(includedir)/tempi-$(TEMPI_API_VERSION)/tempi

## PLEASE KEEP THEM IN ALPHABETICAL ORDER
libtempi_@TEMPI_API_VERSION@_la_include_HEADERS = \
    average.h \
    filter.h \
    identifier.h \
    loopingplayback.h \
    marker.h \
    message.h \
    node.h \
    oscsender.h \
    pingpongplayback.h \
    playback.h \
    player.h \
    properties.h \
    property.h \
    recorder.h \
    rule.h \
    sharedptr.h \
    sink.h \
    sinknode.h \
    slide.h \
    slidefilter.h \
    source.h \
    sourcenode.h \
    state.h \
    statemachine.h \
    table.h \
    table_player.h \
    table_writer.h \
    tempi-version.h \
    tempi.h \
    timeposition.h \
    timer.h \
    track.h

# TODO:averagefilter.h

EXTRA_DIST = config.h

