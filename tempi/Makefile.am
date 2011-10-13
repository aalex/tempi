AM_CXXFLAGS = \
    $(LIBXML_CFLAGS) \
    $(LIBLO_CFLAGS) \
    -I$(top_srcdir) \
    -I$(top_srcdir)/tempi

AM_LIBS = \
    $(LIBLO_LIBS) \
    $(LIBXML_LIBS)

# this file is not installed, just included by .cpp files at compile time.
EXTRA_DIST = config.h

# use lib_LTLIBRARIES to build a shared lib:
lib_LTLIBRARIES = libtempi-@TEMPI_API_VERSION@.la

## Instruct libtool to include ABI version information in the generated shared
## library file (.so).  The library ABI version is defined in configure.ac, so
## that all version information is kept in one place.
libtempi_@TEMPI_API_VERSION@_la_LDFLAGS = -export-dynamic -version-info $(TEMPI_LT_VERSION) $(AM_LIBS)
libtempi_@TEMPI_API_VERSION@_la_LIBS = $(AM_LIBS)
libtempi_@TEMPI_API_VERSION@_la_CXXFLAGS = $(AM_CXXFLAGS)
libtempi_@TEMPI_API_VERSION@_la_includedir = $(includedir)/tempi-$(TEMPI_API_VERSION)/tempi

## Define the source file list for the "libtempi-@TEMPI_API_VERSION@.la"
## target.  Note that @TEMPI_API_VERSION@ is not interpreted by Automake and
## will therefore be treated as if it were literally part of the target name,
## and the variable name derived from that.
## PLEASE KEEP THEM IN ALPHABETICAL ORDER
libtempi_@TEMPI_API_VERSION@_la_SOURCES = \
    filter.cpp \
    graph.cpp \
    sampler/loopingplayback.cpp \
    sampler/marker.cpp \
    message.cpp \
    node.cpp \
    nodefactory.cpp \
    osc/oscreceiver.cpp \
    osc/oscsender.cpp \
    sampler/pingpongplayback.cpp \
    sampler/player.cpp \
    sampler/recorder.cpp \
    fsm/rule.cpp \
    sampler/samplernode.cpp \
    sampler/score.cpp \
    sink.cpp \
    sinknode.cpp \
    mapping/slide.cpp \
    mapping/slidefilter.cpp \
    source.cpp \
    sourcenode.cpp \
    fsm/state.cpp \
    fsm/statemachine.cpp \
    table/table.cpp \
    table/table_player.cpp \
    table/table_writer.cpp \
    tempi.cpp \
    sampler/track.cpp \
    timer.cpp

## PLEASE KEEP THEM IN ALPHABETICAL ORDER
libtempi_@TEMPI_API_VERSION@_la_include_HEADERS = \
    mapping/average.h \
    concurrentqueue.h \
    exceptions.h \
    filter.h \
    graph.h \
    identifier.h \
    sampler/loopingplayback.h \
    sampler/marker.h \
    message.h \
    node.h \
    nodefactory.h \
    osc/oscreceiver.h \
    osc/oscsender.h \
    sampler/pingpongplayback.h \
    sampler/playback.h \
    sampler/player.h \
    sampler/recorder.h \
    fsm/rule.h \
    sampler/samplernode.h \
    sampler/score.h \
    sharedptr.h \
    sink.h \
    sinknode.h \
    mapping/slide.h \
    mapping/slidefilter.h \
    source.h \
    sourcenode.h \
    fsm/state.h \
    fsm/statemachine.h \
    table/table.h \
    table/table_player.h \
    table/table_writer.h \
    tempi-version.h \
    tempi.h \
    timeposition.h \
    timer.h \
    sampler/track.h \
    types.h

