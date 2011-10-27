AM_CXXFLAGS = \
    $(BOOST_CPPFLAGS) \
    $(LIBLO_CFLAGS) \
    $(LIBXML_CFLAGS) \
    $(STK_CXXFLAGS) \
    -I$(top_srcdir) \
    -I$(top_srcdir)/tempi

AM_LIBS = \
    $(BOOST_LDFLAGS) \
    $(BOOST_THREAD_LIB) \
    $(LIBLO_LIBS) \
    $(LIBXML_LIBS) \
    $(STK_LIBS)

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
    base/load.cpp \
    base/metro_node.cpp \
    base/nop_node.cpp \
    base/print_node.cpp \
    filter.cpp \
    fsm/rule.cpp \
    fsm/state.cpp \
    fsm/statemachine.cpp \
    graph.cpp \
    mapping/slide.cpp \
    mapping/slidefilter.cpp \
    message.cpp \
    midi/midiinput.cpp \
    node.cpp \
    nodefactory.cpp \
    osc/oscreceiver.cpp \
    osc/oscreceivernode.cpp \
    osc/oscsender.cpp \
    osc/oscsendernode.cpp \
    sampler/load.cpp \
    sampler/loopingplayback.cpp \
    sampler/marker.cpp \
    sampler/pingpongplayback.cpp \
    sampler/player.cpp \
    sampler/recorder.cpp \
    sampler/samplernode.cpp \
    sampler/score.cpp \
    sampler/track.cpp \
    sink.cpp \
    sinknode.cpp \
    source.cpp \
    sourcenode.cpp \
    table/table.cpp \
    table/table_player.cpp \
    table/table_writer.cpp \
    tempi.cpp \
    timeposition.cpp \
    timer.cpp \
	utils.cpp

## PLEASE KEEP THEM IN ALPHABETICAL ORDER
libtempi_@TEMPI_API_VERSION@_la_include_HEADERS = \
    base/metro_node.h \
    base/nop_node.h \
    base/print_node.h \
    base/load.h \
    concurrentqueue.h \
    exceptions.h \
    filter.h \
    fsm/rule.h \
    fsm/state.h \
    fsm/statemachine.h \
    graph.h \
    identifier.h \
    mapping/average.h \
    mapping/slide.h \
    mapping/slidefilter.h \
    message.h \
    midi/midiinput.h \
    node.h \
    nodefactory.h \
    osc/oscreceiver.h \
    osc/oscreceivernode.h \
    osc/oscsender.h \
    osc/oscsendernode.h \
    sampler/load.h \
    sampler/loopingplayback.h \
    sampler/marker.h \
    sampler/pingpongplayback.h \
    sampler/playback.h \
    sampler/player.h \
    sampler/recorder.h \
    sampler/samplernode.h \
    sampler/score.h \
    sampler/track.h \
    sharedptr.h \
    sink.h \
    sinknode.h \
    source.h \
    sourcenode.h \
    table/table.h \
    table/table_player.h \
    table/table_writer.h \
    tempi-version.h \
    tempi.h \
    timeposition.h \
    timer.h \
    utils.h

